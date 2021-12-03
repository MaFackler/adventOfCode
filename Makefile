all: day01 day02 day03

day01: src/day01.c
	gcc $< -o $@

day02: src/day02.c
	gcc $< -o $@

day03: src/day03.c
	gcc $< -o $@

clean:
	rm day*
