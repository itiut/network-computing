#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>
#include <stdlib.h>
#include "safe_functions.h"
#include "message_queue.h"
#include "user_manager.h"

user_t create_user(int fd) {
    user_t user = (user_t) safe_malloc(sizeof(struct user));
    user->fd = fd;
    user->state = CONNECTED;
    user->name = NULL;
    user->queue = create_message_queue();
    return user;
}

void delete_user(user_t user) {
    if (user->name) {
        free(user->name);
    }
    delete_message_queue(user->queue);
    free(user);
}

user_manager_t create_user_manager() {
    user_manager_t manager = (user_manager_t) safe_malloc(sizeof(struct user_manager));
    manager->n_of_users = 0;
    for (int i = 0; i < MAX_N_OF_USERS; i++) {
        manager->users[i] = NULL;
    }
    return manager;
}

void delete_user_manager(user_manager_t manager) {
    free(manager);
}

int add_user(user_manager_t manager, int fd) {
    if (manager->n_of_users >= MAX_N_OF_USERS) {
        /* cannot add a user */
        return -1;
    }
    manager->users[manager->n_of_users++] = create_user(fd);
    return 1;
}

int delete_user_by_fd(user_manager_t manager, int fd) {
    int i = 0;
    while (i < manager->n_of_users && manager->users[i]->fd != fd) {
        i++;
    }
    if (i == manager->n_of_users) {
        /* cannot found the user with the fd */
        return -1;
    }
    delete_user(manager->users[i]);
    while (i < manager->n_of_users - 1) {
        manager->users[i] = manager->users[i + 1];
        i++;
    }
    manager->n_of_users--;
    return 1;
}

user_t find_user_by_fd(user_manager_t manager, int fd) {
    for (int i = 0; i < manager->n_of_users; i++) {
        if (manager->users[i]->fd == fd) {
            return manager->users[i];
        }
    }
    return NULL;
}
