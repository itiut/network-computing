#ifndef _HTTPD_FORK_H_
#define _HTTPD_FORK_H_

#define LISTENQ 10

void suppress_zombies();
struct sockaddr_in create_server_addr(short port);
int create_listened_socket(struct sockaddr_in addr);
void doit(int sock);

#endif /* _HTTPD_FORK_H_ */
