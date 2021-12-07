#include <stdio.h>
#include "common.h"


int calculate_fuel(int *values, size_t n, size_t fuel_value) {
    int res = 0;
    for (size_t i = 0; i < n; ++i) {
        res += abs(values[i] - fuel_value);
    }
    return res;
}

int main() {

    FILE *fp = fopen("data/input_day07.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    int *values = NULL;
    int value;
    int sum_values = 0;
    int max_value = 0;
    while (fscanf(fp, "%d", &value) != EOF) {
        vec_push(values, value);
        sum_values += value;
        if (value > max_value) {
            max_value = value;
        }
        char c = fgetc(fp);
        if (c != ',') {
            break;
        }
    }
    fclose(fp);

    int occurances[2048] = {0};

    printf("sum of values is %d\n", sum_values);
    int average = sum_values / vec_size(values);
    printf("average is %d\n", average);
    for (size_t i = 0; i < vec_size(values); ++i) {
        occurances[values[i]]++;
    }


    int lowest_fuel_index = -1;
    int lowest_fuel_value = -1;
    for (size_t i = 0; i <= max_value; ++i) {
        int res = calculate_fuel(values, vec_size(values), i);
        printf("got fuel value of index %lu = %d\n", i, res);
        if (res < lowest_fuel_value || lowest_fuel_value == -1) {
            lowest_fuel_value = res;
            lowest_fuel_index = i;
        }
    }

    printf("Lowest fuel index is %d\n", lowest_fuel_index);
    printf("Lowest fuel value is %d\n", lowest_fuel_value);



}
