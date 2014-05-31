#define _POSIX_C_SOURCE 1

#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "safe_functions.h"
#include "httpd_fork.h"

int main(int argc, char *argv[]) {
    suppress_zombies();

    struct sockaddr_in server_addr = create_server_addr(21600 + 1006);
    int listen_fd = create_listened_socket(server_addr);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);

        pid_t pid;
        if ((pid = fork()) == 0) {
            /* child */
            close(listen_fd);
            doit(connection_fd);
            close(connection_fd);
            exit(EXIT_SUCCESS);
        }
        /* parent */
        close(connection_fd);
    }
}

void suppress_zombies() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = SA_NOCLDWAIT;
    if (sigemptyset(&sa.sa_mask) == -1) {
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        exit(EXIT_FAILURE);
    }
}

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

void doit(int sock) {
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    recv(sock, buff, sizeof(buff), 0);
    printf("%s\n", buff);

    char output[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>HELLO</h1>\r\n";

    send(sock, output, sizeof(output), 0);
}
