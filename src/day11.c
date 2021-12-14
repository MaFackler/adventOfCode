#include <stdio.h>
#include <stdbool.h>
#include "common.h"

#define DIM 5


void data_flash(int *data, size_t dim, size_t index) {
	int x = index % dim;
	int y = index / dim;
    for (int iy = -1; iy <= 1; ++iy) {
        for (int ix = -1; ix <= 1; ++ix) {
			int xx = x + ix;
			int yy = y + iy;
			
            if (xx >= 0 && xx < dim &&
                yy >= 0 && yy < dim) {
                int *value = &data[(y + iy) * dim + (x + ix)];
                if (iy == 0 && ix == 0) {
                    *value = -1;
                } else if (*value != -1) {
                    *value = (*value) + 1;
                }
            }
        }
    }
}

int* data_get_flash_list(int *data, size_t dim, int *dest) {
    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            int index = y * dim + x;
            int value = data[index];
            if (value > 9) {
                vec_push(dest, index);
            }
        }
    }
    return dest;
}

int data_step(int *data, size_t dim) {
    int res = 0;
    // increase energy level
    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            data[y * dim + x]++;
        }
    }

    // Flash
    int *flash_list = NULL;
    flash_list = data_get_flash_list(data, dim, flash_list);
    while (vec_size(flash_list) > 0) {
        for (size_t i = 0; i < vec_size(flash_list); ++i) {
            int index = flash_list[i];
            data_flash(data, dim, index);
            res++;
        }
        vec_clear(flash_list);
        flash_list = data_get_flash_list(data, dim, flash_list);
    }
    
    // Set values to 0
    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            int *value = &data[y * dim + x];
            if (*value == -1) {
                *value = 0;
            }
        }
    }

    return res;
}

void data_print(int *data, size_t dim) {
    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            printf("%d", data[y * dim + x]);
        }
        printf("\n");
    }
}


bool data_check_all_flashed(int *data, size_t dim) {
    bool res = true;
    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            if (data[y * dim + x] != 0) {
                res = false;
                return res;
            }
        }
    }
    return res;
}

int main() {
    size_t file_size = 0;
    char *contents = file_read("data/input_day11.txt", &file_size);
    size_t dim = string_line_lenght(contents);

    int *data = (int *) malloc(dim * dim * sizeof(int));
    for (size_t y = 0; y < dim; ++y) {
        for (size_t x = 0; x < dim; ++x) {
            data[y * dim + x] = char_num_to_int(contents[(y * (dim + 1)) + x]);
        }
    }
    free(contents);

    int count = 0;
    int all_flashed_index = -1;
    data_print(data, dim);
    printf("\n");
    for (size_t i = 0; i >= 0; ++i) {
        printf("Step %lu\n", i + 1);
        count += data_step(data, dim);
        data_print(data, dim);
        printf("\n");
        bool all_flashed = data_check_all_flashed(data, dim);
        if (all_flashed) {
            all_flashed_index = i;
            break;
        }
    }

    printf("Result is %d\n", all_flashed_index + 1);

}
