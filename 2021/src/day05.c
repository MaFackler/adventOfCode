#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


bool do_min_max(int *minn, int *maxx, int first_value, int second_value) {
    if (first_value >= second_value) {
        *minn = second_value;
        *maxx = first_value;
        return false;
    } else {
        *minn = first_value;
        *maxx = second_value;
        return true;
    }
}

int main() {
    FILE *fp = fopen("data/input_day05.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    int x1, x2, y1, y2;
    int x_max = 0;
    int y_max = 0;
    size_t amount_lines = 0;
    while(fscanf(fp, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF) {
        amount_lines++;
        if (x1 > x_max) {
            x_max = x1;
        }
        if (x2 > x_max) {
            x_max = x2;
        }
        if (y1 > y_max) {
            y_max = y1;
        }
        if (y2 > y_max) {
            y_max = y2;
        }
    }
    rewind(fp);

    printf("x_max is %d\ny_max is %d\n", x_max, y_max);
    int *data = (int *) calloc((x_max + 1) * (y_max + 1), sizeof(int));
    while(fscanf(fp, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF) {
        int start_x, end_x, start_y, end_y;
        if (x1 == x2) {
            do_min_max(&start_y, &end_y, y1, y2);
            for (int y = start_y; y <= end_y; ++y) {
                data[(y * (x_max + 1)) + x1]++;
            }
        }
        else if (y1 == y2) {
            do_min_max(&start_x, &end_x, x1, x2);
            for (int x = start_x; x <= end_x; ++x) {
                data[(y1 * (x_max + 1)) + x]++;
            }
        } else {
            assert(abs(x1 - x2) == abs(y1 - y2));
            int x_advance = 1;
            int y_advance = x_max + 1;
            if (x2 < x1) {
                x_advance = -x_advance;
            }
            if (y2 < y1) {
                y_advance = -y_advance;
            }
            int *ptr = &data[y1 * (x_max + 1) + x1];
            for (int i = 0; i <= abs(x2 - x1); ++i) {
                *ptr = (*ptr) + 1;
                ptr += x_advance;
                ptr += y_advance;
            }
        }
        
    }
    printf("amount lines %lu\n", amount_lines);

    size_t counter = 0;
    for (int y = 0; y <= y_max; ++y) {
        for (int x = 0; x <= x_max; ++x) {
            int value = data[y * (x_max + 1) + x];
            if (value == 0) {
                //printf(".");
            } else {
                //printf("%d", value);
            }
            if (value >= 2) {
                counter++;
            }
        }
        //printf("\n");
    }
    printf("Result ist %lu\n", counter);



}
