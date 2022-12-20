#include "advent.h"

typedef struct {
    int x;
    int y;
} v2;

bool has_rock(v2 *rocks, v2 a) {
    bool found = false;
    for (int i = 0; i < vec_size(rocks); ++i) {
        v2 r = rocks[i];
        if (r.x == a.x && r.y == a.y) {
            found = true;
            break;
        }
    }
    return found;
}

v2* push_rock(v2 *rocks, v2 a) {
    if (!has_rock(rocks, a))
        vec_push(rocks, a);

    return rocks;
}

int main() {
    char *line = NULL;

    int y_dim = 0;
    v2 *rocks = NULL;
    while (readline(&line)) {
        char *token = strtok(line, " -> ");
        int x, y;
        int xprev = -1;
        int yprev = -1;
        while (token != NULL) {
            sscanf(token, "%d,%d", &x, &y);
            y_dim = MAX(y, y_dim);
            token = strtok(NULL, " -> ");

            if (xprev != -1 && yprev != -1) {
                for (int yy = MIN(y, yprev); yy <= MAX(y, yprev); ++yy) {
                    for (int xx = MIN(x, xprev); xx <= MAX(x, xprev); ++xx) {
                        rocks = push_rock(rocks, ((v2) {xx, yy}));
                    }
                }
            }
            xprev = x;
            yprev = y;
        }
    }


    y_dim++;
    int floor_index = y_dim + 1;
    v2 sand = {500, 0};
    int res = 0;
    while (1) {
        bool free_space = false;
        if (!has_rock(rocks, (v2) {sand.x, sand.y + 1})) {
            sand.y++;
            free_space = true;
        } else if (!has_rock(rocks, (v2) {sand.x - 1, sand.y + 1})) {
            sand.x--;
            sand.y++;
            free_space = true;
        } else if (!has_rock(rocks, (v2) {sand.x + 1, sand.y + 1})) {
            sand.x++;
            sand.y++;
            free_space = true;
        }

        if (free_space) {
            if (sand.y == floor_index) {
                rocks = push_rock(rocks, sand);
                sand.x = 500;
                sand.y = 0;
                continue;
            }

        } else {
            rocks = push_rock(rocks, sand);
            res++;
            if (sand.x == 500 && sand.y == 0) {
                break;
            }
            sand.x = 500;
            sand.y = 0;
        }
    }
    printf("%d\n", res);
}
