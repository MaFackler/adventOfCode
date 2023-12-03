#include "advent.h"

bool is_symbol(char c) {
    return !isdigit(c) && c != '.';
}

bool check_gear(list(char *) lines, int row, int start, int end, int *dr, int *dc) {
    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = start - 1; c <= end; ++c) {
            if (r >= 0 && r < len(lines) && c >= 0 && c < strlen(lines[r])) {
                char cc = lines[r][c];
                if (cc == '*') {
                    *dc = c;
                    *dr = r;
                    return true;
                }
            }
        }
    }
    return false;
}

typedef struct Gear {
    int r, c;
    int n, value;
} Gear;

int main() {
    char *line = NULL;
    int res = 0;
    list(char *) lines = NULL;
    while (readline(&line)) {
        append(lines, line);
    }
    list(Gear) gears = NULL;

    for (int r = 0; r < len(lines); ++r) {
        line = lines[r];
        // print("%s", line);
        for (int c = 0; c < strlen(line); ++c) {
            char cc = line[c];
            if (isdigit(cc)) {
                int value = atoi(&line[c]);
                int start = c;
                while (isdigit(line[c])) {
                    c++;
                }
                int end = c;
                int dr = 0;
                int dc = 0;
                if (check_gear(lines, r, start, end, &dr, &dc)) {
                    bool func(Gear *it) {
                        return it->r == dr && it->c == dc;
                    }
                    Gear *found = find(gears, func);
                    if (found) {
                        found->n++;
                        found->value *= value;
                    } else {
                        *add(gears) = (Gear){
                            .r=dr,
                            .c=dc,
                            .value=value,
                            .n=1
                        };
                    }
                }
            }
        }
    }

    for each(gear, gears) {
        if (gear->n == 2) {
            res += gear->value;
        }
    }

    print("%d", res);
}
