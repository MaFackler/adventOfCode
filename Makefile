CC=gcc -Werror -w
all: day01 day02 day03

day01: src/day01.c
	$(CC) $< -o $@

day02: src/day02.c
	$(CC) $< -o $@

day03: src/day03.c
	$(CC) -g $< -o $@

clean:
	rm day*
