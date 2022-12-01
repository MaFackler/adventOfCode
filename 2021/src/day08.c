#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include "common.h"

//  aaaa 
// b    c
// b    c
//  dddd
// e    f
// e    f
//  gggg
//
// 0 -> abc efg   len(6)
// 1 ->   c  f    len(2)
// 2 -> a cde g   len(5)
// 3 -> a cd fg   len(5)
// 4 ->  bcd f    len(4)
// 5 -> ab d fg     len(5)
// 6 -> ab defg    len(6)
// 7 -> a c  f    len(3)
// 8 -> abcdefg   len(7)
// 9 -> abcd fg   len(6)
//
// a occus 8 times
// b occus 6 times
// c occus 8 times
// d occus 7 times
// e occus 4 times
// f occus 9 times
// g occus 7 times
//
// Conclusion
//
// simple
// len(2) -> 1
// len(3) -> 7
// len(4) -> 4
// len(7) -> 8
//
// complex
// len(5) -> 2,3,5
// len(6) -> 0,6,9


void swap_char(char *a, char *b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void permute(char *string, int l, int r, void cb(char *, void *), void *userdata) {
    if (l == (r - 1)) {
        cb(string, userdata);
    } else {
        for (size_t i = l; i < r; ++i) {
            swap_char(string + l, string + i);
            permute(string, l + 1, r, cb, userdata);
            swap_char(string + l, string + i);
        }
    }
}

void string_sort_letters(char *string) {
    size_t n = strlen(string);
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (string[i] > string[j]) {
                char temp = string[i];
                string[i] = string[j];
                string[j] = temp;
            }
        }
    }
}

void wire_word(char *wires, char *word, char *dest) {
    for (size_t i = 0; i < strlen(word); ++i) {
        dest[i] = wires[word[i] - 'a'];
    }
    dest[strlen(word)] = 0;
    string_sort_letters(dest);
}

typedef struct {
    char **words;
    char **words2;
    long sum;
} CbParameter;


int get_value_of_string(char *string) {
    int value = -1;
    if (strcmp(string, "abcefg") == 0) {
        value = 0;
    } else if (strcmp(string, "cf") == 0) {
        value = 1;
    } else if (strcmp(string, "acdeg") == 0) {
        value = 2;
    } else if (strcmp(string, "acdfg") == 0) {
        value = 3;
    } else if (strcmp(string, "bcdf") == 0) {
        value = 4;
    } else if (strcmp(string, "abdfg") == 0) {
        value = 5;
    } else if (strcmp(string, "abdefg") == 0) {
        value = 6;
    } else if (strcmp(string, "acf") == 0) {
        value = 7;
    } else if (strcmp(string, "abcdefg") == 0) {
        value = 8;
    } else if (strcmp(string, "abcdfg") == 0) {
        value = 9;
    }
    return value;
}

void cb(char *string, void *userdata) {
    CbParameter *cb_params = (CbParameter *) userdata;
    char wires[7] = {};
    assert(strlen(string) == 7);
    for (size_t i = 0; i < 7; ++i) {
        wires[i] = string[i];
    }

    char word[8] = {0};
    bool found = true;
    for (size_t i = 0; i < vec_size(cb_params->words); ++i) {
        wire_word(wires, cb_params->words[i], word);
        //printf("%s\n", word);
        int value = get_value_of_string(word);
        if (value != -1) {
            //printf("%s found value %d\n", word, value);
        }
        found &= value != -1;
    }
    if (found) {
        for (size_t i = 0; i < vec_size(cb_params->words); ++i) {
            wire_word(wires, cb_params->words[i], word);
            //printf("%s\n", word);
        }
        //printf("---\n");
        assert(vec_size(cb_params->words2) == 4);
        int sum = 0;
        for (size_t i = 0; i < vec_size(cb_params->words2); ++i) {
            wire_word(wires, cb_params->words2[i], word);
            int digit_value = get_value_of_string(word);
            printf("%s ", word);
            sum += (digit_value * pow(10, 3 - i));
        }
        printf(": %d\n", sum);
        cb_params->sum += sum;
    }
}

char** clear_words(char **words) {
    for (size_t i = 0; i < vec_size(words); ++i) {
        free(words[i]);
    }
    vec_clear(words);
    return words;
}

int main() {
	FILE *fp = fopen("data/input_day08.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    char sequence[16] = {0};
    bool secondspart = false;
    int counter = 0;
    CbParameter cb_params = {0};
    while (fscanf(fp, "%s", &sequence[0]) != EOF) {
        size_t len = strlen(&sequence[0]);
        if (secondspart) {
            if (len == 2 || len == 3 || len == 4 || len == 7) {
                counter++;
            }
            char *dup = strdup(&sequence[0]);
            vec_push(cb_params.words2, dup);
        } else {
            if (strcmp(&sequence[0], "|") == 0) {
                secondspart = true;
            } else {
                char *dup = strdup(&sequence[0]);
                vec_push(cb_params.words, dup);
            }
        }
        char c = fgetc(fp);
        if (c == '\n') {
            secondspart = false;
            char buf[] = "abcdefg";
            permute(buf, 0, strlen(buf), &cb, (void *) &cb_params);
            cb_params.words = clear_words(cb_params.words);
            cb_params.words2 = clear_words(cb_params.words2);
        }
    }

    fclose(fp);
    printf("Couter %d\n", counter);
    printf("Result is %ld\n", cb_params.sum);
}
