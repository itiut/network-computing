#ifndef SAFE_FUNCTIONS_H
#define SAFE_FUNCTIONS_H

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

void *safe_malloc(size_t size);

int safe_socket(int domain, int type, int protocol);
void safe_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
void safe_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void safe_listen(int sockfd, int backlog);
int safe_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int safe_epoll_create1(int flags);
void safe_epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int safe_epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif /* SAFE_FUNCTIONS_H */
