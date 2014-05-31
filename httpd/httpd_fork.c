#define _POSIX_C_SOURCE 1

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "safe_functions.h"
#include "httpd_fork.h"

int main(int argc, char *argv[]) {
    suppress_zombies();

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(21600 + 1006);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int listen_fd = safe_socket(AF_INET, SOCK_STREAM, 0);
    safe_bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in));
    safe_listen(listen_fd, LISTENQ);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        printf("accepting...\n");
        int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);
        printf("accepted...\n");
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

void doit(int sock) {
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    recv(sock, buff, sizeof(buff), 0);
    printf("%s\n", buff);

    char output[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>HELLO</h1>\r\n";

    send(sock, output, sizeof(output), 0);
}
