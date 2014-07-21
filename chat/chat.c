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

    /* receiver */
    struct thread_args *thread_args = (struct thread_args *) safe_malloc(sizeof(struct thread_args));
    thread_args->sockfd = connection_fd;
    pthread_t tid;
    pthread_create(&tid, NULL, receiver_thread, (void *) thread_args);

    /* sender */
    char buffer[CLIENT_MAX_SEND_BYTES];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
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
    free(args);

    while (1) {
        char buffer[CLIENT_MAX_RECEIVE_BYTES];
        memset(buffer, 0, sizeof(buffer));
        safe_read(connection_fd, buffer, sizeof(buffer));
        printf("%s\n", rtrim_newlines(buffer));
    }
    pthread_exit(EXIT_SUCCESS);
}
