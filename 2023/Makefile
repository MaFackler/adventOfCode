CC=gcc -std=c99 -Wall -Werror -lm -I../libs/letsc/src
DAYS_C=$(wildcard src/day*.c)
DAYS_BIN=$(patsubst src/%.c,build/%,$(DAYS_C))


all: build $(DAYS_BIN)

# NOTE: define rule for each day so only the changed day is rebuild
define DAY_template =
$(1): $(patsubst build/%,src/%.c,$(1))
endef

$(foreach d,$(DAYS_BIN),$(eval $(call DAY_template, $(d))))

build/%: src/%.c
	$(CC) $< -o $@

build:
	mkdir -p $@

BIN=$(shell find build/ -type f | sort | tail -n1)
DATA=$(shell find data/*.txt -type f | sort | tail -n1)
DATA_TEST=$(shell find data/*e.txt -type f | sort | tail -n1)

.PHONY: run
tdd:
	@ls build/* | entr -c -r -s 'make --no-print-directory test'

.PHONY: test
test:
	@./$(BIN) < $(DATA_TEST)

.PHONY: latest
solve:
	@./$(BIN) < $(DATA) | xclip -sel clip
	@xclip -o -sel clip

clean:
	rm build/*
