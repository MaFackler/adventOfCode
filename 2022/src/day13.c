#include "advent.h"

typedef struct ListItem {
    int value;
    struct ListItem *values;
} ListItem;


ListItem _parse_rec(char **it) {
    ListItem res = {-1, NULL};
    int value = -1;
    while (**it) {
        char c = *(*it)++;
        if (c == '[') {
            ListItem new_list = _parse_rec(it);
            vec_push(res.values, new_list);
        } else if (c == ',') {
            if (value >= 0) {
                vec_push(res.values, ((ListItem) {value, NULL}));
                value = -1;
            }
        } else if (c == ']') {
            if (value >= 0) {
                vec_push(res.values, ((ListItem) {value, NULL}));
                value = -1;
            }
            return res;
        } else if (isalnum(c)) {
            if (value == -1) {
                value = c - '0';
            } else {
                value = (value * 10) + (c - '0');
            }
        } else {
            assert(!"INVALID");
        }
    }
    return res;
}

ListItem parse(char *s) {
    assert(strlen(s) > 0 && s[0] == '[');
    s++;
    return _parse_rec(&s);
}


void print_list_item(ListItem *item) {
    if (item->value != -1) {
        printf("%d", item->value);
    } else {
        printf("[");
        for (int i = 0; i < vec_size(item->values); ++i) {
            print_list_item(&item->values[i]);
            if (i < (vec_size(item->values) - 1)) {
                printf(",");
            }
        }
        printf("]");
    }
}


int cmp_list_item(const void *aa, const void *bb) {
    const ListItem *a = (ListItem *) aa;
    const ListItem *b = (ListItem *) bb;
    ListItem a_as_list = {-1, NULL};
    ListItem b_as_list = {-1, NULL};
    if (a->value != -1 && b->value == -1) {
        // a is value and b is list -> transform a to list
        vec_push(a_as_list.values, ((ListItem) {a->value, NULL}));
        a = &a_as_list;
    }
    if (a->value == -1 && b->value != -1) {
        // a is list and b is value -> transform b to list
        vec_push(b_as_list.values, ((ListItem) {b->value, NULL}));
        b = &b_as_list;
    }

    if (a->value != -1 && b->value != -1) {
        // both integers
        if (a->value < b->value) {
            return 1;
        } else if (a->value == b->value) {
            return 0;
        } else {
            return -1;
        }
    }

    if (a->value == -1 && b->value == -1) {
        int i = 0;
        while (i < vec_size(a->values) && i < vec_size(b->values)) {
            int res = cmp_list_item(&a->values[i], &b->values[i]);
            if (res == 1)
                return res;
            if (res == -1)
                return res;
            i++;
        }
        if (i == vec_size(a->values)) {
            if (i == vec_size(b->values)) {
                return 0;
            }
            return 1;
        }
        return -1;
    }
    return -1;
}
int cmp_list_item_asc(const void *aa, const void *bb) {
    int res = cmp_list_item(aa, bb);
    return -res;
}

int main() {
    char *line = NULL;
    int res = 0;
    int i = 0;
    ListItem *packages = NULL;
    while (readline(&line)) {
        if (strlen(line) > 1) {
            i++;
            ListItem p1 = parse(line);
            readline(&line);
            ListItem p2 = parse(line);
#if 0
            print_list_item(&p1);
            printf("\n");
            print_list_item(&p2);
            printf("\n");
#endif
            if (cmp_list_item(&p1, &p2) == 1) {
                res += i;
            } else {
            }

            vec_push(packages, p1);
            vec_push(packages, p2);

        }
    }
    // 
    ListItem a1 = parse("[[2]]");
    ListItem a2 = parse("[[6]]");
    vec_push(packages, a1);
    vec_push(packages, a2);

    qsort(packages, vec_size(packages), sizeof(ListItem), cmp_list_item_asc);
    int res2 = 1;
    for (int i = 0; i < vec_size(packages); ++i) {
        ListItem *item = &packages[i];
        if (item->value == -1 && vec_size(item->values) == 1 && vec_size(item->values[0].values) == 1) {
            int value = item->values[0].values[0].value;
            if (value == 2 || value == 6) {
                res2 *= (i + 1);
            }
        }
    }

    printf("%d\n", res2);
}
