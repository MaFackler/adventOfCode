#include "advent.h"


bool check_column(int *data, int x_dim, int y_dim, int x, int y) {
    bool visible = true;
    int value_self = data[y * x_dim + x];

    // check downwards
    for (int iy = y + 1; iy < y_dim; ++iy) {
        if (iy == y)
            continue;
        int value_other = data[iy * x_dim + x];
        if (value_self <= value_other) {
            visible = false;
        }
    }

    if (visible)
        return visible;

    visible = true;
    // check upwards
    for (int iy = y - 1; iy >= 0; --iy) {
        if (iy == y)
            continue;
        int value_other = data[iy * x_dim + x];
        if (value_self <= value_other) {
            visible = false;
        }
    }

    return visible;

}

bool check_row(int *data, int x_dim, int y_dim, int x, int y) {
    bool visible = true;
    int value_self = data[y * x_dim + x];

    // check to the right
    for (int ix = x + 1; ix < x_dim; ++ix) {
        if (ix == x)
            continue;
        int value_other = data[y * x_dim + ix];
        if (value_self <= value_other) {
            visible = false;
        }
    }

    if (visible)
        return visible;

    visible = true;
    // check to the left
    for (int ix = x - 1; ix >= 0; --ix) {
        if (ix == x)
            continue;
        int value_other = data[y * x_dim + ix];
        if (value_self <= value_other) {
            visible = false;
        }
    }

    return visible;
}

bool check(int *data, int x_dim, int y_dim, int x, int y) {
    int visible_row = check_row(data, x_dim, y_dim, x, y);
    int visible_column = check_column(data, x_dim, y_dim, x, y);
    return visible_row || visible_column;
}

int score_up(int *data, int x_dim, int y_dim, int x, int y) {
    int value_self = data[y * x_dim + x];
    int res = 0;
    for (int iy = y - 1; iy >= 0; --iy) {
        int value_other = data[iy * x_dim + x];
        res++;
        if (value_self <= value_other)
            break;
    }
    return res;
}

int score_down(int *data, int x_dim, int y_dim, int x, int y) {
    int value_self = data[y * x_dim + x];
    int res = 0;
    for (int iy = y + 1; iy < y_dim; ++iy) {
        int value_other = data[iy * x_dim + x];
        res++;
        if (value_self <= value_other)
            break;
    }
    return res;
}

int score_right(int *data, int x_dim, int y_dim, int x, int y) {
    int value_self = data[y * x_dim + x];
    int res = 0;
    for (int ix = x + 1; ix < x_dim; ++ix) {
        int value_other = data[y * x_dim + ix];
        res++;
        if (value_self <= value_other)
            break;
    }
    return res;
}

int score_left(int *data, int x_dim, int y_dim, int x, int y) {
    int value_self = data[y * x_dim + x];
    int res = 0;
    for (int ix = x - 1; ix >= 0; --ix) {
        int value_other = data[y * x_dim + ix];
        res++;
        if (value_self <= value_other)
            break;
    }
    return res;
}

int score(int *data, int x_dim, int y_dim, int x, int y) {
    int res = 1;
    res *= score_up(data, x_dim, y_dim, x, y);
    res *= score_left(data, x_dim, y_dim, x, y);
    res *= score_right(data, x_dim, y_dim, x, y);
    res *= score_down(data, x_dim, y_dim, x, y);
    return res;
}

int main() {
    char *line = NULL;
    int x_dim = 0;
    int y_dim = 0;
    int *data = NULL;

    while (readline(&line)) {
        x_dim = strlen(line) - 1;
        for (int i = 0; i < x_dim; ++i) {
            vec_push(data, line[i] - '0');
        }
        y_dim++;
    }

    const int offset = 1;
    int res = 0;
    int res2 = 0;
    for (int y = offset; y < y_dim - offset; ++y) {
        for (int x = offset; x < x_dim - offset; ++x) {
            //printf("%d", data[y * x_dim + x]);
            if (check(data, x_dim, y_dim, x, y)) {
                res++;
            }
            int score_value = score(data, x_dim, y_dim, x, y);
            res2 = MAX(score_value, res2);

        }
        //printf("\n");
    }
    //printf("check %d\n", score(data, x_dim, y_dim, 2, 3));

    //printf("%dx%d\n", x_dim, y_dim);
    res += x_dim *2;
    res += (y_dim - 2) * 2;
    printf("%d\n", res2);
}
