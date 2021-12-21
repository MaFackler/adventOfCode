import queue

def hex_to_binary(s: str):
    res = ""
    for c in s:
        chunk = str(bin(int(c, 16)))[2:]
        chunk = chunk.zfill(4)
        res += chunk
    return res

def bin_to_dec(s: str):
    return int(s, 2)


def analyze_binary_string(s: str, i, packages):

    # print("\nGot package", s, len(s), i, s[i:i + 10])
    version = bin_to_dec(s[i:i + 3])
    typeid = bin_to_dec(s[i + 3:i + 6])
    # print("Version", version)
    # print("Typeid", typeid)
    i += 6

	
    if typeid == 4:
        # literal value
        # print("Literal")
        chunk_size = 5
        package = ""
        while i < len(s):
            chunk = s[i:i + chunk_size]
            i += chunk_size
            package += chunk[1:]

            if chunk[0] == "0":
                break

        value = bin_to_dec(package)
        packages.append((version, typeid, value))
        return value, i
    else:
        values = []
        packages.append((version, typeid, None))
        assert i < len(s), str(i) + str(s)
        if s[i] == "0":
            i += 1
            bits_to_parse = 15
            len_subpackages = bin_to_dec(s[i:i + bits_to_parse])
            i += bits_to_parse
            start_i = i
            while True:
                value, new_i = analyze_binary_string(s, i, packages)
                assert value is not None
                values.append(value)
                i = new_i
                if i - start_i == len_subpackages:
                    break
        else:
            i += 1
            bits_to_parse = 11
            amount = bin_to_dec(s[i:i + bits_to_parse])
            i += bits_to_parse
            for _ in range(0, amount):
                value, i = analyze_binary_string(s, i, packages)
                assert(value is not None), i
                values.append(value)


        assert None not in values

        retvalue = None
        if typeid == 0:
            retvalue =  sum(values)
        elif typeid == 1:
            res = 1
            for val in values:
                res *= val
            retvalue = res
        elif typeid == 2:
            retvalue = min(values)
        elif typeid == 3:
            retvalue = max(values)
        elif typeid == 5:
            assert len(values) == 2, values
            retvalue =  1 if values[0] > values[1] else 0
        elif typeid == 6:
            assert len(values) == 2
            retvalue = 1 if values[0] < values[1] else 0
        elif typeid == 7:
            assert len(values) == 2
            retvalue = 1 if values[0] == values[1] else 0
        else:
            assert False, typeid
        assert retvalue is not None
        return retvalue, i





        
literal = "D2FE28"
operator = "38006F45291200"
operator2 = "EE00D40C823060"
test1 = "8A004A801A8002F478"
test2 = "620080001611562C8802118E34"
test3 = "C0015000016115A2E0802F182340"
test4 = "A0016C880162017C3686B18A3D4780"

tests = [
    "C200B40A82",
    "04005AC33890",
    "880086C3E88112",
    "CE00C43D881120",
    "D8005AC2A8F0",
    "F600BC2D8F",
    "9C005AC2F8F0",
    "9C0141080250320F1802104A08",
]
contents = None
with open("data/input_day16.txt", "r") as fp:
    contents = fp.read()[:-1]

# print(repr(contents))

# analyze_binary_string(hex_to_binary(literal))
for test in [contents]:
    packages = []
    value, data = analyze_binary_string(hex_to_binary(test), 0, packages)

    # print(packages)
    print(value)

