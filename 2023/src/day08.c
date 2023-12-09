#include "advent.h"

typedef struct Node {
    char *data;
    char *left;
    char *right;
} Node;

bool check_finish(Node *nodes) {
    for each(node, nodes) {
        if (node->data[2] != 'Z') {
            return false;
        }
    }
    return true;
}

int main() {
    char *line = NULL;
    char *sequence = NULL;
    readline(&sequence);
    readline(&line);
    char *data = buffer(8);
    char *left = buffer(8);
    char *right = buffer(8);
    struct { char *key; Node value; } *nodes = NULL;
    sh_new_strdup(nodes);
    while(readline(&line)) {
        // print("%s", line);
        sscanf(line, "%s = (%[^,], %[^)])", data, left, right);
        Node n = {
            .data=strdup(data),
            .left=strdup(left),
            .right=strdup(right),
        };
        shput(nodes, data, n);
    }

    Node *starts = NULL;
    for (int i = 0; i < shlen(nodes); ++i) {
        if (nodes[i].value.data[2] == 'A') {
            append(starts, nodes[i].value);
        }
    }

    int res = 0;
    while (!check_finish(starts)) {

        for (int i = 0; i < len(starts); ++i) {
            for (int j = 0; j < strlen(sequence); ++j) {
                if (sequence[j] == 'L') {
                    starts[i] = shget(nodes, starts[i].left);
                } else {
                    starts[i] = shget(nodes, starts[i].right);
                }
            }
        }
        res += strlen(sequence);
    }

#if 0
    Node current = shget(nodes, "AAA");
    while (!streql(current.data, "ZZZ")) {
        for (int i = 0; i < strlen(sequence); ++i) {
            if (sequence[i] == 'L') {
                current = shget(nodes, current.left);
            } else {
                current = shget(nodes, current.right);
            }
        }
    }
#endif

    print("%d", res);
}
