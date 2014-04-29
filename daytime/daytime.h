#ifndef _DAYTIME_H_
#define _DAYTIME_H_

void _run();

void safe_inet_pton(int af, const char *src, void *dst);
FILE *safe_fdopen(int fd, const char *mode);

#endif /* _DAYTIME_H_ */
