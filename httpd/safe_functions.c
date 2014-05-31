#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "safe_functions.h"

int safe_socket(int domain, int type, int protocol) {
    int sock = socket(domain, type, protocol);
    if (sock == -1) {
        perror("safe_socket: socket(2)");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void safe_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    int ret = setsockopt(sockfd, level, optname, optval, optlen);
    if (ret == -1) {
        perror("safe_setsockopt: setsockopt(2)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void safe_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int ret = bind(sockfd, addr, addrlen);
    if (ret == -1) {
        perror("safe_bind: bind(2)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void safe_listen(int sockfd, int backlog) {
    int ret = listen(sockfd, backlog);
    if (ret == -1) {
        perror("safe_listen: listen(2)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int safe_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int ret = accept(sockfd, addr, addrlen);
    if (ret == -1) {
        perror("safe_accept: accept(2)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return ret;
}

int safe_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    int ret = select(nfds, readfds, writefds, exceptfds, timeout);
    if (ret == -1) {
        perror("safe_select: select(2)");
        exit(EXIT_FAILURE);
    }
    return ret;
}

void safe_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
    int ret = pthread_create(thread, attr, start_routine, arg);
    if (ret == -1) {
        perror("safe_pthread_create: pthread_create(3)");
        exit(EXIT_FAILURE);
    }
}
