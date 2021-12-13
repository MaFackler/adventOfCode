#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"


bool is_opening_brace(char c) {
    bool res = c == '[' ||
           c == '(' ||
           c == '{' || 
           c == '<';
    return res;
}

bool is_closing_brace(char c) {
    bool res = c == ']' ||
           c == ')' ||
           c == '}' || 
           c == '>';
    return res;
}

bool is_syntax_char(char c) {
    bool res = is_opening_brace(c) || is_closing_brace(c);
    return res;
}

char get_opening_brace(char c) {
    assert(is_closing_brace(c));
    char res = 0;
    switch (c) {
        case ']':
            res = '[';
            break;
        case ')':
            res = '(';
            break;
        case '}':
            res = '{';
            break;
        case '>':
            res = '<';
            break;
        default:
            assert(false == true);
    }
    return res;
}

char get_closing_brace(char c) {
    assert(is_opening_brace(c));
    char res = 0;
    switch (c) {
        case '[':
            res = ']';
            break;
        case '(':
            res = ')';
            break;
        case '{':
            res = '}';
            break;
        case '<':
            res = '>';
            break;
        default:
            assert(false == true);
    }
    return res;
}

int get_points(char c) {
    int res = 0;
    switch (c) {
        case ')':
            res = 3;
            break;
        case ']':
            res = 57;
            break;
        case '}':
            res = 1197;
            break;
        case '>':
            res = 25137;
            break;
        default:
            assert(false == true);
    }
    assert (res != 0);
    return res;
}

int main() {
    FILE *fp = fopen("data/input_day10.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    char stack[256] = {0};
    size_t stack_size = 0;

    bool got_error = false;
    int points = 0;
    unsigned long long int *scores = NULL;
    while (1) {
        char c = fgetc(fp);
        if (c == '\n') {
            if (!got_error) {
                unsigned long long int completion = 0;
                //printf("have to complete %s\n", stack);
                for (size_t i = 0; i < stack_size; ++i) {
                    char c = stack[stack_size - 1 - i];
                    assert(is_opening_brace(c));
                    char cc = get_closing_brace(c);
                    if (cc == ')') {
                        completion = (completion * 5) + 1;
                    } else if (cc == ']') {
                        completion = (completion * 5) + 2;
                    } else if (cc == '}') {
                        completion = (completion * 5) + 3;
                    } else if (cc == '>') {
                        completion = (completion * 5) + 4;
                    } else {
                        assert(!"INVALID");
                    }
                }
                //printf("%s %llu\n", stack, completion);
                vec_push(scores, completion);
            }
            for (size_t i = 0; i < 256; ++i) {
                stack[i] = 0;
            }
            stack_size = 0;
            got_error = false;
        } else if (c == EOF) {
            break;
        } else {
            if (!got_error) {
				assert(stack_size < 256);
                stack[stack_size++] = c;
                //printf("Got %c:\n", c);
                if (is_closing_brace(c)) {
                    //printf("Got closing %c of %s\n", c, stack);
                    assert(stack_size > 0);
                    char cp = stack[stack_size - 2];
                    char ce = get_closing_brace(cp);
                    if (ce != c) {
                        //printf("Expected %c, but found %c instead\n", ce, c);
                        points += get_points(c);
                        got_error = true;
                    } else {
                        //printf("pre remove %s\n", stack);
                        stack[--stack_size] = 0;
                        stack[--stack_size] = 0;
                        //printf("Remove result %s\n", stack);
                    }
                } else {
                    //printf("got openign %c -> %s\n", c, stack);
                }
                //printf("-> %s\n", stack);
            }
        }
    }

    fclose(fp);
    //printf("Result is %d\n", points);

    qsort(scores, vec_size(scores), sizeof(unsigned long long int), vec_sort_unsigned_long_long_int);
    printf("vec_size is %lu\n", vec_size(scores));
    //assert(vec_size(scores) % 2 == 1);
    int index = (vec_size(scores) / 2);
    for (size_t i = 0; i < vec_size(scores); ++i) {
        //printf("%llu\n", scores[i]);
    }
    printf("Result Part Two is %d %llu\n", index, scores[index]);
	
}
