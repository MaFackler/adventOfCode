#include "advent.h"

int main() {
    int res = 0;
    char *line = NULL;
    char **map = NULL;
    int dx = 0, dy = 0;
    while (readline(&line)) {
        // print("%s", line);
        append(map, line);
        dy++;
        dx = MAX(dx, strlen(line));
    }

    for (int x = 0; x < dx; ++x) {
        for (int y = 0; y < dy - 1;) {
            char *a = &map[y][x];
            char *b = &map[y + 1][x];
            if (*a == '.' && *b == 'O') {
                char tmp = *a;
                *a = *b;
                *b = tmp;
                y = 0;
            } else {
                y++;
            }
        }
    }

    // print("---");
    for (int y = 0; y < dy; ++y) {
        for (int x = 0; x < dx; ++x) {
            if (map[y][x] == 'O') {
                res += dy - y;
            }
        }
    }

    print("%d", res);
}
