#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "common.h"



int* day(int *population) {
    int *kids = NULL;
    for (size_t i = 0; i < vec_size(population); ++i) {
        int value = population[i];
        if (value - 1 < 0) {
            population[i] = 6;
            vec_push(kids, 8);
        } else {
            population[i]--;
        }
    }

    for (size_t i = 0; i < vec_size(kids); ++i) {
        vec_push(population, kids[i]);
    }

    vec_free(kids);
    return population;
}

void print_popuplation(int *population) {
    size_t size = vec_size(population);
    for (size_t i = 0; i < size; ++i) {
        printf("%d", population[i]);
        if (i < (size - 1)) {
            printf(",");
        }
    }
    printf("\n");
}


int main() {
    FILE *fp = fopen("data/input_day06.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    fscanf(fp, "Initial state: ");


    int *population = NULL;
    int value;
    while (fscanf(fp, "%d", &value) != EOF) {
        vec_push(population, value);
        char c = fgetc(fp);
        if (c != ',')
            break;
    }
    fclose(fp);

    printf("Initial state: ");
    print_popuplation(population);
    size_t days_to_simulate = 80;
    for (size_t i = 0; i < days_to_simulate; ++i) {
        //printf("After %lu day: ", i + 1);
        population = day(population);
        //print_popuplation(population);
    }
    printf("size of popuplation %lu\n", vec_size(population));
    vec_free(population);
}
