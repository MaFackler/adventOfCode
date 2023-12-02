#include "advent.h"

const char *texts[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
};

int search_digit(char *start) {
    for irange(j, ArraySize(texts)) {
        if (strncmp(texts[j], start, strlen(texts[j])) == 0) {
            return j + 1;
        }
    }
    return -1; 
}

int main() {
    char *line = NULL;

    int res = 0;
    while (readline(&line)) {
        //print("%s", line);
        char digits[3] = {0};
        int j = 0;
        for irange(i, strlen(line)) {
            char c = line[i];
            if (isdigit(c)) {
                digits[j++] = c;
                break;
            }
            int digit = search_digit(&line[i]);
            if (digit > 0) {
                digits[j++] = itoc(digit);
                break;
            }
        }
        for irange(i, strlen(line)) {
            int index = strlen(line) - 1 - i;
            char c = line[index];
            if (isdigit(c)) {
                digits[j++] = c;
                break;
            }
            int digit = search_digit(&line[index]);
            if (digit > 0) {
                digits[j++] = itoc(digit);
                break;
            }
        }
        assert (j == 2);
        res += atoi(&digits[0]);
    }
    print("%d", res);
}
