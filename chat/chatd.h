#ifndef CHATD_H
#define CHATD_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <netdb.h>
#include <stdbool.h>
#include "user_manager.h"

const int MAX_RECEIVE_BYTES = 1024;

typedef struct fdinfo {
    int fd;
    struct fdinfo *next;
} fdinfo;

struct fdinfo *create_listened_sockets(const char *port);
int create_listened_socket(struct addrinfo *ai);

bool process_listened_fds(int epoll_fd, struct fdinfo *listened_fds, int fd, user_manager_t manager);

void send_message(int epoll_fd, user_t receiver);

message_t receive_message_from(user_t sender);
void close_connection(int epoll_fd, user_manager_t manager, user_t user);
void enqueue_message_to_others(int epoll_fd, user_manager_t manager, user_t sender, message_t message);
void enqueue_message_to(int epoll_fd, user_t receiver, message_t message);

void print_message(message_t message);

#endif /* CHATD_H */
