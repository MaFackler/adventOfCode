#include "advent.h"
#include "math.h"

int calc(int t, int x) {
    return (t - x) * x;
}

int main() {
    char *line = NULL;
    
    char *tmp = NULL;
    readline(&line);
    char *numbers = buffer(1024);
    sscanf(line, "Time:%[^\n]", numbers);

    int *times = NULL;
    for strsplit(tok, numbers, " ") {
        append(times, atoi(tok));
        for (int i = 0; i < strlen(tok); ++i) {
            append(tmp, tok[i]);
        }
    }

    int64_t ttime = strtoll(tmp, NULL, 10);

    vec_clear(tmp);

    readline(&line);
    int *distances = NULL;
    sscanf(line, "Distance:%[^\n]", numbers);
    for strsplit(tok, numbers, " ") {
        append(distances, atoi(tok));
        for (int i = 0; i < strlen(tok); ++i) {
            append(tmp, tok[i]);
        }
    }

    int64_t tdistance = strtoll(tmp, NULL, 10);

    // print("%ld %ld", ttime, tdistance);
    // y = - x^2 + 7x - 9 
    double a = -1;
    double b = ttime;
    double c = -tdistance;

    double deter = b * b - 4 * a * c;
    assert (deter > 0);
    deter = sqrt(deter);
    int64_t q1 = ceil((-b + deter) / (2.0 * a));
    int64_t q2 = floor((-b - deter) / (2.0 * a));
    q1--;

    assert (q1 < q2);
    int64_t res = q2 - q1;

#if 0
    res = 1;
    for (int i = 0; i < len(distances); ++i) {
        int time = times[i];
        int distance = distances[i];
        int amount = 0;
        for (int x = 0; x <= time; ++x) {
            int r = calc(time, x);
            if (r > distance) {
                amount++;
            }
        }
        res *= amount;

    }
#endif

    print("%ld", res);
}
