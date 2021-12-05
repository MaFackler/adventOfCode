#include <stdlib.h>
#include <stdio.h>

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
        if (x1 == x2) {
            int ystart = y1;
            int yend = y2;
            if (ystart > yend) {
                ystart = y2;
                yend = y1;
            }
            for (int y = ystart; y <= yend; ++y) {
                data[(y * (x_max + 1)) + x1]++;
            }
        }
        if (y1 == y2) {
            int xstart = x1;
            int xend = x2;
            if (xstart > xend) {
                xstart = x2;
                xend = x1;
            }
            for (int x = xstart; x <= xend; ++x) {
                data[(y1 * (x_max + 1)) + x]++;
            }
        }
    }
    printf("amount lines %lu\n", amount_lines);

    size_t counter = 0;
    for (int y = 0; y <= y_max; ++y) {
        for (int x = 0; x <= x_max; ++x) {
            int value = data[y * (x_max + 1) + x];
            //printf("%d", value);
            if (value >= 2) {
                counter++;
            }
        }
        //printf("\n");
    }
    printf("Result ist %lu\n", counter);



}
