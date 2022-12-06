#ifndef ADVENT_H
#define ADVENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector.h>
#include <stdbool.h>
#include <assert.h>

#define ArraySize(arr) (sizeof(arr) / sizeof(arr[0]))

bool readline(char **line) {
    ssize_t n = 0; 
    size_t allocated = 0;

    n = getline(line, &allocated, stdin);
    return n != -1;
}

bool str_equal(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

int str_to_int(const char *a) {
    return strtol(a, NULL, 10);
}

int cmp_int(const void *a, const void *b) {
    return *(int *)a < *(int*) b;
}

void memory_set_int(int *mem, int value, size_t n) {
    for (int i = 0; i < n; ++i) {
        mem[i] = value;
    }
}

#endif // ADVENT_H
