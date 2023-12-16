#include "advent.h"

i64 map_score(char *map, int dim) {
    i64 res = 0;
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            if (map[y * dim + x] == 'O') {
                res += dim - y;
            }
        }
    }
    return res;
}

void map_tilt(char *map, int dim) {
    for (int x = 0; x < dim; ++x) {
        for (int y = 0; y < dim - 1;) {
            char *a = &map[y * dim + x];
            char *b = &map[(y + 1) * dim + x];
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
}

void map_rotate(char *map, int dim, char *dest) {
    for (int x = 0; x < dim; ++x) {
        for (int y = 0; y < dim; ++y) {
            dest[x * dim + (dim - y - 1)] = map[y * dim + x];
        }
    }
}

void map_print(char *map, int dim) {
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            printf("%c", map[y * dim + x]);
        }
        printf("\n");
    }
}


int main() {
    char *line = NULL;
    char *map = NULL;
    int dx = 0, dy = 0;
    while (readline(&line)) {
        // print("%s", line);
        for (int i = 0; i < strlen(line); ++i) {
            append(map, line[i]);
        }
        dy++;
        dx = MAX(dx, strlen(line));
    }
    assert (dx == dy);
    int dim = dx;

    char *west = malloc(sizeof(char) * dim * dim);
    char *south = malloc(sizeof(char) * dim * dim);
    char *east = malloc(sizeof(char) * dim * dim);
    char *north = malloc(sizeof(char) * dim * dim);

    size_t *hashes = NULL;
    i64 cycle_start = 0;
    i64 cycle_end = 0;
    size_t *scores = NULL;

    for (int i = 0; i < 200; ++i) {
        // map_print(map, dim);
        // print("");
        map_tilt(map, dim);
        map_rotate(map, dim, west);
        
        map_tilt(west, dim);
        map_rotate(west, dim, south);

        map_tilt(south, dim);
        map_rotate(south, dim, east);

        map_tilt(east, dim);
        map_rotate(east, dim, north);

        memcpy(map, north, dim * dim);
        size_t score = map_score(map, dim);
        append(scores, score);
        size_t hash = stbds_hash_bytes(map, dim * dim, 0);
        // print("%ld %lu %lu", cycle_end, hash, score);
        for (int j = 0; j < len(hashes); ++j) {
            if (hashes[j] == hash) {
                cycle_start = j;
                goto finish;
            }
        }
        append(hashes, hash);

        // map_print(west, dim);
        //map_tilt(map, dim);
        cycle_end++;
    }

finish:

    i64 loops = 1000000000 - 1;
    i64 res = (loops - cycle_start) % (cycle_end - cycle_start) + cycle_start;

    print("%lu", scores[res]);
}
