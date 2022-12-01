#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

int bits_to_int(const char *string, size_t n) {
    int factor = 0x01;
    int res = 0;
    for (size_t i = 0; i < n; ++i) { 
        char c = string[n - i - 1];
        if (c == '1') {
            res += 1 * factor;
        }
        factor = factor << 1;
    }
    return res;
}

char *parse_data(const char *filename, size_t *amount_bits, size_t *amount_lines) {
    FILE *fp = fopen(filename, "rb");    
    if (!fp)
        perror("Cannor open input file");
    char line[256];
    fseek(fp, 0, SEEK_END);
    size_t filesize = ftell(fp);
    rewind(fp);
    fgets(&line[0], 256, fp);
    size_t line_len = strlen(line);
    assert(line_len > 0);
    size_t lines = filesize / line_len;
    *amount_lines = lines;
    *amount_bits = line_len - 1;
    char *res = malloc(lines * (*amount_bits));  // NOTE: ignore newline
    char *dest = res;
    rewind(fp);
    while (fgets(&line[0], 256, fp) != NULL) {
        memcpy(dest, &line[0], *amount_bits);
        dest += *amount_bits;
    }
    return res;
}

bool bit_criteria_oxygen(size_t one_values, size_t amount_lines, char current_value) {
    size_t zero_values = amount_lines - one_values;
    char common_value = one_values > zero_values ? '1' : '0';
    if (one_values == zero_values) {
        return current_value == '1';
    } else {
        return common_value == current_value;
    }
}

bool bit_criteria_scrubber(size_t one_values, size_t amount_lines, char current_value) {
    assert(one_values <= amount_lines);
    size_t zero_values = amount_lines - one_values;
    char least_common = one_values < zero_values ? '1' : '0';
    if (one_values == zero_values) {
        return current_value == '0';
    } else {
        return least_common == current_value;
    }
}

void calculate_one_values(char *data, size_t amount_bits, size_t amount_lines, unsigned int *values) {
    for (size_t i = 0; i < amount_bits; ++i) {
        values[i] = 0;
        for (size_t j = 0; j < amount_lines; ++j) {
            char c = data[j * amount_bits + i];
            assert(c == '1' || c == '0');
            if (c == '1') {
                values[i]++;
            }
        }
    }
}

int calculate_one_values_by_bitindex_filtered(char *data, size_t amount_bits, size_t bit_index, size_t amount_lines, bool *disabled) {
    int res = 0;
    for (size_t j = 0; j < amount_lines; ++j) {
        char c = data[j * amount_bits + bit_index];
        assert(c == '1' || c == '0');
        if (!disabled[j] && c == '1') {
            res++;
        }
    }
    return res;
}

size_t calculate_threshold(size_t amount_lines) {
    size_t res = amount_lines / 2;
    return res;
}

void print_data_line(char *data, size_t line_index, size_t amount_bits) {
    printf("%.*s\n", (int) amount_bits, &data[line_index * amount_bits]);
}



#define BIT_VALUE(i, amount_bits) (0x01 << (amount_bits - i - 1))

int main() {
    size_t amount_bits = 0;
    size_t amount_lines = 0;
    char *data = parse_data("data/input_day03.txt", &amount_bits, &amount_lines);
    unsigned int *values = calloc(amount_bits, sizeof(int));
    calculate_one_values(data, amount_bits, amount_lines, values);


    unsigned int gamma = 0;
    for (size_t i = 0; i < amount_bits; ++i) {
        size_t threshold = calculate_threshold(amount_lines);
        if (values[i] > threshold) {
            gamma += BIT_VALUE(i, amount_bits);
        }
    }
    unsigned int epsilon = ~(gamma) & ((0x01 << amount_bits) - 1);

    printf("Gamma is %d\n", gamma);
    printf("Epsilon is %d\n", epsilon);
    printf("Result is %d\n", gamma * epsilon);


    // NOTE: boolean list to determine disabled rows
    bool *oxygen = calloc(amount_lines, sizeof(bool));
    size_t oxygens_left = amount_lines;
    bool *scrubber = calloc(amount_lines, sizeof(bool));
    size_t scrubbers_left = amount_lines;
    for (size_t i = 0; i < amount_bits; ++i) {
        int oxygen_ones = calculate_one_values_by_bitindex_filtered(data, amount_bits, i, amount_lines, oxygen);
        int scrubber_ones = calculate_one_values_by_bitindex_filtered(data, amount_bits, i, amount_lines, scrubber);
        size_t oxygen_deleted = 0;
        size_t scrubber_deleted = 0;
        for (size_t j = 0; j < amount_lines; ++j) {
            char c = data[j * amount_bits + i];
            if (!oxygen[j] && (oxygens_left - oxygen_deleted) > 1 && !bit_criteria_oxygen(oxygen_ones, oxygens_left, c)) {
                oxygen[j] = true;
                oxygen_deleted++;
            }
            if (!scrubber[j] && (scrubbers_left - scrubber_deleted) > 1 && !bit_criteria_scrubber(scrubber_ones, scrubbers_left, c)) {
                scrubber[j] = true;
                scrubber_deleted++;
            }
        }
        oxygens_left -= oxygen_deleted;
        scrubbers_left -= scrubber_deleted;
    }

    int oxygen_index = -1;
    int scrubber_index = -1;
    for (size_t j = 0; j < amount_lines; ++j) {
        if (!oxygen[j]) {
            printf("Oxygen %.*s\n", (int) amount_bits, &data[j * amount_bits]);
            assert(oxygen_index == -1);
            oxygen_index = j;
        }
        if (!scrubber[j]) {
            printf("Scrubber %.*s\n", (int) amount_bits, &data[j * amount_bits]);
            assert(scrubber_index == -1);
            scrubber_index = j;
        }
    }
    assert(oxygen_index > 0);
    assert(scrubber_index > 0);
    int oxygen_value = bits_to_int(&data[oxygen_index * amount_bits], amount_bits);
    int scrubber_value = bits_to_int(&data[scrubber_index * amount_bits], amount_bits);
    printf("Oxygen value is %d\n", oxygen_value);
    printf("Scrubber value is %d\n", scrubber_value);
    printf("Result is %d\n", oxygen_value * scrubber_value);

    free(data);
    return 0;
}
