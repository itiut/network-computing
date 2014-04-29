#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
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
    int s = inet_pton(AF_INET, deststr, &server.sin_addr);

    connect(sock, (struct sockaddr *) &server, sizeof(server));

    FILE *f = fdopen(sock, "r");

    char buf[1000];
    fgets(buf, sizeof(buf), f);
    fclose(f);
    fputs(buf, stdout);
}
