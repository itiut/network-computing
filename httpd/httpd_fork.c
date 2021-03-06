#define _POSIX_C_SOURCE 1

#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "httpd_common.h"
#include "httpd_fork.h"
#include "safe_functions.h"
#include "server.h"

int main(int argc, char *argv[]) {
    suppress_zombies();

    struct sockaddr_in server_addr = create_server_addr(PORT);
    int listen_fd = create_listened_socket(server_addr);

    print_starting_message(server_addr);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
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
