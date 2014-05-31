#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "server.h"

void server_main(int sockfd) {
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    recv(sockfd, buff, sizeof(buff), 0);
    printf("%s\n", buff);

    char output[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>HELLO</h1>\r\n";

    send(sockfd, output, sizeof(output), 0);
}
