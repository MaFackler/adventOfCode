#include "advent.h"


int hash(char *s) {
    int res = 0;
    for (int i = 0; i < strlen(s); ++i) {
        res += s[i];
        res *= 17;
        res = res % 256;
    }
    return res;
}

typedef struct Lense {
    char label[16];
    int value;
} Lense;

typedef struct Box {
    Lense *lenses;
} Box;

int box_find(Box *box, char *label) {
    int boxid = -1;
    for (int i = 0; i < len(box->lenses); ++i) {
        Lense *l = &box->lenses[i];
        if (streql(l->label, label)) {
            boxid = i;
            break;
        }
    }
    return boxid;
}

void print_lenses(Box *box) {
    for (int i = 0; i < len(box->lenses); ++i) {
        printf("[%s %d], ", box->lenses[i].label, box->lenses[i].value);
    }
    printf("\n");
}

int main() {
    char *line = NULL;
    // i64 res = 0;

    Box HASHMAP[256] = {0};

    char *label = buffer(1024);
    char *cmd = buffer(1024);
    while (readline(&line)) {
        // print("%s", line);
        for strsplit(tok, line, ",") {
#if 0
            int v = hash(tok);
            res += v;
#endif
            sscanf(tok, "%[^-=]%s", label, cmd);
            int h = hash(label);
            Box *box = &HASHMAP[h];
            int boxid = box_find(box, label);
            if (cmd[0] == '-') {
                if (boxid >= 0) {
                    memcpy(&box->lenses[boxid], &box->lenses[boxid + 1], sizeof(Lense) * (len(box->lenses) - boxid - 1));
                    vec_header(box->lenses)->size--;
                }
            } else {
                // add
                assert (cmd[0] == '=');
                int value = atoi(&cmd[1]);
                if (boxid >= 0) {
                    box->lenses[boxid].value = value;
                } else {
                    Lense *l = add(box->lenses);
                    strcpy(&l->label[0], label);
                    l->value = value;
                }
            }

            //print_lenses(&HASHMAP[0]);
            // print_lenses(&HASHMAP[3]);
            // print("%lu", len(HASHMAP[0].lenses));
            // print("%lu", len(HASHMAP[3].lenses));
        }
    }

    i64 res = 0;
    for (int boxid = 0; boxid < ArraySize(HASHMAP); ++boxid) {
        Box *box = &HASHMAP[boxid];
        if (len(box->lenses))
            // print("Box %d", boxid);
        for (int i = 0; i < len(box->lenses); ++i) {
            Lense *lense = &box->lenses[i];
            res += (boxid + 1) * (i + 1) * lense->value;
        }
    }
    print("%ld", res);

}
