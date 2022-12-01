
# contents = "target area: x=20..30, y=-10..-5"
contents = "target area: x=195..238, y=-93..-67"


contents = contents.replace("target area:", "")

x, y = contents.split(",")
x = x.replace(" x=", "")
y = y.replace(" y=", "")

x0, x1 = [int(e) for e in x.split("..")]
y0, y1 = [int(e) for e in y.split("..")]


def probe_in(x0, x1, y0, y1, p):
    x, y = p

    res = True
    res = res and x >= x0
    res = res and x <= x1
    res = res and y <= y1
    res = res and y >= y0
    return res


def step(probe, v):
    x, y = probe
    dx, dy = v
    x += dx
    y += dy


    if dx > 0:
        dx -= 1
    elif dx < 0:
        dx += 1

    dy -= 1
    return (x, y), (dx, dy)


def print_board(x0, x1, y0, y1, probes):
    assert y0 < 0
    assert y1 < 0
    assert x0 > 0
    assert x1 > 0

    # NOTE: inversed
    y_min = max(y0, y1)
    y_max = min(y0, y1)
    y_min = max(y_min, 0)

    x_min = min(x0, x1)
    x_max = max(x0, x1)
    y_min = min(0, y_min)
    x_min = min(0, x_min)
    for p in probes:
        x_min = min(x_min, p[0])
        x_max = max(x_max, p[0])
        y_min = max(y_min, p[1])
        y_max = min(y_max, p[1])

    # my = min(y0, y1)
    # mx = max(x0, x1)

    print(x_min, x_max)
    print(y_min, y_max)
    assert y_max < 0
    for y in range(y_min, y_max - 1, -1):
        for x in range(x_min, x_max):
            c = "."
            if x >= x0 and y <= y1:
                c = "T"
            if (x, y) in probes:
                c = "#"
            if x == 0 and y == 0:
                c = "S"
            print(c, end="")
        print()


def try_probe(probes, iv):
    v = iv
    while True:
        probe, v = step(probes[-1], v)
        probes.append(probe)
        if probe_in(x0, x1, y0, y1, probes[-1]):
            found = True
            maxp = max(p[1] for p in probes)
            # print("FOUND", iv, maxp)
            found_probes.append(iv)
            if maxp > max_y:
                pass
                # sv = iv
                # max_y = maxp
            return True

        if v[0] > x1 or (v[1] < 0 and probes[-1][1] < y0):
            # print("FAILED", iv, probes)
            break
    return False

found = False
dim = x1

max_y = 0
sv = None
y = -dim
counter = 0
found_probes = []
while y <= dim:
    x = 0
    while x <= dim:
        iv = (x, y)
        # print(iv)
        probes = [(0, 0)]
        if try_probe(probes, iv):
            counter += 1
            max_y = max(p[1] for p in probes)
        x += 1
    y += 1
# print(found, probes, probes[-1])
print(max_y)
print(counter)
