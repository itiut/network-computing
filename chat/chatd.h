#ifndef CHATD_H
#define CHATD_H

#include "user_manager.h"

struct sockaddr_in create_server_addr(short port);
int create_listened_socket(struct sockaddr_in addr);

void send_message(int epoll_fd, user_t receiver);
void receive_message(int epoll_fd, user_manager_t manager, user_t sender);
void enqueue_message_to_others(int epoll_fd, user_manager_t manager, user_t sender, const char *body);

#endif /* CHATD_H */
