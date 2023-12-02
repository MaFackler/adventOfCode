#ifndef ADVENT_H
#define ADVENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#define append vec_push
#define len vec_size

#define print(fmt, ...) printf(fmt "\n", ## __VA_ARGS__)

#define ArraySize(arr) (sizeof(arr) / sizeof(arr[0]))
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define irange(var, __i) (int var = 0; var < __i; ++var)

bool readline(char **line) {
    ssize_t n = 0; 
    size_t allocated = 0;

    n = getline(line, &allocated, stdin);
    if (n >= 0) {
        (*line)[n - 1] = 0;
    }
    return n != -1;
}

int ctoi(char c) {
    assert (c >= '0' && c <= '9');
    return c - '0';
}

char itoc(int i) {
    assert (i >= 0 && i <= 9);
    return i + '0';
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
