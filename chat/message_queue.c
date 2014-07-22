#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <string.h>
#include "safe_functions.h"
#include "message_queue.h"

const char *MESSAGE_TIMESTAMP_FORMAT = "%T"; /* %H:%M:%S */

void fill_timestamp(char *timestamp, size_t length) {
    time_t timep = safe_time(NULL);
    struct tm *tm = safe_localtime(&timep);
    safe_strftime(timestamp, length, MESSAGE_TIMESTAMP_FORMAT, tm);
}

message_t create_message(const char *sender_name, const char *body) {
    message_t message = (message_t) safe_malloc(sizeof(struct message));
    fill_timestamp(message->timestamp, sizeof(message->timestamp));
    message->sender_name = safe_strdup(sender_name);
    message->body = safe_strdup(body);
    message->next = NULL;
    message->bytes = strlen(message->timestamp) + strlen(message->sender_name) + strlen(message->body);
    return message;
}

message_t copy_message(const message_t m0) {
    message_t m1 = (message_t) safe_malloc(sizeof(struct message));
    strncpy(m1->timestamp, m0->timestamp, sizeof(m1->timestamp));
    m1->sender_name = safe_strdup(m0->sender_name);
    m1->body = safe_strdup(m0->body);
    m1->next = NULL;
    m1->bytes = m0->bytes;
    return m1;
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

void push_message(message_queue_t queue, message_t m0) {
    message_t m1 = copy_message(m0);
    if (queue->n_of_messages == 0) {
        queue->first = m1;
    } else {
        queue->last->next = m1;
    }
    queue->last = m1;
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
