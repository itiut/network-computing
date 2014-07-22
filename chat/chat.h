#ifndef CHAT_H
#define CHAT_H

#include <curses.h>

const int CLIENT_MAX_SEND_BYTES = 1024;
const int CLIENT_MAX_RECEIVE_BYTES = 1500;

const int INPUT_WINDOW_HEIGHT = 2;

int MAX_N_OF_COLORS = 16;

struct thread_args {
    int sockfd;
    WINDOW* output_win;
};

const short colors[] = {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE
};
int n_of_colors = sizeof(colors) / sizeof(colors[0]);

typedef struct colorinfo {
    char *user_name;
    unsigned long color_pair;
    struct colorinfo *next;
} colorinfo;

int create_connected_socket(const char *host, const char *port);
void *receiver_thread(void *args);

void init_color_pairs();

struct colorinfo *create_colorinfo(int *color_number);
void colorinfo_add_user(struct colorinfo **colorinfo, const char *user_name, int *color_number);
void colorinfo_delete_user(struct colorinfo **colorinfo, const char *user_name);

unsigned long get_color_pair(int index);
void update_color_number(int *color_number);

void scan_message(char *buffer, char **timestamp, char **sender_name, char **body);
char *next_tok(char *p);

unsigned long get_color_pair_by_user_name(struct colorinfo **colorinfo, const char *user_name, int *color_number);
void print_message_with_color(WINDOW *win, const char *timestamp, const char *sender_name, const char *body, unsigned long color);

#endif /* CHAT_H */
