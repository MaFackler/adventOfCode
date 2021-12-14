#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

// Simple vector implementation
#define DEFAULT_VEC_SIZE 256

// NOTE: char is *nomally* one byte so its convenient to use
typedef char vec_memory;
typedef struct {
    size_t size;
    size_t capacity;
} vec_header;

#define vec_push(data, value) \
    vec__check_and_grow((vec_memory **) &data); \
    data[vec__get_header((vec_memory *) data)->size++] = value

//vec_header *__header = vec__get_header((vec_memory *)data);
//data[__header->size++] = value;

#define vec_size(data) \
    (data == NULL ? 0 : vec__get_header((vec_memory *) data)->size)

#define vec_free(data) \
    if (data != NULL) free(vec__get_header((vec_memory *) data))

#define vec_clear(data) \
    if (data != NULL) vec__get_header((vec_memory *) data)->size = 0; \
    data = NULL



vec_header* vec__get_header(vec_memory *state) {
    vec_header *res = (vec_header *) ((char *) state - sizeof(vec_header));
    return res;
}

void vec__allocate(vec_memory **data, size_t n_elements) {
    size_t size_to_alloc = n_elements * sizeof(vec_memory) * sizeof(vec_header);
    void *mem = calloc(size_to_alloc, 1);
    *data = (vec_memory *) (mem + sizeof(vec_header));
}

void vec__reallocate(vec_memory **data, size_t n_elements) {
    size_t size_to_alloc = n_elements * sizeof(vec_memory) * sizeof(vec_header);
    assert(*data != NULL);
    vec_header *header = vec__get_header(*data);
    header = realloc(header, size_to_alloc);
    *data = (vec_memory *) (header + 1);
    header->capacity = n_elements;
}

void vec__check_and_grow(vec_memory **data) {
    if (*data == NULL) {
        // Empty vector
        vec__allocate(data, DEFAULT_VEC_SIZE);
        // NOTE: normally not needed because auf calloc
        vec_header *header = vec__get_header(*data);
        header->size = 0;
        header->capacity = DEFAULT_VEC_SIZE;
    } else {
        // Grow vector
        vec_header *header = vec__get_header(*data);
        if (header->size >= header->capacity) {
            vec__reallocate(data, header->capacity * 2);
        }
    }
}


int vec_sort_int_asc(const void *a, const void *b) {
    int *aa = (int*) a;
    int *bb = (int*) b;
    return *aa - *bb;
}

int vec_sort_unsigned_int_asc(const void *a, const void *b) {
    unsigned int *aa = (unsigned int*) a;
    unsigned int *bb = (unsigned int*) b;
    if (*aa > *bb)
        return 1;
    else if (*aa == *bb)
        return 0;
    else
        return -1;
}

int vec_sort_unsigned_long_long_int(const void *a, const void *b) {
    unsigned long long int *aa = (unsigned long long int*) a;
    unsigned long long int *bb = (unsigned long long int*) b;
    if (*aa > *bb)
        return 1;
    else if (*aa == *bb)
        return 0;
    else
        return -1;
}

int vec_sort_long_int_asc(const void *a, const void *b) {
    long int *aa = (long int*) a;
    long int *bb = (long int*) b;
    return *aa - *bb;
}

int char_num_to_int(char c) {
    assert(isdigit(c));
    return c - '0';
}

size_t string_line_lenght(const char *buf) {
    size_t res = 0;
    while (*buf != 0 && *buf != '\n') {
        res++;
        buf++;
    }
    return res;
}

char *file_read(const char *filename, size_t *size) {
    FILE *fp = fopen(filename, "rb");
    assert(fp != NULL);

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *res = (char *) malloc(sizeof(char) * file_size);

    size_t amount_read = 0;
    while (amount_read < file_size) {
        size_t r = fread(res + amount_read, sizeof(char), file_size, fp);
        amount_read += r;
    }
    *size = file_size;
    fclose(fp);
    return res;
}

#endif // COMMON_H
