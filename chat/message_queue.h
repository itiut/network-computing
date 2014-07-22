#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdlib.h>

typedef struct message {
    char timestamp[12];         /* HH:MM:SS */
    char *sender_name;
    char *body;
    struct message *next;
    size_t bytes;
} *message_t;

typedef struct message_queue {
    int n_of_messages;
    message_t first;
    message_t last;
} *message_queue_t;

void fill_timestamp(char *timestamp, size_t length);

message_t create_message(const char *sender_name, const char *body);
message_t copy_message(const message_t m0);
void delete_message(message_t message);

message_queue_t create_message_queue();
void delete_message_queue(message_queue_t queue);

void push_message(message_queue_t queue, message_t m0);
message_t pop_message(message_queue_t queue);

#endif /* MESSAGE_QUEUE_H */
