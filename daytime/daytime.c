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
    const char *default_ipv6_host = "2001:200:180:430:216:3eff:fe0f:19b";
    const short default_port = 13;

    short port = default_port;
    bool ipv4 = true;

    int ip_version;
    int opt;
    while ((opt = getopt_long(argc, argv, "hi:p:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'h':
            usage(argv[0]);
            exit(0);
        case 'i':
            ip_version = atoi(optarg);
            if (ip_version == 4) {
                ipv4 = true;
            } else if (ip_version == 6) {
                ipv4 = false;
            } else {
                printf("%s: Invalid IP version\n", argv[0]);
                exit(1);
            }
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case '?':
            usage(argv[0]);
            exit(1);
        }
    }

    if (optind == argc) {
        daytime((ipv4) ? default_ipv4_host : default_ipv6_host, port, ipv4);
    } else {
        daytime(argv[optind], port, ipv4);
    }
    return 0;
}

void usage(const char *program) {
    printf("Usage: %s [-h|--help] [-i <ip_version>|--ip=<ip_version>] [-p <port>|--port=<port>] [<host>]\n", program);
}

void daytime(const char *host, short port, bool ipv4) {
    int sock = create_connection(host, port, ipv4);
    FILE *f = safe_fdopen(sock, "r");
    char buf[1000];
    fgets(buf, sizeof(buf), f);
    fclose(f);
    fputs(buf, stdout);
}

int create_connection(const char *host, short port, bool ipv4) {
    if (ipv4) {
        return create_ipv4_connection(host, port);
    }
    return create_ipv6_connection(host, port);
}

int create_ipv4_connection(const char *host, short port) {
    int sock = safe_socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    safe_inet_pton(AF_INET, host, &server.sin_addr);
    safe_connect(sock, (struct sockaddr *) &server, sizeof(server));
    return sock;
}

int create_ipv6_connection(const char *host, short port) {
    int sock = safe_socket(AF_INET6, SOCK_STREAM, 0);
    struct sockaddr_in6 server;
    server.sin6_family = AF_INET6;
    server.sin6_port = htons(port);
    safe_inet_pton(AF_INET6, host, &server.sin6_addr);
    safe_connect(sock, (struct sockaddr *) &server, sizeof(server));
    return sock;
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

void safe_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int ret = connect(sockfd, addr, addrlen);
    if (ret == -1) {
        perror("safe_connect: connect(2)");
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
