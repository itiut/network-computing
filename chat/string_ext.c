#include <ctype.h>
#include <string.h>

char *ltrim(char *string) {
    while (isspace(*string)) {
        string++;
    }
    return string;
}

char *rtrim_newlines(char *string) {
    char *last = string + strlen(string) - 1;
    while (*last == '\n' || *last == '\r') {
        last--;
    }
    *(last + 1) = '\0';
    return string;
}

char *rtrim_after_first_space(char *string) {
    strtok(string, " ");
    return string;
}
