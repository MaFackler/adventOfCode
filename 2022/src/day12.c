#include "advent.h"

typedef struct {
    int x;
    int y;
    int value;
} Path;

bool cmp_path_coords(Path a, Path b) {
    return a.x == b.x && a.y == b.y;
}

bool is_visited(Path *visited, Path item) {
    for (int i = 0; i < vec_size(visited); ++i) {
        if (cmp_path_coords(visited[i], item)) {
            return true;
        }
    }
    return false;
}

Path* look_at(Path *queue, int *data, int x_dim, int y_dim, Path item, int dx, int dy) {
    Path near = {
        .x=item.x + dx,
        .y=item.y + dy,
        .value=item.value + 1,
    };

    if (near.x < 0 || near.x >= x_dim)
        return queue;

    if (near.y < 0 || near.y >= y_dim)
        return queue;

    int v1 = data[item.y * x_dim + item.x];
    int v2 = data[near.y * x_dim + near.x];

    if (v2 - v1 < 2) {
        vec_push(queue, near);
    }
    return queue;
}

int main() {
    char *line = NULL;
    int *data = NULL;
    int x_dim = 0;
    int y_dim = 0;
    Path *queue = NULL;
    Path *visited = NULL;
    Path end = {0};

    while (readline(&line)) {
        x_dim = strlen(line) - 1;
        for (int i = 0; i < x_dim; ++i) {
            char c = line[i];
            int v = -1;
            if (c == 'S') {
                c = 'a';
            }
            if (c == 'E') {
                c = 'z';
                end.x = i;
                end.y = y_dim;
            }
            v = c - 'a';
            if (v == 0) {
                vec_push(queue, ((Path) {i, y_dim, 0}));
            }
            vec_push(data, v);
        }
        y_dim++;
    }

    int res = -1;
    while (vec_size(queue) > 0) {
        Path item = queue[0];
        memcpy(&queue[0], &queue[1], sizeof(Path) * vec_size(queue) - 1);
        vec_header(queue)->size--;

        if (is_visited(visited, item)) {
            continue;
        }
        vec_push(visited, item);

        if (cmp_path_coords(item, end)) {
            res = item.value;
            break;
        }

        queue = look_at(queue, data, x_dim, y_dim, item, 1, 0);
        queue = look_at(queue, data, x_dim, y_dim, item, -1, 0);
        queue = look_at(queue, data, x_dim, y_dim, item, 0, 1);
        queue = look_at(queue, data, x_dim, y_dim, item, 0, -1);
    }
    printf("%d\n", res);

}
