#ifndef _DAYTIME_H_
#define _DAYTIME_H_

void usage(const char *program);

void daytime(const char *host, short port, bool ipv4);

int create_connection(const char *host, short port, bool ipv4);

int safe_socket(int domain, int type, int protocol);
void safe_inet_pton(int af, const char *src, void *dst);
void safe_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
FILE *safe_fdopen(int fd, const char *mode);

#endif /* _DAYTIME_H_ */
