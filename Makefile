CC=gcc -g -Wall -Werror
all: day01 day02 day03 day04 day05

day01: src/day01.c
	$(CC) $< -o $@

day02: src/day02.c
	$(CC) $< -o $@

day03: src/day03.c
	$(CC) $< -o $@

day04: src/day04.c
	$(CC) $< -o $@

day05: src/day05.c
	$(CC) $< -o $@

clean:
	rm day*
