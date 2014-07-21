#ifndef CHAT_H
#define CHAT_H

const int CLIENT_MAX_SEND_BYTES = 1024;
const int CLIENT_MAX_RECEIVE_BYTES = 1500;

struct thread_args {
    int sockfd;
};

void usage();
int create_connected_socket(const char *host, const char *port);
void *receiver_thread(void *args);

#endif /* CHAT_H */
