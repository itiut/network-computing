#ifndef _HTTPD_THREAD_H_
#define _HTTPD_THREAD_H_

struct thread_args {
    int sockfd;
};

void *server_thread(void *args);

#endif /* _HTTPD_THREAD_H_ */
