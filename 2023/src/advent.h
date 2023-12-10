#ifndef ADVENT_H
#define ADVENT_H

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define append vec_push
#define add vec_add
#define len vec_size
#define pop(arr) &arr[--vec_header(arr)->size]
typedef int64_t i64;
typedef uint64_t u64;

#define print(fmt, ...) printf(fmt "\n", ## __VA_ARGS__)

#define ArraySize(arr) (sizeof(arr) / sizeof(arr[0]))
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define irange(var, __i) (int var = 0; var < __i; ++var)
#define strsplit(var, s, delim) (char *var##_rest = NULL, *var = strtok_r((s), (delim), &var##_rest); var != NULL; var = strtok_r(NULL, (delim), &var##_rest))
#define each(var, collection) (__auto_type var = &collection[0]; var != NULL && var != &collection[len(collection)]; var++)
#define buffer(n) (char *) alloca(n)
#define list(T) T*
#define sort(collection, func) qsort(collection, len(collection), sizeof(*collection), (int (*)(const void*, const void*)) func)

#define printl(collection, fmt) \
    printf("("); \
    for each(__ele, collection) { \
        printf(fmt ", ", *__ele); \
    } \
    printf(")\n");


#define print_ints(collection) printl(collection, "%d")

void* findt(void *collection, size_t stride, size_t n, bool func(void *it)) {
    for (int i = 0; i < n; ++i) {
        char *it = (char *) collection + stride * i;
        if (func(it)) {
            return it;
        }
    }
    return NULL;
}

#define find(collection, func) findt(collection, sizeof(*collection), len(collection), (bool (*)(void *)) func)

#define streql(a, b) (strcmp((a), (b)) == 0)
#define INVALID assert(false);

char* strlstrip(char *s, char delim) {
    while (*s++ == delim) {}
    return s - 1;
}

bool readline(char **line) {
    size_t n = 0; 
    size_t allocated = 0;

    n = getline(line, &allocated, stdin);
    if (n >= 0) {
        (*line)[n - 1] = 0;
    }
    return n != -1;
}

bool readlinefp(char **line, FILE *fp) {
    size_t n = 0; 
    size_t allocated = 0;

    n = getline(line, &allocated, fp);
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

int64_t gcd(int64_t a, int64_t b) {
    if (a == 0)
        return abs(b);
    if (b == 0)
        return abs(a);

    int64_t rest = 0;
    do {
        rest = a % b;
        a = b;
        b = rest;
    } while (b != 0);
    return abs(a);
}

int64_t lcm(int64_t a, int64_t b) {
    return a * b / gcd(a, b);
}

#endif // ADVENT_H
