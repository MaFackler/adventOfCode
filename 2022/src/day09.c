#include "advent.h"

typedef struct {
    int x;
    int y;
} v2;

v2 v2_sub(v2 a, v2 b) {
    v2 res = {};
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    return res;
}

bool v2_equals(v2 a, v2 b) {
    return a.x == b.x && a.y == b.y;
}

bool vec_v2_in(v2 *vec, v2 a) {
    for (int i = 0; i < vec_size(vec); ++i) {
        if (v2_equals(vec[i], a)) {
            return true;
        }
    }
    return false;
}

int main() {
    char *line = NULL;
    v2 head = {};
    v2 *tails = NULL;
    v2 *heads = NULL;
    vec_push(heads, head);
    vec_push(tails, head);

    while (readline(&line)) {
        //printf("%s", line);
        char direction;
        int amount;
        sscanf(line, "%c %d\n", &direction, &amount);

        for (int i = 0; i < amount; ++i) {
            switch (direction) {
                case 'R':
                    head.x++;
                    break;
                case 'L':
                    head.x--;
                    break;
                case 'U':
                    head.y++;
                    break;
                case 'D':
                    head.y--;
                    break;
                default:
                    assert(!"INVALID");
            }

            v2 *tail = &tails[vec_size(tails) - 1];
            v2 diff = v2_sub(head, *tail);
            //printf("diff is %d/%d\n", diff.x, diff.y);
            if (abs(diff.x) > 1 || abs(diff.y) > 1) {
                vec_push(tails, heads[vec_size(heads) - 1]);
            }
            vec_push(heads, head);
            //printf("%d/%d\n", head.x, head.y);
        }
    }
    v2 *set = NULL;
    for (int i = 0; i < vec_size(tails); ++i) {
        if (!vec_v2_in(set, tails[i])) {
            vec_push(set, tails[i]);
            //printf("Got tail %d/%d\n", tails[i].x, tails[i].y);
        }
    }

    printf("%lu\n", vec_size(set));
}
