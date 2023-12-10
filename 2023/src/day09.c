#include "advent.h"

int *reduce(int *numbers) {
    int *res = NULL;
    for (int i = 0; i < len(numbers) - 1; ++i) {
        int a = numbers[i];
        int b = numbers[i + 1];
        int d = b - a;
        append(res, d);
    }
    return res;
}

bool is_finished(int *numbers) {
    for each(number, numbers) {
        if (*number != 0) {
            return false;
        }
    }
    return true;
}

int main() {
    char *line = NULL;
    int res = 0;
    while (readline(&line)) {
        // print("%s", line);
        int **tree = NULL;
        int *numbers = NULL;
        for strsplit(tok, line, " ") {
            append(numbers, atoi(tok));
        }

        int *current = numbers;
        while (!is_finished(current)) {
            // print_ints(current);
            append(tree, current);
            current = reduce(current);
        }
        append(tree, current);

        append(tree[len(tree) - 1], 0);
        int new_value = 0;
        for (int i = len(tree) - 1; i > 0; --i) {
            int *bottom = tree[i];
            int *top = tree[i - 1];
            int value = bottom[len(bottom) - 1] + top[len(top) - 1];
            append(top, value);
            // NOTE: reallocation can happen so pointer changes...
            tree[i - 1] = top;
            // print("- %d", value);
            new_value = value;
        }
        res += new_value;
    }

    print("%d", res);
}
