#include <stdlib.h>
#include "safe_functions.h"
#include "message_queue.h"

message_t create_message(const char *sender_name, const char *body) {
    message_t message = (message_t) safe_malloc(sizeof(struct message));
    message->sender_name = safe_strdup(sender_name);
    message->body = safe_strdup(body);
    message->next = NULL;
    return message;
}

void delete_message(message_t message) {
    free(message->sender_name);
    free(message->body);
}

message_queue_t create_message_queue() {
    message_queue_t queue = (message_queue_t) safe_malloc(sizeof(struct message_queue));
    queue->n_of_messages = 0;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

void delete_message_queue(message_queue_t queue) {
    message_t message = queue->first;
    while (queue->first != NULL) {
        message_t next = message->next;
        delete_message(message);
        message = next;
    }
}

void push_message(message_queue_t queue, const char *sender_name, const char *body) {
    message_t message = create_message(sender_name, body);
    if (queue->n_of_messages == 0) {
        queue->first = message;
    } else {
        queue->last->next = message;
    }
    queue->last = message;
    queue->n_of_messages++;
}

message_t pop_message(message_queue_t queue) {
    if (queue->n_of_messages <= 0) {
        return NULL;
    }
    message_t popped_message = queue->first;
    if (queue->n_of_messages == 1) {
        queue->first = queue->last = NULL;
    } else {
        queue->first = popped_message->next;
    }
    queue->n_of_messages--;
    return popped_message;
}
