#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <getopt.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "daytime.h"

const struct option longopts[] = {
    {"help", no_argument,       NULL, 'h'},
    {"ip",   required_argument, NULL, 'i'},
    {"port", required_argument, NULL, 'p'},
    {0, 0, 0, 0}
};

int main(int argc, char *argv[]) {
    const char *default_ipv4_host = "133.11.206.167";
    const char *default_ipv6_host = "2001:200:180:430:216:3eff:fe49:c10e";
    const int default_port = 13;

    int opt;
    while ((opt = getopt_long(argc, argv, "h", longopts, NULL)) != -1) {
        switch (opt) {
        case 'h':
            usage(argv[0]);
            exit(0);
        case 'i':
            break;
        case 'p':
            break;
        case '?':
            usage(argv[0]);
            exit(1);
        }
    }

    daytime(default_ipv4_host, default_port, true);
    return 0;
}

void usage(const char *program) {
    printf("Usage: %s [-h|--help] [-i <ip_version>|--ip=<ip_version>] [-p <port>|--port=<port>] <host>\n", program);
}

void daytime(const char *host, short port, bool ipv4) {
    int sock = safe_socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    safe_inet_pton(AF_INET, host, &server.sin_addr);

    connect(sock, (struct sockaddr *) &server, sizeof(server));

    FILE *f = safe_fdopen(sock, "r");

    char buf[1000];
    fgets(buf, sizeof(buf), f);
    fclose(f);
    fputs(buf, stdout);
}

int safe_socket(int domain, int type, int protocol) {
    int sock = socket(domain, type, protocol);
    if (sock == -1) {
        perror("safe_socket: socket(2)");
        exit(1);
    }
    return sock;
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
