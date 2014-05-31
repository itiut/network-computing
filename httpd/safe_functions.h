#ifndef _SAFE_FUNCTIONS_H_
#define _SAFE_FUNCTIONS_H_

#include <sys/socket.h>

int safe_socket(int domain, int type, int protocol);
void safe_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void safe_listen(int sockfd, int backlog);
int safe_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#endif /* _SAFE_FUNCTIONS_H_ */
