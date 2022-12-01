CC=gcc -g -Wall -Werror -lm
DAYS_C=$(wildcard src/day*.c)
DAYS_BIN=$(patsubst src/%.c,build/%,$(DAYS_C))


all: build $(DAYS_BIN)

$(DAYS_BIN): $(DAYS_C)

build/%: src/%.c
	$(CC) $< -o $@

build:
	mkdir -p $@

clean:
	rm build/*
