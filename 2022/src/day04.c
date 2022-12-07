#include "advent.h"

int main() {
    char *line = NULL;
    int res = 0;
    int res2 = 0;

    while (readline(&line)) {
        int amin, amax, bmin, bmax;
        sscanf(line, "%d-%d,%d-%d", &amin, &amax, &bmin, &bmax);
        if ((amin >= bmin && amax <= bmax)
            || (bmin >= amin && bmax <= amax)) {
            //printf("%s", line);
            res++;
        }

        for (int i = amin; i <= amax; ++i) {
            if (i >= bmin && i <= bmax) {
                res2++;
                break;
            }
        }
    }
    printf("%d\n", res2);
}
