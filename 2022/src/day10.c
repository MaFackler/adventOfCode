#include "advent.h"

enum InstructionType {
    INSTRUCTION_TYPE_NOOP,
    INSTRUCTION_TYPE_ADDX,
};

typedef struct {
    enum InstructionType type;
    int cycles;
    int value;
} Instruction;

int main() {
    char *line = NULL;
    Instruction *instructions = NULL;
    int x = 1;
    while (readline(&line)) {
        char buf[256] = {0};
        int value = 0;

        if (strlen(line) > 1) {  // NOTE: \n
            Instruction *ins = vec_add(instructions);
            if (line[0] == 'n') {
                ins->type = INSTRUCTION_TYPE_NOOP;
                ins->cycles = 1;
            } else {
                assert(line[0] == 'a');
                sscanf(line, "%s %d", &buf[0], &value);
                ins->type = INSTRUCTION_TYPE_ADDX;
                ins->value = value;
                ins->cycles = 2;
            }
        }
    }

    int instruction_counter = 0;
    int res = 0;
    for (int i = 0; i < vec_size(instructions);) {
        Instruction *instruction = &instructions[i];

        while (instruction->cycles--) {
            //printf("instruction nr %d ", instruction_counter + 1);
            #define ADD_TO_RES(n) \
                if (instruction_counter == n - 1) { \
                    int diff = (n) * x; \
                    res += diff; \
                }

            ADD_TO_RES(20);
            ADD_TO_RES(60);
            ADD_TO_RES(100);
            ADD_TO_RES(140);
            ADD_TO_RES(180);
            ADD_TO_RES(220);

            #undef ADD_TO_RES
            if (instruction->cycles == 0) {
                if (instruction->type == INSTRUCTION_TYPE_ADDX) {
                    //printf("add value %d", instruction->value);
                    x += instruction->value;
                } else {
                    //printf("instruction noop");
                }
                i++;
            }
            //printf("\n");

#if 0
            if (instruction_counter == 219) {
                printf("x is %d\n", x);
            }
#endif
            instruction_counter++;
        }
    }
    printf("%d\n", res);
}
