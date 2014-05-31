#define _POSIX_C_SOURCE 1

#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "httpd_fork.h"
#include "safe_functions.h"
#include "server.h"

int main(int argc, char *argv[]) {
    suppress_zombies();

    struct sockaddr_in server_addr = create_server_addr(21600 + 1006);
    int listen_fd = create_listened_socket(server_addr);

    printf("Serving HTTP on %s port %d ...\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);

        pid_t pid;
        if ((pid = fork()) == 0) {
            /* child */
            close(listen_fd);
            server_main(connection_fd);
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
