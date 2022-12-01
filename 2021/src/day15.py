import queue
from collections import defaultdict

contents = None
with open("data/input_day15.txt", "r") as fp:
    contents = fp.read().split("\n")[:-1]

offsets = [(0, 1), (1, 0), (-1, 0), (0, -1)]
# offsets = [(0, 1), (1, 0)]


def dijkstra(tiles):
    q = queue.PriorityQueue()
    q.put((0, 0, 0))

    columns = len(contents[0])
    rows = len(contents)

    def get_value(x, y):
        res = int(contents[y % rows][x % columns]) + (y // rows) + (x // columns)
        while res > 9:
            res -= 9
        return res

    distances = {}
    tile = [0, 0]
    res = None
    while not q.empty():
        value, x, y = q.get(timeout=0)
        if x < 0 or y < 0 or x >= columns * tiles or y >= rows * tiles:
            continue

        new_value = value + get_value(x, y)
        shortest = distances.get((x, y), None)

        if shortest is None or shortest > new_value:
            distances[(x, y)] = new_value
            for xo, yo in offsets:
                nx = x + xo
                ny = y + yo
                q.put((new_value, nx, ny))

            if x == (columns * tiles) - 1 and y == (rows * tiles) - 1:
                break

    return distances[(tiles * columns - 1, tiles * rows - 1)] - int(contents[0][0])

# print("Part One")
print(dijkstra(tiles=1))
# print("Part Two")
print(dijkstra(tiles=5))
