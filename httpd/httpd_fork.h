#ifndef _HTTPD_FORK_H_
#define _HTTPD_FORK_H_

#define LISTENQ 10

void suppress_zombies();

void doit(int sock);

#endif /* _HTTPD_FORK_H_ */
