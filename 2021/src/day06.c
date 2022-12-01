#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"


#define DAYS_RANGE 9

void day(long *days_population, size_t n) {
    long days_to_add[DAYS_RANGE] = {0};
    for (size_t i = 0; i < n; ++i) {
        long amount = days_population[i];
        if (i == 0) {
            days_to_add[8] += amount;
            days_to_add[6] += amount;
            days_to_add[0] -= amount;
        } else {
            days_to_add[i - 1] += amount;
            days_to_add[i] -= amount;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        days_population[i] += days_to_add[i];
    }
}



int main() {
    FILE *fp = fopen("data/input_day06.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    fscanf(fp, "Initial state: ");


    long *population = NULL;
    long value;
    while (fscanf(fp, "%ld", &value) != EOF) {
        vec_push(population, value);
        char c = fgetc(fp);
        if (c != ',')
            break;
    }
    fclose(fp);

    long days_population[DAYS_RANGE] = {0};
    for (size_t i = 0; i < vec_size(population); ++i) {
        long days_left = population[i];
        days_population[days_left]++;
    }

    for (size_t i = 0; i < 256; ++i) {
        day(&days_population[0], DAYS_RANGE);
    }

    long res = 0;
    for (size_t i = 0; i < DAYS_RANGE; ++i) {
        res += days_population[i];
    }
    printf("sum is %ld\n", res);

    

    vec_free(population);
}
