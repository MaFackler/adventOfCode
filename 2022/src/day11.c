#include "advent.h"

int operator_mul(int left, int right) {
    return left * right;
}

int operator_add(int left, int right) {
    return left + right;
}

typedef struct {
    int *worries;
    int (*operator)(int left, int right);
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
    int *counts = NULL;
    while (readline(&line)) {
        if (line[0] == 'M') {
            Monkey *monkey = vec_add(monkeys);
            monkey->worries = NULL;

            // Parse worries
            readline(&line);
            char *start = strstr(line, ": ") + 2;
            char *token = strtok(start, ", ");
            while (token != NULL) {
                int value = str_to_int(token);
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

    for (int i = 0; i < vec_size(monkeys); ++i) {
        vec_push(counts, 0);    
    }

    const int nrounds = 20;

    for (int round_index = 0; round_index < nrounds; ++round_index) {

        for (int monkey_index = 0; monkey_index < vec_size(monkeys); ++monkey_index) {
            Monkey *monkey = &monkeys[monkey_index];
            //printf("Parse monkey %d\n", monkey_index);
            for (int i = 0; i < vec_size(monkey->worries); ++i) {
                int worry_level = monkey->worries[i];
                //printf("  Got worrie level %d\n", worry_level);
                int right = monkey->right_value;
                if (!monkey->use_right_value) {
                    right = worry_level;    
                }
                int res = monkey->operator(worry_level, right);
                res = res / 3;
                bool divisible = res % monkey->div_value == 0;
                assert(monkey->throw_to[divisible] != monkey_index);
                vec_push(monkeys[monkey->throw_to[divisible]].worries, res);
                //printf("  %d is thrown to %d\n", res, monkey->throw_to[divisible]);
                counts[monkey_index]++;
            }
            vec_clear(monkey->worries);
        }
    }

    qsort(counts, vec_size(counts), sizeof(int), cmp_int);
#if 0
    for (int i = 0; i < vec_size(counts); ++i) {
        printf("%d\n", counts[i]);
    }
#endif
    printf("%d\n", counts[0] * counts[1]);
}
