



def poly_transform(sequence):
    parts = []
    res = ""
    for i in range(0, len(sequence) - 1):
        a = sequence[i:i + 2]
        parts.append(sequence[i] + polymer[a])

    parts.append(sequence[-1])
    for part in parts:
        res += part
    return res


def calc_result(sequence):
    chars = set(list(sequence))
    data = {}
    for c in chars:
        data[c] = sequence.count(c)
    max_value = max(data.values())
    min_value = min(data.values())
    res = max_value - min_value
    return res


def read_data():
    with open("data/input_day15.txt") as fp:
        contents = fp.read().split("\n")

    s = contents[0]


    polymer = {}
    for poly in contents[2:-1]:
        key, value = poly.split(" -> ")
        polymer[key] = value

    return s, polymer

s, polymer = read_data()
for i in range(0, 10):
    s = poly_transform(s)
print(calc_result(s))


