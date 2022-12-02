#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector.h>

int cmp_int(const void *a, const void *b) {
    return *(int *)a < *(int*) b;
}

int main() {
    ssize_t n = 0; 
    size_t allocated = 0;
    char *line = NULL;
    int sum = 0;
    int *values = NULL;

    while ((n = getline(&line, &allocated, stdin)) != -1) {
        if (strcmp(line, "\n") == 0) {
            vec_push(values, sum);
            sum = 0;
        } else {
            sum += strtol(line, NULL, 10);
        }
    }
    vec_push(values, sum);
    qsort(values, vec_size(values), sizeof(int), cmp_int);
    printf("%d\n", values[0] + values[1] + values[2]);
}
