#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "daytime.h"

int main(int argc, char *argv[]) {
    _run();
    return 0;
}

void _run() {
    char * deststr = "133.11.206.167";
    int destport = 13;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(destport);
    safe_inet_pton(AF_INET, deststr, &server.sin_addr);

    connect(sock, (struct sockaddr *) &server, sizeof(server));

    FILE *f = safe_fdopen(sock, "r");

    char buf[1000];
    fgets(buf, sizeof(buf), f);
    fclose(f);
    fputs(buf, stdout);
}

void safe_inet_pton(int af, const char *src, void *dst) {
    int ret = inet_pton(af, src, dst);
    if (ret == -1) {
        perror("safe_inet_pton: inet_pton(3)");
        exit(1);
    } else if (ret == 0) {
        printf("safe_inet_pton: Invalid network address\n");
        exit(1);
    }
}

FILE *safe_fdopen(int fd, const char *mode) {
    FILE *f = fdopen(fd, mode);
    if (f == NULL) {
        perror("safe_fdopen: fdopen(3)");
        exit(1);
    }
    return f;
}
