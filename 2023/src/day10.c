#include "advent.h"

typedef enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    DIRECTION_COUNT,
} Direction;

typedef struct Pos {
    int x;
    int y;
} Pos;

typedef struct Traversal {
    Pos pos;
    Direction came_from;
    int weight;
} Traversal;

typedef struct Pipe {
    Direction outputs[2];
} Pipe;

Pipe pipes[256] = {
    ['-'] = {{ WEST, EAST }},
    ['|'] = {{ NORTH, SOUTH }},
    ['J'] = {{ WEST, NORTH }},
    ['F'] = {{ SOUTH, EAST }},
    ['L'] = {{ NORTH, EAST }},
    ['7'] = {{ SOUTH, WEST }},
};

bool pipe_has_direction(Pipe *pipe, Direction dir) {
    for (int i = 0; i < ArraySize(pipe->outputs); ++i) {
        if (pipe->outputs[i] == dir) {
            return true;
        }
    }
    return false;
}

char get_char_for_directions(Direction a, Direction b) {
    for (int i = 0; i < ArraySize(pipes); ++i) {
        char c = (char) i;
        if (c == '-' || c == '|' || c == 'J' || c == 'F' || c == 'L' || c == '7') {
            Pipe pipe = pipes[i];
            if (pipe_has_direction(&pipe, a) && pipe_has_direction(&pipe, b)) {
                return c;
            }
        }
    }
    INVALID;
    return '0';
}


Direction pipe_get_other_direction(Pipe *pipe, Direction dir) {
    assert (pipe_has_direction(pipe, dir) == true);
    for (int i = 0; i < ArraySize(pipe->outputs); ++i) {
        if (pipe->outputs[i] != dir) {
            return pipe->outputs[i];
        }
    }
    INVALID;
    return 0;
}

Pos directions[] = {
    [NORTH] = {0, -1},
    [EAST] = {1, 0},
    [SOUTH] = {0, 1},
    [WEST] = {-1, 0},
};

Pos pos_add_direction(Pos a, Direction dir) {
    Pos d = directions[dir];
    Pos res = {
        .x=a.x + d.x,
        .y=a.y + d.y,
    };
    return res;
}

Direction direction_inverse(Direction dir) {
    switch (dir) {
        case NORTH:
            return SOUTH;
            break;
        case SOUTH:
            return NORTH;
            break;
        case EAST:
            return WEST;
            break;
        case WEST:
            return EAST;
            break;
        default:
            INVALID;
            break;
    }
}

Direction get_next_direction(char **map, Pos pos, Direction came_from) {
    char c = map[pos.y][pos.x];
    assert (c != '.');
    Direction res = pipe_get_other_direction(&pipes[(int) c], came_from);
    return res;
}

int raytracex(char **map, int dx, Pos pos, Direction dir) {
    int res = 0;

    pos = pos_add_direction(pos, dir);
    while (inrange(pos.x, 0, dx)) {
        char c = map[pos.y][pos.x];
        // J, F, 7, L
        // F---J
        // L---7
        if (c == '|') {
            res++;
        } else if (c == 'J') {
            res++;
        } else if (c == 'L') {
            res++;
        }

        pos = pos_add_direction(pos, dir);
    }
    return res;
}

int raytracey(char **map, int dy, Pos pos, Direction dir) {
    int res = 0;

    pos = pos_add_direction(pos, dir);
    while (inrange(pos.y, 0, dy)) {
        char c = map[pos.y][pos.x];
        // J, F, 7, L
        // F
        // |
        // J
        //
        // 7
        // |
        // L
        if (c == '-') {
            res++;
        } else if (c == 'F') {
            res++;
        } else if (c == '7') {
            res++;
        }

        pos = pos_add_direction(pos, dir);
    }
    return res;
}

int main() {
    char *line = NULL;
    char **map = NULL;
    char **only_pipe = NULL;
    int res = 0;
    int dx = 0;
    int dy = 0;
    while (readline(&line)) {
        append(map, strdup(line));
        append(only_pipe, strdup(line));
        dx = MAX(strlen(line), dx);

        // print("%s", line);
    }
    dy = len(map);
    Pos spos = {0};

    Direction *sdirections = NULL;
    Traversal *starts = NULL;
    for (int y = 0; y < dy; ++y) {
        for (int x = 0; x < dx; ++x) {
            Pos p = {x, y};
            char c = map[p.y][p.x];
            if (c == 'S') {
                spos.x = x;
                spos.y = y;
                for (int i = 0; i < DIRECTION_COUNT; ++i) {
                    Pos dest = pos_add_direction(p, i);
                    if (inrange(dest.x, 0, dx) && inrange(dest.y, 0, dy)) {
                        Direction came_from = direction_inverse(i);
                        char dc = map[dest.y][dest.x];
                        if (dc != '.' && pipe_has_direction(&pipes[(int) dc], came_from)) {
                            append(sdirections, i);
                            Traversal t = {
                                .pos=dest,
                                .came_from=came_from,
                                .weight=1,
                            };
                            append(starts, t);
                        }
                    }
                }
            }
        }
    }
    assert (len(sdirections) == 2);
    print("%d %d", sdirections[0], sdirections[1]);
    char schar = get_char_for_directions(sdirections[0], sdirections[1]);
    assert (len(starts) == 2);
    int *weights = calloc(sizeof(int), dx * dy);

    while (1) {
        for each(t, starts) {
            // print("%d %d %c", t->pos.x, t->pos.y, map[t->pos.y][t->pos.x]);
            int *w = &weights[t->pos.y * dx + t->pos.x];
            if (*w > 0) {
                goto finish;
            }
            *w = t->weight++;
            res = MAX(*w, res);
            Direction dir = get_next_direction(map, t->pos, t->came_from);
            t->pos = pos_add_direction(t->pos, dir);
            t->came_from = direction_inverse(dir);
        }
    }
finish:
#if 1
    for (int y = 0; y < dy; ++y) {
        for (int x = 0; x < dx; ++x) {
            char c = '.';
            if (weights[y * dx + x]) {
                c = map[y][x];
            }
            if (y == spos.y && x == spos.x) {
                c = schar;
            }
            only_pipe[y][x] = c;
            // printf("%c", c);
        }
        // printf("\n");
    }
#endif


#if 1
    print("\n######\n");
    res = 0;
    for (int y = 0; y < dy; ++y) {
        for (int x = 0; x < dx; ++x) {
            char c = only_pipe[y][x];
            if (c == '.') {
                int resw = raytracex(only_pipe, dx, (Pos) {x, y}, WEST);
                int rese = raytracex(only_pipe, dx, (Pos) {x, y}, EAST);
                // int resn = raytracey(map, dy, (Pos) {x, y}, NORTH);
                //int ress = raytracey(map, dy, (Pos) {x, y}, SOUTH);
                // if (resw % 2 == 1 && rese % 2 == 1 && resn % 2 == 1 && ress % 2 == 1) {
                if (resw % 2 == 1 && rese % 2 == 1) {
                    only_pipe[y][x] = 'O';
                    res++;
                }
            }
            printf("%c", only_pipe[y][x]);
        }
        printf("\n");
    }
#endif


    print("%d", res);
}
