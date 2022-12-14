#include "advent.h"
#include <stdint.h>

typedef uint64_t u64;

int cmp_u64(const void *a, const void *b) {
    return *(u64 *)a < *(u64*) b;
}

u64 operator_mul(u64 left, u64 right) {
    return left * right;
}

u64 operator_add(u64 left, u64 right) {
    return left + right;
}

typedef struct {
    u64 *worries;
    u64 (*operator)(u64 left, u64 right);
    bool use_right_value;
    int right_value;
    int div_value;
    int throw_to[2];
} Monkey;

Monkey *monkey_create() {
    Monkey *monkey = calloc(sizeof(Monkey), 1);
    return monkey;
}

int main() {
    char *line = NULL;
    Monkey *monkeys = NULL;
    u64 *counts = NULL;
    while (readline(&line)) {
        if (line[0] == 'M') {
            Monkey *monkey = vec_add(monkeys);
            monkey->worries = NULL;

            // Parse worries
            readline(&line);
            char *start = strstr(line, ": ") + 2;
            char *token = strtok(start, ", ");
            while (token != NULL) {
                u64 value = str_to_int(token);
                vec_push(monkey->worries, value);
                token = strtok(NULL, ", ");
            }

            // Parse operation
            readline(&line);
            char left[256] = {};
            char right[256] = {};
            char operator = 0;
            sscanf(line, "  Operation: new = %s %c %s", &left[0], &operator, &right[0]);
            assert(str_equal(left, "old"));
            if (str_equal(right, "old")) {
                monkey->right_value = 0;
                monkey->use_right_value = false;
            } else {
                monkey->right_value = str_to_int(&right[0]);
                monkey->use_right_value = true;
            }
            switch (operator) {
                case '*':
                    monkey->operator = operator_mul;
                    break;
                case '+':
                    monkey->operator = operator_add;
                    break;
                default:
                    assert(!"INVALID");
                    break;
            }

            // Parse test
            readline(&line);
            int div_value = 0;
            sscanf(line, "  Test: divisible by %d\n", &div_value);
            assert(div_value != 0);
            monkey->div_value = div_value;
            // True to
            int to_monkey = -1;
            readline(&line);
            sscanf(line, "    If true: throw to monkey %d\n", &to_monkey);
            assert(to_monkey >= 0);
            monkey->throw_to[true] = to_monkey;
            // False to
            readline(&line);
            to_monkey = -1;
            sscanf(line, "    If false: throw to monkey %d\n", &to_monkey);
            assert(to_monkey >= 0);
            monkey->throw_to[false] = to_monkey;
        }
    }

    int part_two_divisor = 1;
    for (int i = 0; i < vec_size(monkeys); ++i) {
        vec_push(counts, 0);    
        part_two_divisor = part_two_divisor * monkeys[i].div_value;
    }

    const int nrounds = 10000;

    for (int round_index = 0; round_index < nrounds; ++round_index) {

        for (int monkey_index = 0; monkey_index < vec_size(monkeys); ++monkey_index) {
            Monkey *monkey = &monkeys[monkey_index];
            //printf("Parse monkey %d\n", monkey_index);
            for (int i = 0; i < vec_size(monkey->worries); ++i) {
                u64 worry_level = monkey->worries[i];
                //printf("  Got worrie level %d\n", worry_level);
                int right = monkey->right_value;
                if (!monkey->use_right_value) {
                    right = worry_level;    
                }
                u64 res = monkey->operator(worry_level, right);
                res = res % part_two_divisor;
                bool divisible = res % monkey->div_value == 0;
                assert(monkey->throw_to[divisible] != monkey_index);
                vec_push(monkeys[monkey->throw_to[divisible]].worries, res);
                //printf("  %d is thrown to %d\n", res, monkey->throw_to[divisible]);
                counts[monkey_index]++;
            }
            vec_clear(monkey->worries);
        }
    }

    qsort(counts, vec_size(counts), sizeof(u64), cmp_u64);
#if 0
    for (int i = 0; i < vec_size(counts); ++i) {
        printf("%lu\n", counts[i]);
    }
#endif
    printf("%lu\n", counts[0] * counts[1]);
}
