#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


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

    int line_counter = 1;
    bool break_till_new_line = false;
    int points = 0;
    while (1) {
        char c = fgetc(fp);
        if (c == '\n') {
            line_counter++;
            for (size_t i = 0; i < 256; ++i) {
                stack[i] = 0;
            }
            //printf("newline %d\n", line_counter);
            stack_size = 0;
            break_till_new_line = false;
        } else if (c == EOF) {
            break;
        } else {
            if (!break_till_new_line) {
                stack[stack_size++] = c;
                //printf("Got %c:\n", c);
                if (is_closing_brace(c)) {
                    //printf("Got closing %c of %s\n", c, stack);
                    assert(stack_size > 0);
                    char cp = stack[stack_size - 2];
                    char ce = get_closing_brace(cp);
                    if (ce != c) {
                        printf("Expected %c, but found %c instead\n", ce, c);
                        points += get_points(c);
                        break_till_new_line = true;
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

    printf("Result is %d\n", points);
    fclose(fp);
	
}
