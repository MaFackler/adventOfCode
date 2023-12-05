#include "advent.h"

int main() {
    char *line = NULL;
    int res = 0;
    list(int) numbers[2] = {0};
    while (readline(&line)) {
        char *cards = buffer(1024);
        int id = 0;
        sscanf(line, "Card %d: %[^\n]", &id, cards);

        int i = 0;
        for strsplit(token, cards, "|") {
            for strsplit(svalue, token, " ") {
                append(numbers[i], atoi(svalue));
            }
            i++;
        }

        int value = 0;
        for each(roll, numbers[1]) {
            for each(win, numbers[0]) {
                if (*roll == *win) {
                    if (value == 0) {
                        value = 1;
                    } else {
                        value *= 2;
                    }
                }
            }
        }
        vec_clear(numbers[0]);
        vec_clear(numbers[1]);
        res += value;
    }

    print("%d", res);
}
