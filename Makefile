SRC = main.c lexer.c stack.c queue.c
CC = cc
DEBUG_CFLAGS = -g -O0 -Wall -Wpedantic
CFLAGS = -O2 -Wall -Wpedantic -march=native

debug:
	$(CC) $(DEBUG_CFLAGS) -o mathparser $(SRC)

release:
	$(CC) $(CFLAGS) -o mathparser $(SRC)

clean:
	rm -f mathparser

.PHONY: clean
