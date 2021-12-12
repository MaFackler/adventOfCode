CC=gcc -g -Wall -Werror
all: day01 day02 day03 day04 day05 day06 day07 day08 day09

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

day06: src/day06.c src/common.h
	$(CC) $< -o $@

day07: src/day07.c src/common.h
	$(CC) $< -o $@

day08: src/day08.c src/common.h
	$(CC) $< -lm -o $@

day09: src/day09.c
	$(CC) $< -o $@

clean:
	rm day*
