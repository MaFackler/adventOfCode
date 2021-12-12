#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "common.h"



#define GET_2D_INDEX(x, y, amount_numbers) (y * amount_numbers + x)

static int x_offset[4] = {-1, 1, 0, 0};
static int y_offset[4] = {0, 0, -1, 1};

int *calculate_basins(int *numbers, int x, int y, int amount_numbers, int amount_lines, int *visited, int *res) {
    int value = numbers[GET_2D_INDEX(x, y, amount_numbers)];
    for (size_t i = 0; i < ARRAY_LENGTH(x_offset); ++i) {
        int nx = x + x_offset[i];
        int ny = y + y_offset[i];
        assert(value != 9);
        if (nx >= 0 && nx < amount_numbers && ny >= 0 && ny < amount_lines) {
            int nindex = GET_2D_INDEX(nx, ny, amount_numbers);
            int neighbour_value = numbers[nindex];
            if (neighbour_value != 9 && neighbour_value > value) {
                bool is_visited = false;
                for (size_t j = 0; j < vec_size(visited); ++j) {
                    if (visited[j] == nindex) {
                        is_visited = true;
                    }
                }
                if (!is_visited) {
                    vec_push(visited, nindex);
                    *res = (*res) + 1;
                }
                visited = calculate_basins(numbers, nx, ny, amount_numbers, amount_lines, visited, res);
            }
        }
    }
    return visited;
}

int sort_int_desc(const void *a, const void *b) {
    int *aa = (int *) a;
    int *bb = (int *) b;
    return *bb - *aa;
}

int main() {
	FILE *fp = fopen("data/input_day09.txt", "rb");
	if (!fp)
        perror("Cannot open file");

    int *numbers = NULL;
    int *risk_levels = NULL;
    size_t amount_lines = 0;
    size_t amount_numbers = 0;
    bool first_line = true;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (isdigit(c)) {
            int num = c - '0';
            vec_push(numbers, num);
            if (first_line)
                amount_numbers++;
        } else {
            assert(c == '\n');
            first_line = false;
            amount_lines++;
        }
    }
    printf("amount lines %lu\n", amount_lines);

    int *visited = NULL;
    int *basins = NULL;
    for (size_t y = 0; y < amount_lines; ++y) {
        for (size_t x = 0; x < amount_numbers; ++x) {
            bool is_lower = true;
            int value = numbers[GET_2D_INDEX(x, y, amount_numbers)];
            for (size_t i = 0; i < ARRAY_LENGTH(x_offset); ++i) {
                int nx = x + x_offset[i];
                int ny = y + y_offset[i];
                if (nx >= 0 && nx < amount_numbers && ny >= 0 && ny < amount_lines) {
                    int neighbour_value = numbers[GET_2D_INDEX(nx, ny, amount_numbers)];
                    is_lower &= value < neighbour_value;
                }
            }
            if (is_lower) {
                vec_push(risk_levels, value);
                int basin = 1;
                visited = calculate_basins(numbers, x, y, amount_numbers, amount_lines, visited, &basin);
                vec_push(basins, basin);
                printf("got basin %d\n", basin);
            }
        }
    }
    int sum_risk = 0;
    for (size_t i = 0; i < vec_size(risk_levels); ++i) {
        sum_risk += (risk_levels[i] + 1);
    }

    qsort(basins, vec_size(basins), sizeof(int), sort_int_desc);
    printf("Result is %d\n", sum_risk);

    assert(vec_size(basins) >= 3);
    printf("%d\n", basins[0]);
    printf("%d\n", basins[1]);
    printf("%d\n", basins[2]);
    printf("Result is %d\n", basins[0] * basins[1] * basins[2]);

    fclose(fp);
}
