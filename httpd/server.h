#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>

#define REQUEST_MAX_CHARS 4096

struct HTTPRequest {
    char *method;
    char *path;
    int http_minor_version;
};

void server_main(int sockfd);

struct HTTPRequest *read_request(int sockfd);
void parse_request_header(struct HTTPRequest *req, char *request_string);
void respond_to(struct HTTPRequest *req, int sockfd);
void free_request(struct HTTPRequest *req);

#endif /* _SERVER_H_ */
