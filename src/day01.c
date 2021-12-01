#include <stdio.h>
#include "input_day01.c"

void partOne() {
    int counter = 0;
    for (int i = 1; i < sizeof(input) / sizeof(input[0]); ++i) {
        if (input[i - 1] < input[i]) {
            counter++;
        }
    }
    printf("%d\n", counter);
}

void partTwo() {
    int counter = 0;
    int amount_windows = sizeof(input) / sizeof(input[0]) - 2;
    for (int i = 1; i < amount_windows; ++i) {
        int previous_sum = input[i - 1] + input[i] + input[i + 1];
        int measurement_sum = input[i] + input[i + 1] + input[i + 2];
        if (previous_sum < measurement_sum) {
            counter++;
        }
    }
    printf("%d\n", counter);
}

int main() {
    partOne();
    partTwo();
}
