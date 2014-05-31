#ifndef _SAFE_FUNCTIONS_H_
#define _SAFE_FUNCTIONS_H_

#include <pthread.h>
#include <sys/select.h>
#include <sys/socket.h>

int safe_socket(int domain, int type, int protocol);
void safe_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
void safe_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void safe_listen(int sockfd, int backlog);
int safe_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int safe_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
void safe_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

#endif /* _SAFE_FUNCTIONS_H_ */
