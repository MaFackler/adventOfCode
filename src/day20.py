with open("data/input_day20.txt", "r") as fp:
    contents = fp.read().strip()


def _get_buffer(in_buffer, row, col):
    res = None
    if row >= 0 and row < len(in_buffer):
        r = in_buffer[row]
        if col >= 0 and col < len(in_buffer[row]):
            res = in_buffer[row][col]
    return res


def get_index(in_buffer, row, col):
    buf = ['-', ] * 9
    buf[0] = _get_buffer(in_buffer, row - 1, col - 1) or "."
    buf[1] = _get_buffer(in_buffer, row - 1, col) or "."
    buf[2] = _get_buffer(in_buffer, row - 1, col + 1) or "."
    buf[3] = _get_buffer(in_buffer, row, col - 1) or "."
    buf[4] = _get_buffer(in_buffer, row, col) or "."
    buf[5] = _get_buffer(in_buffer, row, col + 1) or "."
    buf[6] = _get_buffer(in_buffer, row + 1, col - 1) or "."
    buf[7] = _get_buffer(in_buffer, row + 1, col) or "."
    buf[8] = _get_buffer(in_buffer, row + 1, col + 1) or "."
    assert "-" not in buf
    out = ["1" if c == "#" else "0" for c in buf]
    return int("".join(out), 2)


def enhance(contents: str, pixels):
    width = len(contents.split("\n")[0])
    margin = 5
    width += (margin * 2)
    buffer = []
    for i in range(0, margin):
        buffer.append([".",] * width)

    for line in contents.split("\n"):
        assert line, repr(line)
        l = ([".",] * margin) + list(line) + ([".",] * margin)
        buffer.append(l)

    for i in range(0, margin):
        buffer.append([".",] * width)


    out = ""
    res = 0
    for row, line in enumerate(buffer):
        outline = []
        for col, c in enumerate(line):
            i = get_index(buffer, row, col)
            p = pixels[i]
            outline.append(p)
            if p == "#" and col > margin and col < width - margin - 1:
                if row > margin and row < len(buffer) - margin - 1:
                    res += 1
        out += "".join(outline) + "\n"

    return (res, out[:-1])


# NOTE: this code is bad. It buffers the whole image
# better way is to just store the light pixels in a set.
# Will be done in part two
pixels, buffer = contents.split("\n\n")
width = len(buffer.split("\n")[0])
assert len(pixels) == 512, len(pixels)

res = 0
for i in range(0, 2):
    res, buffer = enhance(buffer, pixels)

print(res)

