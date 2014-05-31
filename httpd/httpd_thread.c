#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include "httpd_common.h"
#include "httpd_thread.h"
#include "safe_functions.h"
#include "server.h"

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr = create_server_addr(PORT);
    int listen_fd = create_listened_socket(server_addr);

    print_starting_message(server_addr);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int connection_fd = safe_accept(listen_fd, (struct sockaddr *) &client_addr, &len);

        struct thread_args *thread_args = (struct thread_args *) safe_malloc(sizeof(struct thread_args));
        thread_args->sockfd = connection_fd;

        pthread_t tid;
        safe_pthread_create(&tid, NULL, server_thread, (void *) thread_args);
    }
}

void *server_thread(void *args) {
    pthread_detach(pthread_self());

    int connection_fd = ((struct thread_args *) args)->sockfd;
    free(args);

    server_main(connection_fd);
    close(connection_fd);

    pthread_exit(EXIT_SUCCESS);
}
