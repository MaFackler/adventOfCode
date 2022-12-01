CC=gcc -g -Wall -Werror -lm
DAYS_C=$(wildcard src/day*.c)
DAYS_BIN=$(patsubst src/%.c,build/%,$(DAYS_C))


all: $(DAYS_BIN) | build

$(DAYS_BIN): $(DAYS_C)

build/%: src/%.c src/common.h
	$(CC) $< -o $@

build:
	mkdir -p $@

clean:
	rm build/*
