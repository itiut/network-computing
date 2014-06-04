#define _POSIX_C_SOURCE 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "safe_functions.h"
#include "server.h"

void server_main(int sockfd) {
    struct HTTPRequest *req = read_request(sockfd);
    respond_to(req, sockfd);
    free_request(req);
}

struct HTTPRequest *read_request(int sockfd) {
    struct HTTPRequest *req = (struct HTTPRequest *) safe_malloc(sizeof(struct HTTPRequest));

    char buffer[REQUEST_MAX_CHARS];
    memset(buffer, 0, sizeof(buffer));
    recv(sockfd, buffer, sizeof(buffer), 0);

    parse_request_header(req, buffer);

    return req;
}

void parse_request_header(struct HTTPRequest *req, char *request_string) {
    char *p = strchr(request_string, ' ');
    if (p == NULL) {
        printf("parse error on HTTP header: %s\n", request_string);
        exit(EXIT_FAILURE);
    }
    *p++ = '\0';
    req->method = (char *) safe_malloc(p - request_string);
    strcpy(req->method, request_string);

    char *path = p;
    p = strchr(path, ' ');
    if (p == NULL) {
        printf("parse error on HTTP header: %s\n", request_string);
        exit(EXIT_FAILURE);
    }
    *p++ = '\0';
    req->path = (char *) safe_malloc(p - path);
    strcpy(req->path, path);

    if (strncasecmp(p, "HTTP/1.", strlen("HTTP/1.")) != 0) {
        printf("parse error on HTTP header: %s\n", request_string);
        exit(EXIT_FAILURE);
    }
    p += strlen("HTTP/1.");
    req->http_minor_version = atoi(p);
}

void respond_to(struct HTTPRequest *req, int sockfd) {
    char *body[] = {
        "<h1>501 Not Implemented</h1>\r\n",          /* 501 */
        "",                                          /* HEAD 200 */
        "<font color=red><h1>HELLO</h1></font>\r\n", /* GET 200 */
    };

    int status[] = {
        501,
        200,
        200,
    };

    int status_id = 0;
    if (strncasecmp(req->method, "HEAD", strlen("HEAD")) == 0) {
        status_id = 1;
    }
    if (strncasecmp(req->method, "GET", strlen("GET")) == 0) {
        status_id = 2;
    }

    char output[4096];
    memset(output, 0, sizeof(output));
    snprintf(output, sizeof(output),
             "HTTP/1.0 %d OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "%s",
             status[status_id],
             body[status_id]
    );

    printf("\"%s %s HTTP/1.%d\" %d -\n", req->method, req->path, req->http_minor_version, status[status_id]);
    send(sockfd, output, sizeof(output), 0);
}

void free_request(struct HTTPRequest *req) {
    free(req->method);
    free(req->path);
    free(req);
}
