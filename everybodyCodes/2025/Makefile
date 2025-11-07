
all:
	$(MAKE) -C build

live:
	find src/ -type f | entr -c -r -s 'make --no-print-directory -C build && ./build/day01'
