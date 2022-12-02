#include <stdio.h>
#define WIN 6
#define DRAW 3
#define LOSE 0

int scores[][3] = {
//   X  Y  Z
    {DRAW, WIN, LOSE},  // A
    {LOSE, DRAW, WIN},  // B
    {WIN, LOSE, DRAW},  // C
};

int main() {
    char *line = NULL;
    ssize_t n = 0;
    size_t a = 0;
    int res = 0;
    int res2 = 0;

    while ((n = getline(&line, &a, stdin)) != -1) {
        char opponent = 0;
        char me = 0;
        sscanf(line, "%c %c\n", &opponent, &me);
        char dest_score = me;
        int score2 = (dest_score - 'X') * 3;
        for (int i = 0; i < 3; ++i) {
            if (scores[opponent - 'A'][i] == score2) {
                score2 += i + 1;
                break;
            }
        }

        int score = scores[opponent - 'A'][me - 'X'];
        score += (me - 'X') + 1;
        res += score;
        res2 += score2;
    }
    printf("%d", res2);
}
