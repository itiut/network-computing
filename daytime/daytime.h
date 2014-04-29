#ifndef _DAYTIME_H_
#define _DAYTIME_H_

void usage(const char *program);

bool is_ipv4_or_exit_if_invalid_ip_version(int ip_version, const char *program);

void daytime(const char *host, short port, bool ipv4);

int create_connection(const char *host, short port, bool ipv4);
int create_ipv4_connection(const char *host, short port);
int create_ipv6_connection(const char *host, short port);

int safe_socket(int domain, int type, int protocol);
void safe_inet_pton(int af, const char *src, void *dst);
void safe_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
FILE *safe_fdopen(int fd, const char *mode);
void safe_fgets(char *s, int size, FILE *stream);

#endif /* _DAYTIME_H_ */
