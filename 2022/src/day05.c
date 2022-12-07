#include "advent.h"

void print_stack(char *stack) {
    printf("%.*s\n", (int) vec_size(stack), stack);
}

int main() {
    char *res = NULL;
    char *line = NULL;
    char *stacks[16] = {0};
    bool parse_moves = false;

    while (readline(&line)) {
        //printf("%s", line);
        if (str_equal(line, "\n")) {
            parse_moves = true;
            continue;
        }

        if (!parse_moves) {
            for (int i = 1; i < strlen(line); i += 4) {
                char c = line[i];
                if (c >= 'A' && c <= 'Z') {
                    int stack_index = (i - 1) / 4;
                    vec_push(stacks[stack_index], c);
                }
            }
        } else {
            int amount, stack_index_from, stack_index_to;
            sscanf(line, "move %d from %d to %d\n", &amount, &stack_index_from, &stack_index_to);
            stack_index_from--;
            stack_index_to--;
            char *stack_from = stacks[stack_index_from];
            char *stack_to = stacks[stack_index_to];
#if 0 
            for (int i = 0; i < amount; ++i) {
                if (vec_size(stack_from)) {
                    // pop front
                    char c = stack_from[0];
                    memcpy(&stack_from[0], &stack_from[1], vec_size(stack_from) - 1);
                    vec_header(stack_from)->size--;

                    // push front
                    vec_push(stack_to, '0');  // NOTE: make sure vector has enough size
                    memcpy(&stack_to[1], &stack_to[0], vec_size(stack_to) - 1);
                    stack_to[0] = c;
                }
            }
#endif
            amount = MIN(vec_size(stack_from), amount);
            char *tmp = NULL;
            for (int i = 0; i < amount; ++i) {
                char c = stack_from[0];
                memcpy(&stack_from[0], &stack_from[1], vec_size(stack_from) - 1);
                vec_header(stack_from)->size--;
                vec_push(tmp, c);
            }

            for (int i = 0; i < amount; ++i) {
                vec_push(stack_to, '0');
            }

            memcpy(&stack_to[amount], &stack_to[0], vec_size(stack_to) - amount);

            for (int i = 0; i < amount; ++i) {
                assert(i < vec_size(tmp));
                stack_to[i] = tmp[i];
            }

            vec_clear(tmp);

            // NOTE: store pointers back in a case of reallocation
            stacks[stack_index_from] = stack_from;
            stacks[stack_index_to] = stack_to;
        }
    }

    for (int i = 0; i < ArraySize(stacks); ++i) {
        if (vec_size(stacks[i])) {
            //print_stack(stacks[i]);
            vec_push(res, stacks[i][0]);
        }
    }
    vec_push(res, 0);  // NOTE: null terminator

    printf("%s\n", res);
}
