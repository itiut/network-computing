#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "safe_functions.h"

ssize_t safe_read(int fd, void *buf, size_t count) {
    ssize_t bytes = read(fd, buf, count);
    if (bytes == -1) {
        perror("safe_read: read(2)");
        exit(EXIT_FAILURE);
    }
    return bytes;
}

ssize_t safe_write(int fd, const void *buf, size_t count) {
    ssize_t bytes = write(fd, buf, count);
    if (bytes == -1) {
        perror("safe_write: write(2)");
        exit(EXIT_FAILURE);
    }
    return bytes;
}

void *safe_malloc(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        perror("safe_malloc: malloc(3)");
        exit(EXIT_FAILURE);
    }
    return p;
}

char *safe_strdup(const char *s) {
    char *duplicated = strdup(s);
    if (duplicated == NULL) {
        perror("safe_strdup: strdup(3)");
        exit(EXIT_FAILURE);
    }
    return duplicated;
}

void safe_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
    int ret = getaddrinfo(node, service, hints, res);
    if (ret != 0) {
        fprintf(stderr, "safe_getaddrinfo: getaddrinfo(3): %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
}

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
    int accepted_sockfd = accept(sockfd, addr, addrlen);
    if (accepted_sockfd == -1) {
        perror("safe_accept: accept(2)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return accepted_sockfd;
}

int safe_epoll_create1(int flags) {
    int epoll_fd = epoll_create1(flags);
    if (epoll_fd == -1) {
        perror("safe_epoll_create1: epoll_create1(2)");
        exit(EXIT_FAILURE);
    }
    return epoll_fd;
}

void safe_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
    int ret = epoll_ctl(epfd, op, fd, event);
    if (ret == -1) {
        perror("safe_epoll_etl: epoll_ctl(2)");
        exit(EXIT_FAILURE);
    }
}

void safe_epoll_ctl1(int epfd, int op, int fd, uint32_t events) {
    struct epoll_event event = {
        .events = events,
        .data = {
            .fd = fd
        }
    };
    safe_epoll_ctl(epfd, op, fd, &event);
}

int safe_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout) {
    int n_of_fds = epoll_wait(epfd, events, maxevents, timeout);
    if (n_of_fds == -1) {
        perror("safe_epoll_wait: epoll_wait(2)");
        exit(EXIT_FAILURE);
    }
    return n_of_fds;
}

time_t safe_time(time_t *t) {
    time_t timep = time(t);
    if (timep == (time_t) -1) {
        perror("safe_time: time(2)");
        exit(EXIT_FAILURE);
    }
    return timep;
}

struct tm *safe_localtime(const time_t *timep) {
    struct tm *tm = localtime(timep);
    if (tm == NULL) {
        perror("safe_localtime: localtime(3)");
        exit(EXIT_FAILURE);
    }
    return tm;
}

size_t safe_strftime(char *s, size_t max, const char *format, const struct tm *tm) {
    size_t bytes = strftime(s, max, format, tm);
    if (bytes == 0) {
        fprintf(stderr, "safe_strftime: strftime(3) returned 0\n");
        exit(EXIT_FAILURE);
    }
    return bytes;
}
