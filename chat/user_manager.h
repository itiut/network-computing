#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <stdbool.h>

#define MAX_N_OF_USERS 100

typedef struct user {
    int fd;
    char *name;
} *user_t;

user_t create_user(int fd, const char *name);
void delete_user(user_t user);

typedef struct user_manager {
    int n_of_users;
    user_t users[MAX_N_OF_USERS];
} *user_manager_t;

user_manager_t create_user_manager();
void delete_user_manager(user_manager_t manager);

int add_user(user_manager_t manager, int fd, const char *name);
int delete_user_by_fd(user_manager_t manager, int fd);
user_t find_user_by_fd(user_manager_t manager, int fd);

#endif /* USER_MANAGER_H */
