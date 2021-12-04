import sys

data = []
amount_lines = 0
with open("data/input_day03.txt") as fp:
    for line in fp.readlines():
        data.append(line[:-1])
        amount_lines += 1;


if not data:
    sys.exit()


num_bits = len(data[0])
bit_values = [0, ] * num_bits
for bit_index in range(0, num_bits):
    for bitstring in data:
        if bitstring[bit_index] == '1':
            bit_values[bit_index] += 1



def do_filter(data, index, filterfunc):
    res = []
    threshold = len(data) // 2
    if len(data) % 2 != 0:
        threshold += 1
    bits_set = sum([1 for d in data if d[index] == "1"])
    print("bits set", bits_set, len(data), data)
    for bitstring in data:
        value = bitstring[index]
        if filterfunc(bits_set, threshold, value):
            res.append(bitstring)
    if not res:
        res = [data[-1]]
    print(res)
    return res


def oxy_filter(bits_set, threshold, value):
    print(bits_set, threshold, value)
    if bits_set == threshold:
        return value == "1"
    common_value = "1" if bits_set > threshold else "0"
    return common_value == value

def scrubber_filter(bits_set, threshold, value):
    if bits_set == threshold:
        return value == "0"
    common_value = "0" if bits_set > threshold else "1"
    return common_value == value


oxies = list(data)
scrubber = list(data)
for bit_index in range(0, num_bits):
    oxies = do_filter(oxies, bit_index, oxy_filter)
    scrubber = do_filter(scrubber, bit_index, scrubber_filter)

oxi_value = int(oxies[0], 2)
scrubber_value = int(scrubber[0], 2)
print("oxi is", oxi_value)
print("scrubber_value", scrubber_value)
print(oxi_value * scrubber_value)








