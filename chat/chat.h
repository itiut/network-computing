#ifndef CHAT_H
#define CHAT_H

#include <curses.h>

const int CLIENT_MAX_SEND_BYTES = 1024;
const int CLIENT_MAX_RECEIVE_BYTES = 1500;

const int INPUT_WINDOW_HEIGHT = 2;

struct thread_args {
    int sockfd;
    WINDOW* output_win;
};

void usage();
int create_connected_socket(const char *host, const char *port);
void *receiver_thread(void *args);

void parse_message(char *buffer, char **timestamp, char **sender_name, char **body, short *color);
char *next_tok(char *p);

void print_message_with_color(WINDOW *win, const char *timestamp, const char *sender_name, const char *body, short color);

#endif /* CHAT_H */
