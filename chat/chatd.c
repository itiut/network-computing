#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "safe_functions.h"
#include "user_manager.h"
#include "chatd.h"

const short PORT = 8090;
const int LISTENQ = 10;

int main(int argc, char *argv[]) {
    struct fdinfo *listened_fds = create_listened_sockets("8090");

    int epoll_fd = safe_epoll_create1(0);
    for (struct fdinfo *fi = listened_fds; fi != NULL; fi = fi->next) {
        safe_epoll_ctl1(epoll_fd, EPOLL_CTL_ADD, fi->fd, EPOLLIN);
    }

    const int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];

    user_manager_t manager = create_user_manager();

    while (1) {
        int n_of_fds = safe_epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < n_of_fds; i++) {
            int fd = events[i].data.fd;
            if (process_listened_fds(epoll_fd, listened_fds, fd, manager)) {
                continue;
            }
            user_t user = find_user_by_fd(manager, fd);
            if (events[i].events & EPOLLOUT) {
                send_message(epoll_fd, user);
            } else if (events[i].events & EPOLLIN) {
                message_t message = receive_message_from(user);
                if (message == NULL) {
                    close_connection(epoll_fd, manager, user);
                    continue;
                }
                enqueue_message_to_others(epoll_fd, manager, user, message);
                print_message(message);
                delete_message(message);
            }
        }
    }
    return 0;
}

struct fdinfo *create_listened_sockets(const char *port) {
    struct fdinfo *listened_fds = NULL;
    struct addrinfo hints = {
        .ai_flags = AI_PASSIVE,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };
    struct addrinfo *res;
    safe_getaddrinfo(NULL, port, &hints, &res);
    for (struct addrinfo *ai = res; ai != NULL; ai = ai->ai_next) {
        int sockfd = create_listened_socket(ai);
        struct fdinfo *fi = (struct fdinfo *) safe_malloc(sizeof(struct fdinfo));
        fi->fd = sockfd;
        fi->next = listened_fds;
        listened_fds = fi;
    }
    freeaddrinfo(res);
    return listened_fds;
}

int create_listened_socket(struct addrinfo *ai) {
    int sockfd = safe_socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (ai->ai_family == AF_INET || ai->ai_family == AF_INET6) {
        int reuse = 1;
        safe_setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &reuse, sizeof(reuse));
    }
    if (ai->ai_family == AF_INET6) {
        int ipv6 = 1;
        safe_setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, (const void *) &ipv6, sizeof(ipv6));
    }
    safe_bind(sockfd, ai->ai_addr, ai->ai_addrlen);
    safe_listen(sockfd, LISTENQ);
    return sockfd;
}

bool process_listened_fds(int epoll_fd, struct fdinfo *listened_fds, int fd, user_manager_t manager) {
    for (struct fdinfo *fi = listened_fds; fi != NULL; fi = fi->next) {
        if (fi->fd == fd) {
            struct sockaddr_storage client_addr;
            socklen_t len = sizeof(client_addr);
            int connection_fd = safe_accept(fi->fd, (struct sockaddr *) &client_addr, &len);
            safe_epoll_ctl1(epoll_fd, EPOLL_CTL_ADD, connection_fd, EPOLLIN);

            add_user(manager, connection_fd, "aaa");
            printf("user %s joined.\n", "aaa");
            return true;
        }
    }
    return false;
}

void send_message(int epoll_fd, user_t receiver) {
    message_t message = pop_message(receiver->queue);
    send(receiver->fd, message->body, strlen(message->body), 0);
    delete_message(message);

    if (receiver->queue->n_of_messages == 0) {
        safe_epoll_ctl1(epoll_fd, EPOLL_CTL_MOD, receiver->fd, EPOLLIN);
    }
}

message_t receive_message_from(user_t sender) {
    char buffer[MAX_RECEIVE_BYTES];
    memset(buffer, 0, sizeof(buffer));
    int bytes = safe_read(sender->fd, buffer, sizeof(buffer));
    if (bytes == 0) {
        /* connection was closed by remote host */
        return NULL;
    }
    return create_message(sender->name, buffer);
}

void close_connection(int epoll_fd, user_manager_t manager, user_t user) {
    safe_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, user->fd, NULL);
    delete_user_by_fd(manager, user->fd);
}

void enqueue_message_to_others(int epoll_fd, user_manager_t manager, user_t sender, message_t message) {
    for (int i = 0; i < manager->n_of_users; i++) {
        user_t user = manager->users[i];
        if (user == sender) {
            continue;
        }
        enqueue_message_to(epoll_fd, user, message);
    }
}

void enqueue_message_to(int epoll_fd, user_t receiver, message_t message) {
    push_message(receiver->queue, message);
    safe_epoll_ctl1(epoll_fd, EPOLL_CTL_MOD, receiver->fd, EPOLLIN | EPOLLOUT);
}

void print_message(message_t message) {
    printf("%s %s: %s", message->timestamp, message->sender_name, message->body);
}
