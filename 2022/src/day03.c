#include <stdio.h>
#include <string.h>
#include <assert.h>

int counters[3][256] = {0};

int get_value(char c) {
    int value = 0;
    if (c >= 'a' && c <= 'z') {
        value = c - 'a' + 1;
    }
    if (c >= 'A' && c <= 'Z') {
        value = c - 'A' + 27;
    }
    assert(value != 0);
    return value;
}

int main() {
    char *line = NULL;
    ssize_t n = 0;
    size_t a = 0;
    int res = 0;
    int res2 = 0;

    int line_count = 0;
    while ((n = getline(&line, &a, stdin)) != -1) {
        size_t len = strlen(line) - 1;
        size_t halflen = len / 2;
        assert(len % 2 == 0);
        char c = 0;

        for (int i = 0; i < len; ++i) {
            counters[line_count][(int) line[i]]++;
        }

        if ((++line_count % 3) == 0) {

            for (int i = 0; i < 256; ++i) {
                if (counters[0][i] > 0 && counters[1][i] > 0 && counters[2][i] > 0) {
                    //int res2 = counters[0][i] + counters[1][i] + counters[2][i];
                    //printf("got %c %d %d", i, get_value(i), res2);
                    res2 += get_value(i);
                }
            }
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 256; ++j) {
                    counters[i][j] = 0;
                }
            }
            line_count = 0;
        }

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
        res += get_value(c);
    }
    printf("%d\n", res2);

}
