#include "advent.h"

typedef struct Pos {
    i64 x;
    i64 y;
} Pos;


i64 difference(Pos a, Pos b) {
    i64 res = 0;
    res += llabs(a.x - b.x);
    res += llabs(a.y - b.y);
    return res;
}

int main() {
    char *line = NULL;
    char **map = NULL;
    int y = 0;
    Pos *galaxies = NULL;
    int dy = 0;
    int dx = 0;
    i64 expansion = 1000000 - 1;
    while (readline(&line)) {
        // print("%s", line);
        for (int x = 0; x < strlen(line); ++x) {
            char c = line[x];
            if (c == '#') {
                Pos p = {x, y};
                append(galaxies, p);
            }
        }
        dx = MAX(strlen(line), dx);
        y++;
        append(map, line);
    }
    dy = y;

    i64 *expandy = calloc(sizeof(i64), dy);
    i64 *expandx = calloc(sizeof(i64), dx);

    i64 value = 0;
    for (int y = 0; y < dy; ++y) {
        bool expanded = true;
        for (int x = 0; x < dx; ++x) {
            char c = map[y][x];
            if (c == '#') {
                expanded = false;
                break;
            }
        }
        if (expanded) {
            value += expansion;
        }
        expandy[y] = value;
        value++;
    }

    value = 0;
    for (int x = 0; x < dx; ++x) {
        bool expanded = true;
        for (int y = 0; y < dy; ++y) {
            char c = map[y][x];
            if (c == '#') {
                expanded = false;
                break;
            }
        }
        if (expanded) {
            value += expansion;
        }
        expandx[x] = value;
        value++;
    }

    for each(galaxy, galaxies) {
        galaxy->x = expandx[galaxy->x];
        galaxy->y = expandy[galaxy->y];
    }

    i64 res = 0;
    for (int i = 0; i < len(galaxies); ++i) {
        for (int j = i; j < len(galaxies); ++j) {
            res += difference(galaxies[i], galaxies[j]);
        }
    }

    print("%ld", res);
}
