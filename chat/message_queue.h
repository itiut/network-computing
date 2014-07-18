#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

typedef struct message {
    char *sender_name;
    char *body;
    struct message *next;
} *message_t;

typedef struct message_queue {
    int n_of_messages;
    message_t first;
    message_t last;
} *message_queue_t;

message_t create_message(const char *sender_name, const char *body);
void delete_message(message_t message);

message_queue_t create_message_queue();
void delete_message_queue(message_queue_t queue);

void push_message(message_queue_t queue, const char *sender_name, const char *body);
message_t pop_message(message_queue_t queue);

#endif /* MESSAGE_QUEUE_H */
