#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"

typedef struct {
    int *route;
} Path;

Path* path_new() {
    Path *res = (Path *) malloc(sizeof(Path));
    res->route = NULL;
    return res;
}

Path *path_copy(Path *other) {
    Path *res = path_new();
    for (size_t i = 0; i < vec_size(other->route); ++i) {
        vec_push(res->route, other->route[i]);
    }
    return res;
}

bool path_contains(Path *path, int id) {
    return vec_int_contains(path->route, id);
}

void path_append(Path *path, int id) {
    vec_push(path->route, id);
}





typedef struct {
    int id;  // NOTE: id is the idex of the node in the pool
    char *name;
    int *children;
} Node;


static size_t result = 0;
static size_t pool_index = 0;
static Node node_pool[1024] = {0};


void node_pool_print_path(Path *path) {
    for (size_t i = 0; i < vec_size(path->route); ++i) {
        int index = path->route[i];  
        Node *node = &node_pool[index];
        printf("%s,", node->name);
    }
    printf("\n");
}

int node_pool_get_index(const char *name) {
    for (size_t i = 0; i < pool_index; ++i) {
        Node *node = &node_pool[i];
        if (strcmp(name, &node->name[0]) == 0) {
            return i;
        }
    }
    return -1;
}

Node *node_pool_get(const char *name) {
    int index = node_pool_get_index(name);
    assert(index >= 0);
    return &node_pool[index];
}

Node* node_pool_get_or_create(const char *name) {
    int index = node_pool_get_index(name);
    Node *res = NULL;
    if (index == -1) {
        res = &node_pool[pool_index];    
        res->name = strdup(name);
        res->id = pool_index;
        res->children = NULL;
        pool_index++;
    } else {
        res = &node_pool[index];
    }
    assert(res != NULL);
    return res;
}

void node_add_child(Node *node, int id) {
    for (size_t i = 0; i < vec_size(node->children); ++i) {
        if (node->children[i] == id)
            return;
    }
    vec_push(node->children, id);
}

Node* node_pool_create_linked(const char *left, const char *right) {
    Node *a = node_pool_get_or_create(left);
    Node *b = node_pool_get_or_create(right);
    node_add_child(a, b->id);
    node_add_child(b, a->id);
    return a;
}


void traverse2(Node *node, Path *path, Node *start, Node *end, bool foundTwice) {
    if (strcmp(node->name, "end") == 0) {
        result++;
        return;
    }
    for (size_t i = 0; i < vec_size(node->children); ++i) {
        Node *child = &node_pool[node->children[i]];
        if (!path_contains(path, child->id)) {
            // Not in path
            Path *to_traverse = path;
            if (!string_is_upper(child->name)) {
                Path *new_path = path_copy(path);  // TODO: memory???
                path_append(new_path, child->id);
                to_traverse = new_path;
            }
            traverse2(child, to_traverse, start, end, foundTwice);
        } else {
            if (foundTwice == false && child != start && child != end) {
                traverse2(child, path, start, end, true);
            }
        }
    }
}

void traverse(Node *node, Path *path) {
    if (strcmp(node->name, "end") == 0) {
        result++;
        return;
    }
    for (size_t i = 0; i < vec_size(node->children); ++i) {
        Node *child = &node_pool[node->children[i]];
        if (!path_contains(path, child->id)) {
            Path *to_traverse = path;
            if (!string_is_upper(child->name)) {
                Path *new_path = path_copy(path);  // TODO: memory???
                path_append(new_path, child->id);
                to_traverse = new_path;
            }
            traverse(child, to_traverse);
        }
    }
}

int main() {

    FILE *fp = fopen("data/input_day12.txt", "rb");
    if (!fp)
        perror("Cannot open file");

    char left[256] = {0};
    char right[256] = {0};
    bool use_right = false;
    size_t index = 0;
    while (1) {
        char c = fgetc(fp);
        char *dest = use_right ? &right[0] : &left[0];
        if (isalpha(c)) {
            dest[index++] = c;
        } else if (c == '-') {
            use_right = true;
            index = 0;
        } else if (c == '\n') {
            //printf("%s - %s\n", &left[0], &right[0]);
            node_pool_create_linked(&left[0], &right[0]);
            use_right = false;
            memset(left, 0, sizeof(left));
            memset(right, 0, sizeof(right));
            index = 0;
        } else {
            break;
        }
    }


    Node *start = node_pool_get("start");
    Node *end = node_pool_get("end");
    Path *path = path_new();
    path_append(path, start->id);
    traverse(start, path);
    printf("Part One result is %lu\n", result);
    result = 0;
    traverse2(start, path, start, end, false);

    printf("Part Two result is %lu\n", result);


    fclose(fp);
}
