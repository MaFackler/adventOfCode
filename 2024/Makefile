CC=g++ -g -std=c++23
DAYS_CPP=$(wildcard src/day*.cpp)
DAYS_BIN=$(patsubst src/%.cpp,build/%,$(DAYS_CPP))



all: build $(DAYS_BIN)

build:
	mkdir -p $@


build/%: src/%.cpp
	$(CC) $< -o $@


.PHONY: demo
demo: src/demo.cpp
	@$(CC) $< -o $@ && ./$@

entr:
	@echo src/demo.cpp | entr -c -r -s 'make --no-print-directory demo'



