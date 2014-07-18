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

void send_message(int epoll_fd, user_t receiver) {
    message_t message = pop_message(receiver->queue);
    send(receiver->fd, message->body, strlen(message->body), 0);
    delete_message(message);

    if (receiver->queue->n_of_messages == 0) {
        struct epoll_event e;
        e.events = EPOLLIN;
        e.data.fd = receiver->fd;
        safe_epoll_ctl(epoll_fd, EPOLL_CTL_MOD, receiver->fd, &e);
    }
}

void enqueue_message_to_others(int epoll_fd, user_manager_t manager, user_t sender, const char *body) {
    for (int i = 0; i < manager->n_of_users; i++) {
        user_t user = manager->users[i];
        if (user == sender) {
            continue;
        }
        push_message(user->queue, sender->name, body);

        struct epoll_event e;
        e.events = EPOLLIN | EPOLLOUT;
        e.data.fd = user->fd;
        safe_epoll_ctl(epoll_fd, EPOLL_CTL_MOD, user->fd, &e);
    }
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
                continue;
            }

            int fd = events[i].data.fd;
            user_t user = find_user_by_fd(manager, fd);

            if (events[i].events & EPOLLOUT) {
                send_message(epoll_fd, user);
            } else if (events[i].events & EPOLLIN) {
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                int ret = read(fd, buffer, sizeof(buffer));
                if (ret == -1) {
                    perror("read(2)");
                    exit(EXIT_FAILURE);
                } else if (ret == 0) {
                    /* connection was closed by remote host */
                    printf("user %s left.\n", user->name);
                    safe_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    delete_user_by_fd(manager, fd);
                    continue;
                }
                printf("%s:%s", user->name, buffer);

                enqueue_message_to_others(epoll_fd, manager, user, buffer);
            }
        }
    }
    return 0;
}
