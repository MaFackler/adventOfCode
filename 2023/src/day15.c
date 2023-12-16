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

int main() {
    char *line = NULL;
    i64 res = 0;

    while (readline(&line)) {
        // print("%s", line);
        for strsplit(tok, line, ",") {
            int v = hash(tok);
            res += v;
        }
    }

    print("%ld", res);
}
