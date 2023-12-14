#include "advent.h"

int check_same_col(char **map, int dy, int c1, int c2) {
    int res = 0;
    for (int y = 0; y < dy; ++y) {
        if (map[y][c1] != map[y][c2]) {
            res++;
        }
    }
    return res;
}

int check_same_row(char **map, int dx, int r1, int r2) {
    int res = 0;
    for (int x = 0; x < dx; ++x) {
        if (map[r1][x] != map[r2][x]) {
            res++;
        }
    }
    return res;
}

int solve(char **map, int dx, int dy) {
    int res = 0;
    for (int x = 0; x < dx - 1; ++x) {
        int left = x;
        int right = x + 1;
        // int value = 0;
        int smudge = 0;
        while (left >= 0 && right < dx) {
            smudge += check_same_col(map, dy, left, right);
            left--;
            right++;
        }

        if (smudge == 1) {
            res = x + 1;
        }
    }

    for (int y = 0; y < dy - 1; ++y) {
        int top = y;
        int bottom = y + 1;
        // int value = 0;
        int smudge = 0;
        while (top >= 0 && bottom < dy) {
            smudge += check_same_row(map, dx, top, bottom);
            top--;
            bottom++;
        }
        if (smudge == 1) {
            if (y + 1 > res) {
                res = (y + 1) * 100;
            }
        }
    }
    return res;
}


int main() {
    char *line = NULL;
    char **map = NULL;
    int dx = 0;
    int dy = 0;
    int res = 0;
    while (readline(&line)) {
        if (strlen(line)) {
            // print("%s", line);
            append(map, line);
            dy++;
            dx = MAX(dx, strlen(line));
        } else {

            res += solve(map, dx, dy);
            vec_clear(map);
            map = NULL;
            dx = 0;
            dy = 0;
        }
    }

    res += solve(map, dx, dy);

    print("%d", res);
}
