#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

int main() {
	FILE *fp = fopen("data/input_day08.txt", "rb");
    if (!fp)
        perror("Cannot open file");


    char sequence[16] = {0};
    bool secondspart = false;
    int counter = 0;
    while (fscanf(fp, "%s", &sequence[0]) != EOF) {
        if (secondspart) {
            size_t len = strlen(&sequence[0]);
            if (len == 2 || len == 3 || len == 4 || len == 7) {
                counter++;
            }
        }
        char c = fgetc(fp);
        if (c == '\n') {
            secondspart = false;
        }
        if (strcmp(&sequence[0], "|") == 0) {
            secondspart = true;
        }
    }
    fclose(fp);

    printf("Couter %d\n", counter);
}
