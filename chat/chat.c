#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <arpa/inet.h>
#include <curses.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "safe_functions.h"
#include "string_ext.h"
#include "chat.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *host = argv[1];
    const char *port = argv[2];
    int connection_fd = create_connected_socket(host, port);
    if (connection_fd == -1) {
        fprintf(stderr, "Failed in creating connection to %s:%s\n", host, port);
        exit(EXIT_FAILURE);
    }

    initscr();
    init_color_pairs();
    WINDOW *input_win = newwin(INPUT_WINDOW_HEIGHT, COLS, LINES - INPUT_WINDOW_HEIGHT - 1, 0);
    scrollok(input_win, TRUE);
    WINDOW *output_win = newwin(LINES - INPUT_WINDOW_HEIGHT - 1, COLS, 0, 0);
    scrollok(output_win, TRUE);

    /* receiver */
    struct thread_args *thread_args = (struct thread_args *) safe_malloc(sizeof(struct thread_args));
    thread_args->sockfd = connection_fd;
    thread_args->output_win = output_win;
    pthread_t tid;
    pthread_create(&tid, NULL, receiver_thread, (void *) thread_args);

    /* sender */
    while (1) {
        wclear(input_win);
        mvwprintw(input_win, 0, 0, "> ");
        wrefresh(input_win);

        char buffer[CLIENT_MAX_SEND_BYTES];
        wgetnstr(input_win, buffer, sizeof(buffer));

        rtrim_newlines(buffer);
        size_t length = strlen(buffer);
        if (length == 0) {
            continue;
        }
        safe_write(connection_fd, buffer, length);
    }
}

int create_connected_socket(const char *host, const char *port) {
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };
    struct addrinfo *res;
    safe_getaddrinfo(host, port, &hints, &res);
    for (struct addrinfo *ai = res; ai != NULL; ai = ai->ai_next) {
        int connection_fd = safe_socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (connect(connection_fd, ai->ai_addr, ai->ai_addrlen) == 0) {
            freeaddrinfo(res);
            return connection_fd;
        }
    }
    freeaddrinfo(res);
    return -1;
}

void *receiver_thread(void *args) {
    pthread_detach(pthread_self());

    int connection_fd = ((struct thread_args *) args)->sockfd;
    WINDOW *output_win = ((struct thread_args *) args)->output_win;
    free(args);

    int color_number;
    struct colorinfo *colorinfo = create_colorinfo(&color_number);

    while (1) {
        char buffer[CLIENT_MAX_RECEIVE_BYTES];
        memset(buffer, 0, sizeof(buffer));
        int bytes = safe_read(connection_fd, buffer, sizeof(buffer));
        if (bytes == 0) {
            exit(EXIT_SUCCESS);
        }

        char *timestamp, *sender_name, *body;
        scan_message(rtrim_newlines(buffer), &timestamp, &sender_name, &body);
        unsigned long color_pair = get_color_pair_by_user_name(&colorinfo, sender_name, &color_number);

        print_message_with_color(output_win, timestamp, sender_name, body, color_pair);
        wrefresh(output_win);

        /* delete left user */
        if (strcmp(sender_name, "[system]:") == 0) {
            char *command = next_tok(body);
            if (strcmp(command, "left.") == 0) {
                colorinfo_delete_user(&colorinfo, body);
            }
        }
    }
}

void init_color_pairs() {
    start_color();
    if (n_of_colors > COLOR_PAIRS) {
        n_of_colors = COLOR_PAIRS;
    }
    for (int i = 0; i < n_of_colors; i++) {
        init_pair(i + 1, colors[i], COLOR_BLACK);
    }
}

struct colorinfo *create_colorinfo(int *color_number) {
    *color_number = 0;
    struct colorinfo *colorinfo = NULL;
    colorinfo_add_user(&colorinfo, "[system]", color_number);
    return colorinfo;
}

void colorinfo_add_user(struct colorinfo **colorinfo, const char *user_name, int *color_number) {
    struct colorinfo *ci = (struct colorinfo *) safe_malloc(sizeof(struct colorinfo));
    ci->user_name = strdup(user_name);
    ci->color_pair = get_color_pair(*color_number);
    ci->next = *colorinfo;
    *colorinfo = ci;

    update_color_number(color_number);
}

void colorinfo_delete_user(struct colorinfo **colorinfo, const char *user_name) {
    size_t length = strlen(user_name);
    for (struct colorinfo *prev = NULL, *ci = *colorinfo; ci != NULL; prev = ci, ci = ci->next) {
        if (strncmp(ci->user_name, user_name, length) != 0) {
            continue;
        }
        if (prev == NULL) {
            *colorinfo = ci->next;
        } else {
            prev->next = ci->next;
        }
        free(ci->user_name);
        free(ci);
        return;
    }
}

unsigned long get_color_pair(int num) {
    if (num <= n_of_colors) {
        return COLOR_PAIR(num) | A_BOLD;
    } else {
        exit(EXIT_FAILURE);
    }
}

void update_color_number(int *num) {
    *num = (*num + 1) % n_of_colors;
}

void scan_message(char *buffer, char **timestamp, char **sender_name, char **body) {
    *timestamp = buffer;
    *sender_name = next_tok(*timestamp);
    *body = next_tok(*sender_name);
}

char *next_tok(char *p) {
    char *next = strchr(p, ' ');
    *next++ = '\0';
    return next;
}

unsigned long get_color_pair_by_user_name(struct colorinfo **colorinfo, const char *user_name, int *color_number) {
    for (struct colorinfo *ci = *colorinfo; ci != NULL; ci = ci->next) {
        if (strcmp(ci->user_name, user_name) == 0) {
            return ci->color_pair;
        }
    }

    colorinfo_add_user(colorinfo, user_name, color_number);
    return (*colorinfo)->color_pair;
}

void print_message_with_color(WINDOW *win, const char *timestamp, const char *sender_name, const char *body, unsigned long color) {
    wprintw(win, "%s ", timestamp);
    wattron(win, color);
    wprintw(win, "%s ", sender_name);
    wattroff(win, color);
    wprintw(win, "%s\n", body);
}
