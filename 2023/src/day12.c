#include "advent.h"

typedef struct Key {
    int a, b, c;
} Key;

struct { Key key; i64 value;} *cache = NULL;

i64 calc(char *str, int string_index, int *numbers, int number_index, int count) {

    i64 res = 0;

    Key k = {
        string_index,
        number_index,
        count
    };
    int index = hmgeti(cache, k);
    if (index != -1) {
        return cache[index].value;
    }

    if (string_index < strlen(str)) {
        char c = str[string_index++];
        if (c == '?') {
            // got '#'
            res += calc(str, string_index, numbers, number_index, count + 1);
            // got '.'
            if (count > 0 && number_index < len(numbers) && numbers[number_index] == count) {
                // found valid
                res += calc(str, string_index, numbers, number_index + 1, 0);
            } else if (count == 0) {
                res += calc(str, string_index, numbers, number_index, 0);
            }
        } else if (c == '.') {
            if (count > 0 && number_index < len(numbers) && numbers[number_index] == count) {
                // found valid
                res += calc(str, string_index, numbers, number_index + 1, 0);
            } else if (count == 0) {
                res += calc(str, string_index, numbers, number_index, 0);
            }
        } else {
            assert (c == '#');
            res += calc(str, string_index, numbers, number_index, count + 1);
        }
    } else {
        if (number_index == len(numbers) - 1 && numbers[number_index] == count) {
            res = 1;
        } else if (number_index == len(numbers) && count == 0) {
            res = 1;
        } else {
            res = 0;
        }
        // print("finish %s ni=%d count=%d", str, number_index, count);

    }
    hmput(cache, k, res);
    return res;
}


int main() {
    char *line = NULL;
    char *springs = buffer(2048);
    char *snumbers = buffer(2048);
    int *numbers = NULL;

    i64 res = 0;
    while (readline(&line)) {
        // print("%s", line);
        sscanf(line, "%s %s", springs, snumbers);
        for strsplit(tok, snumbers, ",") {
            append(numbers, atoi(tok));
        }

        int n = 5;
        char *springsext = NULL;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < strlen(springs); ++j) {
                append(springsext, springs[j]);
            }
            if (i != n - 1)
                append(springsext, '?');
        }
        append(springsext, 0);

        int *numbersext = NULL;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < len(numbers); ++j) {
                append(numbersext, numbers[j]);
            }
        }

        hmfree(cache);
        cache = NULL;
        // print("%s %lu", springsext, len(numbersext));
        res += calc(springsext, 0, numbersext, 0, 0);
        // print("res is %ld", res);
        vec_clear(numbers);
        vec_clear(springsext);
        vec_clear(numbersext);
    }
    print("%ld", res);
}
