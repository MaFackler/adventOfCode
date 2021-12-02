#include <stdio.h>
#include <string.h>

int main() {
    char action[256];
    int value;

    FILE *fp = fopen("data/input_day02.txt", "r");
    if (!fp)
        perror("cannot open file");

    int depth = 0;
    int horizontal = 0;
    int aim = 0;
    while (fscanf(fp, "%s %d\n", &action[0], &value) != EOF) {
        if (strcmp(action, "forward") == 0) {
            horizontal += value;
        } else if (strcmp(action, "down") == 0) {
            depth += value;
        } else if (strcmp(action, "up") == 0) {
            depth -= value;
        } else {
            fprintf(stderr, "Unknown action %s\n", action);
        }
    }
    fclose(fp);

    printf("Depth is %d\n", depth);
    printf("Horizontal is %d\n", horizontal);
    printf("Result is %d\n", depth * horizontal);
}
