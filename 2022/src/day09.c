#include "advent.h"

#define vec_last(v) &v[vec_size(v) - 1]

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

bool move(const v2 *head, const v2 *tail, v2 *out) {
    bool res = false;
    v2 diff = v2_sub(*head, *tail);
    if (abs(diff.x) > 1) {
        res = true;
        out->y = head->y;
        out->x = (diff.x > 0) ? (head->x - 1) : (head->x + 1);
        assert(abs(diff.x) <= 2);
        assert(abs(diff.y) <= 2);
        if (abs(diff.y) > 1) {
            out->y = (diff.y > 0) ? (head->y - 1) : (head->y + 1);
        }
    } else if (abs(diff.y) > 1) {
        res = true;
        out->x = head->x;
        out->y = (diff.y > 0) ? (head->y - 1) : (head->y + 1);
        assert(abs(diff.y) <= 2);
        assert(abs(diff.x) <= 2);
        if (abs(diff.x) > 1) {
            out->x = (diff.x > 0) ? (head->x - 1) : (head->x + 1);
        }
    } else {
        //printf("Diff is %d/%d\n", diff.x, diff.y);
        assert((abs(diff.x) == 1 && abs(diff.y) == 0) || ((abs(diff.y) == 1) && (abs(diff.x) == 0)) || (diff.x == 0 && diff.y == 0) || (abs(diff.x) == 1 && abs(diff.y) == 1));
    }
    return res;
}

int main() {
    char *line = NULL;
    v2 head = {};
    const int nropes = 9;
    v2 *ropes = NULL;
    v2 *heads = NULL;
    vec_push(heads, head);

    for (int i = 0; i < nropes; ++i) {
        vec_push(ropes, head);
    }

    v2 *set = NULL;

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

            v2 out = {};
            for (int j = 0; j < nropes; ++j) {
                v2 *move_to = &head;
                if (j > 0)
                    move_to = &ropes[j - 1];
                if (move(move_to, &ropes[j], &out)) {
                    ropes[j] = out;
                }
            }
            if (!vec_v2_in(set, ropes[8])) {
                vec_push(set, ropes[8]);
            }
            vec_push(heads, head);
        }
    }

    for (int i = 0; i < vec_size(set); ++i) {
        //printf("%d/%d\n", set[i].x, set[i].y);
    }

    printf("%lu\n", vec_size(set));
}
