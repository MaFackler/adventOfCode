#include "advent.h"

int str_charcountn(char *s, char c, size_t n) {
    assert(n <= strlen(s));
    int res = 0;
    for (size_t i = 0; i < n; ++i) {
        if (s[i] == c) {
            res++;
        }
    }
    return res;
}

int main() {
    char *line = NULL;
    readline(&line);
    int res = 0;
    const int message_marker_size = 14;

    for (int i = 0; i < strlen(line) - 1; ++i) {
        bool found = true;
        for (int j = 0; j < message_marker_size; ++j) {
            char c = line[i + j];
            int char_count = str_charcountn(&line[i], c, message_marker_size);
            if (char_count > 1) {
                found = false;
                break;
            }
            res = i + j + 1;
            //printf("char count of '%c' is %d\n", c, char_count);
        }
        if (found)
            break;
    }
    printf("%d\n", res);
}
