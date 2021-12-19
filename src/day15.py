import queue
from collections import defaultdict

contents = None
with open("data/input_day15.txt", "r") as fp:
    contents = fp.read().split("\n")[:-1]

offsets = [(0, 1), (1, 0), (-1, 0), (0, -1)]
# offsets = [(0, 1), (1, 0)]


def dijkstra():
    q = queue.PriorityQueue()
    q.put((0, 0, 0, [(0, 0)]))
    distances = {(0, 0): 0}

    res = None
    found = False
    while not q.empty() and not found:
        value, x, y, path = q.get(timeout=0)
        if x == len(contents[0]) - 1 and y == len(contents) - 1:
            # print(x, y, path, value)
            res = value
            break
        # print("Got", x, y, path)
        for xo, yo in offsets:
            nx = x + xo
            ny = y + yo
            if 0 <= nx < len(contents[0]) and 0 <= ny < len(contents):
                nvalue = int(contents[ny][nx])
                shortest = distances.get((nx, ny), None)
                if shortest is None or shortest >= value + nvalue:
                    # print("put", value + nvalue, nx, ny)
                    q.put((value + nvalue, nx, ny, path + [(nx, ny)]))
                    distances[(nx, ny)] = value + nvalue


    assert res is not None
    return res

print(dijkstra())
