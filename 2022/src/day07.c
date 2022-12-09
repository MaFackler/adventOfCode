#include "advent.h"

bool str_startswithc(char *s, char c) {
    return strlen(s) > 0 && s[0] == c;
}

bool str_startswith(char *s, char *b) {
    assert(strlen(b) <= strlen(s));
    return strncmp(s, b, strlen(b));
}

enum NodeType {
    NODE_TYPE_UNDEFINED,
    NODE_TYPE_DIRECTORY,
    NODE_TYPE_FILE,
};

typedef struct Node {
    enum NodeType node_type;
    char *name;
    int size;  // NOTE: only true for directories
    struct Node **children;
    struct Node *parent;
} Node;

Node* node_create() {
    Node *res = (Node *) malloc(sizeof(Node));
    res->node_type = NODE_TYPE_UNDEFINED;
    res->name = NULL;
    res->size = 0;
    res->children = NULL;
    res->parent = NULL;
    return res;
}

Node* node_create_child(Node *node, char *name, size_t n) {
    Node *child = node_create();
    vec_push(node->children, child);
    child->parent = node;
    child->name = strndup(name, n);
    return child;
}

Node* node_get_child(Node *node, char *name, size_t n) {
    Node *res = NULL;
    for (int i = 0; i < vec_size(node->children); ++i) {
        Node *child = node->children[i];
        if (strncmp(child->name, name, n) == 0) {
            res = child;
            break;
        }
    }
    return res;
}

void node_print(Node *node, size_t indent) {
    assert(node->name != NULL);
    assert(node->node_type != NODE_TYPE_UNDEFINED);
    for (int i = 0; i < indent; ++i) {
        printf(" ");
    }

    if (node->node_type == NODE_TYPE_DIRECTORY) {
        printf("- %s (dir)\n", node->name);
    } else {
        printf("- %s (file, size=%d)\n", node->name, node->size);
    }
    for (int i = 0; i < vec_size(node->children); ++i) {
        node_print(node->children[i], indent + 2);
    }
}

int node_get_total_size(Node *node) {
    int res = 0;
    if (node->node_type == NODE_TYPE_FILE) {
        res += node->size;
    }
    for (int i = 0; i < vec_size(node->children); ++i) {
        res += node_get_total_size(node->children[i]);
    }
    return res;
}

void collect_size(Node *node, int *res, int threshold) {
    int actual = node_get_total_size(node);
    assert(node->node_type == NODE_TYPE_DIRECTORY);
    if (actual < threshold) {
        *res = *res + actual;
    }

    for (int i = 0; i < vec_size(node->children); ++i) {
        Node *child = node->children[i];
        if (child->node_type == NODE_TYPE_DIRECTORY)
            collect_size(child, res, threshold);
    }
}


typedef struct {
    char *path;
    int size;
} NodeListItem;

static NodeListItem *nodes = NULL;

void tree_to_list(Node *node, int threshold) {
    int size = node_get_total_size(node);
    if (size >= threshold) {
        NodeListItem *item = vec_add(nodes);
        Node *c = node;
        char buf[256] = {0};
        while (c != NULL) {
            strcat(&buf[0], c->name);
            c = c->parent;
        }
        item->path = strdup(&buf[0]);
        item->size = size;
    }

    for (int i = 0; i < vec_size(node->children); ++i) {
        Node *child = node->children[i];
        if (child->node_type == NODE_TYPE_DIRECTORY)
            tree_to_list(child, threshold);
    }
}


int cmp_node_list_item(const void *a, const void *b) {
    return ((NodeListItem*) a)->size > ((NodeListItem*) b)->size;
}

int main() {

    Node *root = NULL;
    Node *current_node = NULL;
    char *line = NULL;
    while (readline(&line)) {
        //printf("parse line %s", line);
        if (str_startswithc(line, '$')) {
            // NOTE: command
            if (str_equal(line, "$ cd /\n")) {
                if (root == NULL) {
                    root = node_create();
                    root->name = "/";
                    root->node_type = NODE_TYPE_DIRECTORY;
                }
                current_node = root;
            } else if (str_equal(line, "$ ls\n")) {
            } else {
                assert(str_startswith(line, "cd"));
                assert(current_node != NULL);
                assert(root != NULL);
                if (line[5] == '.') {
                    assert(line[6] == '.');
                    assert(current_node->parent != NULL);
                    current_node = current_node->parent;
                } else {
                    Node *child = node_get_child(current_node, &line[5], strlen(line) - 5 - 1);
                    // NOTE: assume child is already parsed with dir
#if 0
                    if (child == NULL) {
                        child = node_create_child(current_node, &line[5], strlen(line) - 5 - 1);
                        child->node_type = NODE_TYPE_DIRECTORY;
                    }
#endif
                    assert(child != NULL);
                    current_node = child;
                }
            }

        } else {
            // ls result
            int size = 0;
            char buf[256] = {0};
            enum NodeType node_type = NODE_TYPE_UNDEFINED;
            if (str_startswithc(line, 'd')) {
                sscanf(line, "dir %s\n", &buf[0]);
                node_type = NODE_TYPE_DIRECTORY;
            } else {
                sscanf(line, "%d %s\n", &size, &buf[0]);
                node_type = NODE_TYPE_FILE;
                //printf("got file %s with size %d\n", &buf[0], size);
            }
            Node *child = node_create_child(current_node, &buf[0], strlen(&buf[0]));
            child->node_type = node_type;
            child->size = size;
        }
    }

    // TODO: this solution is way to complicated :P
    // node_print(root, 0);
    //int res = 0;
    //const int threshold = 100000;
    const int total_disk_space = 70000000;
    const int expected_disk_free_space = 30000000;
    int actual_used_space = node_get_total_size(root);
    int actual_free_space = total_disk_space - actual_used_space;
    assert(expected_disk_free_space >= actual_free_space);
    int space_to_delete = expected_disk_free_space - actual_free_space;

    tree_to_list(root, space_to_delete);

    qsort(nodes, vec_size(nodes), sizeof(NodeListItem), cmp_node_list_item);
#if 0
    for (int i = 0; i < vec_size(nodes); ++i) {
        NodeListItem *item = &nodes[i];
        printf("Item %s has size %d\n", item->path, item->size);
    }
#endif

    printf("%d\n", nodes[0].size);

}

#if 0 
- / (dir)
  - a (dir)
    - e (dir)
      - i (file, size=584)
    - f (file, size=29116)
    - g (file, size=2557)
    - h.lst (file, size=62596)
  - b.txt (file, size=14848514)
  - c.dat (file, size=8504156)
  - d (dir)
    - j (file, size=4060174)
    - d.log (file, size=8033020)
    - d.ext (file, size=5626152)
    - k (file, size=7214296)}
#endif
