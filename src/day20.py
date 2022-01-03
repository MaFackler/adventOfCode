

def iter_rows(data: set, margin: int):
    row_min = min((row for row, _ in data))
    row_max = max((row for row, _ in data))
    for row in range(row_min - margin, row_max + (2 * margin)):
        yield row_min, row, row_max

def iter_cols(data: set, margin: int):
    col_min = min((col for _, col in data))
    col_max = max((col for _, col in data))
    for col in range(col_min - margin, col_max + margin):
        yield col_min, col, col_max



def enhance(data: set, pixels: str, light_pixels_stored: bool):
    light = set()
    dark = set()
    margin = 5
    for row_min, row, row_max in iter_rows(data, margin):
        for col_min, col, col_max in iter_cols(data, margin):
            index_string = ""
            for row_offset in (-1, 0, 1):
                for col_offest in (-1, 0, 1):
                    lookup = (row + row_offset, col + col_offest)
                    index_string += "1" if (lookup in data) == light_pixels_stored else "0"
            assert len(index_string) == 9
            index = int(index_string, 2)
            if pixels[index] == "#":
                light.add((row, col))
            else:
                dark.add((row, col))

    return light, dark


def print_image(data: set):
    for _, row, _ in iter_rows(data, 0):
        for _, col, _ in iter_cols(data, 0):
            print("#" if (row, col) in data else ".", end="")
        print()


def main():
    with open("data/input_day20.txt", "r") as fp:
        contents = fp.read().strip()

    pixels, buffer = contents.split("\n\n")
    width = len(buffer.split("\n")[0])
    assert len(pixels) == 512, len(pixels)


    data = set()
    for row, line in enumerate(buffer.split("\n")):
        for col, c in enumerate(line):
            if c == "#":
                data.add((row, col))

    light_pixels_stored = True
    for i in range(0, 50):
        light, dark = enhance(data, pixels, light_pixels_stored=light_pixels_stored)

        inverse = False
        if pixels[0] == ".":
            data = light
        else:
            # NOTE: (i + 1) because this will be the next iteration
            light_pixels_stored = (i + 1) % 2 == 0
            data = light if light_pixels_stored else dark
        if i == 1:
            print(len(data))

    print(len(data))


main()
