#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "safe_functions.h"
#include "user_manager.h"
#include "chatd.h"

const short PORT = 8090;
const int LISTENQ = 10;

/* void make_socket_nonblocking(int sockfd) { */
/*     int current_flags = fcntl(sockfd, F_GETFL, 0); */
/*     if (current_flags == -1) { */
/*         perror("make_socket_nonbloking: fcntl(2), F_GETFL"); */
/*         exit(EXIT_FAILURE); */
/*     } */
/*     if (fcntl(sockfd, F_SETFL, current_flags | O_NONBLOCK) == -1) { */
/*         perror("make_socket_nonblocking: fcntl(2), F_SETFL"); */
/*         exit(EXIT_FAILURE); */
/*     } */
/* } */

struct sockaddr_in create_server_addr(short port) {
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = {
            .s_addr = htonl(INADDR_ANY)
        },
        .sin_port = htons(port)
    };
    return addr;
}

int create_listened_socket(struct sockaddr_in addr) {
    int sockfd = safe_socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    safe_setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &reuse, sizeof(reuse));
    safe_bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    safe_listen(sockfd, LISTENQ);
    return sockfd;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr = create_server_addr(PORT);
    int listen_fd = create_listened_socket(server_addr);

    int epoll_fd = safe_epoll_create1(0);
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    safe_epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);

    const int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];

    user_manager_t manager = create_user_manager();

    while (1) {
        int n_of_fds = safe_epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < n_of_fds; i++) {
            if (events[i].data.fd == listen_fd) {
                struct sockaddr_in client_addr;
                socklen_t len = sizeof(client_addr);
                int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);

                struct epoll_event e;
                e.events = EPOLLIN;
                e.data.fd = connection_fd;
                safe_epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connection_fd, &e);

                add_user(manager, connection_fd, "aaa");
                printf("user %s joined.\n", "aaa");
            } else {
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));

                int fd = events[i].data.fd;
                read(fd, buffer, sizeof(buffer));

                user_t user = find_user_by_fd(manager, fd);
                printf("%s:%s", user->name, buffer);
            }
        }
    }
    return 0;
}
