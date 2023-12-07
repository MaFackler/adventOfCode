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

typedef struct Range {
    uint64_t start;
    uint64_t end;
} Range;

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

    Range *ranges = NULL;
    for (int i = 0; i < len(numbers); i += 2) {
        *add(ranges) = (Range) {
            .start=numbers[i],
            .end=numbers[i] + numbers[i + 1]
        };
    }

    for (int i = 0; i <= seed_index; ++i) {
        Range *tmp_results = NULL;

        while (len(ranges) > 0) {
            Range *a = pop(ranges);

            bool found = false;
            for each(map, maps[i]) {
                Range b = {
                    .start=map->src,
                    .end=map->src + map->range
                };

                uint64_t intersection_start = MAX(a->start, b.start);
                uint64_t intersection_end = MIN(a->end, b.end);
                if (intersection_start < intersection_end) {
                    uint64_t d = map->dest - map->src;
                    // overlap
                    *add(tmp_results) = (Range) {intersection_start + d, intersection_end + d};
                    if (a->start < intersection_start) {
                        // left part
                        *add(ranges) = (Range) {a->start, intersection_start};
                    }

                    if (intersection_end < a->end) {
                        *add(ranges) = (Range) {intersection_end, a->end};
                    }
                    found = true;
                    break;
                }
            }

            if (!found) {
                *add(tmp_results) = *a;
            }
        }

        assert(len(ranges) == 0);
        for each(range, tmp_results) {
            append(ranges, *range);
        }
        vec_free(tmp_results);
    }

    for each(range, ranges) {
        res = MIN(range->start, res);
    }

#if 0
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
#endif
    print("%lu", res);
}
