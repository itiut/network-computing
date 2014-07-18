#ifndef SAFE_FUNCTIONS_H
#define SAFE_FUNCTIONS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

void *safe_malloc(size_t size);

char *safe_strdup(const char *s);

void safe_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int safe_socket(int domain, int type, int protocol);
void safe_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
void safe_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void safe_listen(int sockfd, int backlog);
int safe_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int safe_epoll_create1(int flags);
void safe_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
void safe_epoll_ctl1(int epfd, int op, int fd, uint32_t events);
int safe_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif /* SAFE_FUNCTIONS_H */
