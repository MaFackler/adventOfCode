#include "advent.h"
#include "stdint.h"

typedef struct SeedMap {
    uint64_t dest;
    uint64_t src;
    uint64_t range;
} SeedMap;

bool map_check(SeedMap *map, uint64_t value) {
    return value >= map->src && value < map->src + map->range;
}

uint64_t map_get_value(SeedMap *map, uint64_t value) {
    uint64_t res = 0;
    if (map->dest > map->src) {
        int64_t d = map->dest - map->src;
        res = value + d;
    } else {
        int64_t d = map->src - map->dest;
        assert (d > 0);
        res = value - d;
    }
    return res;
}

int main() {
    char *line = NULL;
    readline(&line);

    char *right = buffer(1024);
    sscanf(line, "seeds:%[^\n]", right);
    uint64_t *numbers = NULL;
    for strsplit(nrtok, right, " ") {
        append(numbers, strtoull(nrtok, NULL, 10));
    }

    assert (UINT64_MAX == __UINT64_C(18446744073709551615));
    uint64_t res = UINT64_MAX;
    SeedMap* maps[10] = {0};
    int seed_index = -1;
    while (readline(&line)) {
        if (strlen(line)) {
            if (isalpha(line[0])) {
                seed_index++;
            } else {
                SeedMap map = {0};
                sscanf(line, "%lu %lu %lu", &map.dest, &map.src, &map.range);
                append(maps[seed_index], map);
            }
        }
    }


    for (int k = 0; k < len(numbers); k+=2) { 

        uint64_t start = numbers[k];
        uint64_t end = numbers[k + 1];
        for (uint64_t index = start; index < start + end; ++index) {
            uint64_t value = index;
            for (int i = 0; i <= seed_index; ++i) {
                assert(i <= 7);
                for (int j = 0; j < len(maps[i]); ++j) {
                    SeedMap *map = &maps[i][j];
                    if (map_check(map, value)) {
                        value = map_get_value(map, value);
                        break;
                    }
                }
            }
            res = MIN(res, value);
        }

    }
    print("%lu", res);
}
