#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
    char *line = NULL;
    ssize_t n = 0;
    size_t a = 0;
    int res = 0;

    while ((n = getline(&line, &a, stdin)) != -1) {
        size_t len = strlen(line) - 1;
        size_t halflen = len / 2;
        assert(len % 2 == 0);
        char c = 0;

        for (int i = 0; i < halflen; ++i) {
            for (int j = 0; j < halflen; ++j) {
                if (line[i] == line[halflen + j]) {
                    //printf("%c\n", line[i]);
                    c = line[i];
                    goto skip;
                }
            }
        }
        skip:
        int value = 0;
        if (c >= 'a' && c <= 'z') {
            value = c - 'a' + 1;
        }
        if (c >= 'A' && c <= 'Z') {
            value = c - 'A' + 27;
        }
        assert(value != 0);
        res += value;
    }
    printf("%d\n", res);

}
