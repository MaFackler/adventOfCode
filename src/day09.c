#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "common.h"



#define GET_2D_INDEX(x, y, amount_numbers) (y * amount_numbers + x)

void get_2d_index(int i, int *x, int *y, int amount_numbers) {
    *x = i % amount_numbers;
    *y = i / amount_numbers;
}

bool in_range(int index, int upper) {
    return (index >= 0 && index < upper);
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

    int x, y;
    for (size_t i = 0; i < vec_size(numbers); ++i) {
        get_2d_index(i, &x, &y, amount_numbers);
        int uy = y - 1;
        int dy = y + 1;
        int lx = x - 1;
        int rx = x + 1;
        bool is_lower = true;
        int value = numbers[GET_2D_INDEX(x, y, amount_numbers)];
        if (in_range(uy, amount_lines)) {
            is_lower &= value < numbers[GET_2D_INDEX(x, uy, amount_numbers)];
        }
        if (in_range(dy, amount_lines)) {
            is_lower &= value < numbers[GET_2D_INDEX(x, dy, amount_numbers)];
        }
        if (in_range(lx, amount_numbers)) {
            is_lower &= value < numbers[GET_2D_INDEX(lx, y, amount_numbers)];
        }
        if (in_range(rx, amount_numbers)) {
            is_lower &= value < numbers[GET_2D_INDEX(rx, y, amount_numbers)];
        }

        if (is_lower) {
            //printf("x is %d; y is %x\n", x, y);
            //printf("value is %d\n", numbers[GET_2D_INDEX(x, y)]);
            vec_push(risk_levels, value);
        }

    }

    int sum_risk = 0;
    for (size_t i = 0; i < vec_size(risk_levels); ++i) {
        sum_risk += (risk_levels[i] + 1);
    }
    printf("Result is %d\n", sum_risk);

    fclose(fp);
}
