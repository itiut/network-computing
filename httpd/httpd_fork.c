#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "safe_functions.h"
#include "httpd_fork.h"

int main(int argc, char *argv[]) {

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
        int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);

        pid_t pid;
        if ((pid = fork()) == 0) {
            /* child */
            close(listen_fd);
            doit(connection_fd);
            close(connection_fd);
            exit(0);
        }
        /* parent */
        close(connection_fd);
    }
    return 0;
}
