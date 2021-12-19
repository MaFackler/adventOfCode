from collections import defaultdict

def poly_transform(data, polymer):
    to_add = []
    new_data = defaultdict(int)
    for part, value in data.items():
        c = polymer[part]
        new_data[part[0] + c] += value
        new_data[c + part[1]] += value
    return new_data


def calc_result(data, last_char):
    chars = set(list("".join(data.keys())))
    res_dict = defaultdict(int)
    for part, value in data.items():
        res_dict[part[0]] += value
    res_dict[last_char] += 1

    max_value = max(res_dict.values())
    min_value = min(res_dict.values())
    res = max_value - min_value
    return res

def chop(sequence):
    res = []
    for i in range(0, len(sequence) - 1):
        a = sequence[i:i + 2]
        res.append(a)
    return res


def read_data():
    with open("data/input_day14.txt") as fp:
        contents = fp.read().split("\n")

    s = contents[0]
    polymer = {}
    for poly in contents[2:-1]:
        key, value = poly.split(" -> ")
        polymer[key] = value

    data = defaultdict(int)
    for part in chop(s):
        data[part] += 1
    return s, data, polymer

s, data, polymer = read_data()
for i in range(0, 40):
    data = poly_transform(data, polymer)

print(calc_result(data, last_char=s[-1]))


