#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include "httpd_common.h"
#include "httpd_select.h"
#include "safe_functions.h"
#include "server.h"

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr = create_server_addr(PORT);
    int listen_fd = create_listened_socket(server_addr);

    print_starting_message(server_addr);

    fd_set fds, saved_fds;
    FD_ZERO(&saved_fds);
    FD_SET(listen_fd, &saved_fds);

    struct timeval waitval = {
        .tv_sec = 2,
        .tv_usec = 500
    };

    while (1) {
        memcpy(&fds, &saved_fds, sizeof(saved_fds));

        safe_select(FD_SETSIZE, &fds, NULL, NULL, &waitval);

        if (FD_ISSET(listen_fd, &fds)) {
            FD_CLR(listen_fd, &fds);

            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);

            FD_SET(connection_fd, &saved_fds);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &fds)) {
                server_main(i);
                close(i);
                FD_CLR(i, &saved_fds);
            }
        }
    }
}
