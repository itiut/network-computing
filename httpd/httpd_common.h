#ifndef _HTTPD_COMMON_H_
#define _HTTPD_COMMON_H_

#define LISTENQ 10

struct sockaddr_in create_server_addr(short port);
int create_listened_socket(struct sockaddr_in addr);

#endif /* _HTTPD_COMMON_H_ */
