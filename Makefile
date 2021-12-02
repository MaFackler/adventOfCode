all: day01 day02

day01: src/day01.c
	gcc $< -o $@

day02: src/day02.c
	gcc -g $< -o $@

clean:
	rm day*
