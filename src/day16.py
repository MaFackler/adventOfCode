

def hex_to_binary(s: str):
    res = ""
    for c in s:
        chunk = str(bin(int(c, 16)))[2:]
        chunk = chunk.zfill(4)
        res += chunk
    return res

def bin_to_dec(s: str):
    return int(s, 2)


def analyze_binary_string(s: str, packages):
    if not s:
        return
    # print("\nGot package", s)
    version = bin_to_dec(s[:3])
    typeid = bin_to_dec(s[3:6])
    # print("Version", version)
    # print("Typeid", typeid)

    packages.append((version, typeid))
    if typeid == 4:
        # literal value
        # print("Literal")

        chunk_size = 5
        i = 6
        package = ""
        while i < len(s):
            chunk = s[i:i + chunk_size]
            i += chunk_size
            package += chunk[1:]

            if chunk[0] == "0":
                break

        value = bin_to_dec(package)
        # print("GOT VALUE", value)
        if "1" in s[i:]:
            return analyze_binary_string(s[i:], packages)
    else:
        # print("Found Operator")
        if s[6] == "0":
            # print("15 OPERATOR")
            bits_to_parse = 15
            len_subpackages = bin_to_dec(s[7:7 + bits_to_parse])
            start = 7 + bits_to_parse
            data = s[start:]
            while True:
                data = analyze_binary_string(data, packages)
                if not data:
                    break
        else:
            # print("11 OPERATOR")
            bits_to_parse = 11
            amount_subpackages = bin_to_dec(s[7:7 + bits_to_parse])
            # print("FOUND SUBPACKES", amount_subpackages)
            data = s[7 + bits_to_parse:]
            for _ in range(0, amount_subpackages):
                data = analyze_binary_string(data, packages)




    
literal = "D2FE28"
operator = "38006F45291200"
operator2 = "EE00D40C823060"
test1 = "8A004A801A8002F478"
test2 = "620080001611562C8802118E34"
test3 = "C0015000016115A2E0802F182340"
test4 = "A0016C880162017C3686B18A3D4780"
contents = None
with open("data/input_day16.txt", "r") as fp:
    contents = fp.read()[:-1]


# analyze_binary_string(hex_to_binary(literal))
for test in (contents,):
    packages = []
    analyze_binary_string(hex_to_binary(test), packages)
    # print(packages)
    print(sum((a[0] for a in packages)))
