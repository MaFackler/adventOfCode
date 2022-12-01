#include "common.h"

void data_fold_x(char *data, int row_advance, int x_max, int y_max, int x_fold) {
    for (int y = 0; y < y_max; ++y) {
        char *start = &data[y * row_advance + x_fold];
        char *left = start - 1;
        char *right = start + 1;
        for (int x = 0; x < x_fold; ++x) {
            if (*right == '#') {
                *left = '#';
            }
            left--;
            right++;
        }
    }
}

void data_fold_y(char *data, int row_advance, int x_max, int y_max, int y_fold) {
    char *start = &data[y_fold * row_advance];
    char *up = start - row_advance;
    char *down = start + row_advance;
    for (int y = 0; y < y_fold; ++y) {
        for (int x = 0; x < x_max; ++x) {
            if (down[x] == '#') {
                up[x] = '#';
            }
        }
        up -= row_advance;
        down += row_advance;
    }
    //for (int y = y_fold; y < y_max; ++y) {
    //    for (int x = 0; x < x_max; ++x) {
    //        char c = data[y * x_max + x];
    //        if (c == '#') {
    //            int dest_y = y_fold - y + y_fold;
    //            data[dest_y * row_advance + x] = '#';
    //        }
    //    }
    //}
}

void data_print(char *data, int row_advance, int x_max, int y_max) {
    printf("\n");
    for (int y = 0; y < y_max; ++y) {
        for (int x = 0; x < x_max; ++x) {
            char c = data[y * row_advance + x];
            if (c == 0) {
                c = '.';
            }
            printf("%c", c);
        }
        printf("\n");
    }
    printf("\n");
}

int data_count_dots(char *data, int row_advance, int x_max, int y_max) {
    int res = 0;
    for (int y = 0; y < y_max; ++y) {
        for (int x = 0; x < x_max; ++x) {
            if (data[y * row_advance + x] == '#') {
                res++;
            }
        }
    }
    return res;
}


int main() {
    FILE *fp = fopen("data/input_day13.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    int *xs = NULL;
    int *ys = NULL;

    int x_max = 0;
    int y_max = 0;
    int left, right;
    while(fscanf(fp, "%d,%d\n", &left, &right) != 0) {
        x_max = MAX(left, x_max);
        y_max = MAX(right, y_max);
        vec_push(xs, left);
        vec_push(ys, right);
    }
    assert(x_max > 0);
    assert(y_max > 0);
    x_max++;
    y_max++;

    int row_advance = x_max;
    char *data = (char*) malloc(sizeof(char) * x_max * y_max);
    memset(data, 0, x_max * y_max);

    for (int i = 0; i < vec_size(xs); ++i) {
        int x = xs[i];
        int y = ys[i];
        data[y * x_max + x] = '#';
    }
    //data_print(data, row_advance, x_max, y_max);


    char direction;
    int index;
    while (fscanf(fp, "fold along %c=%d\n", &direction, &index) != EOF) {
        if (direction == 'y') {
            data_fold_y(data, row_advance, x_max, y_max, index);
            y_max = index;
        } else {
            data_fold_x(data, row_advance, x_max, y_max, index);
            x_max = index;

        }
    }

    fclose(fp);

    data_print(data, row_advance, x_max, y_max);
    int res = data_count_dots(data, row_advance, x_max, y_max);
    printf("%d\n", res);

}
