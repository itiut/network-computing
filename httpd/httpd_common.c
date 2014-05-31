#include <arpa/inet.h>
#include <stdio.h>
#include "httpd_common.h"
#include "safe_functions.h"

struct sockaddr_in create_server_addr(short port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return addr;
}

int create_listened_socket(struct sockaddr_in addr) {
    int sockfd = safe_socket(AF_INET, SOCK_STREAM, 0);
    safe_bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    safe_listen(sockfd, LISTENQ);
    return sockfd;
}

void print_starting_message(struct sockaddr_in addr) {
    printf("Serving HTTP on %s port %d ...\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}
