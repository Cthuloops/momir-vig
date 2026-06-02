CC = gcc
CFLAGS = -g -Wall -Werror -pedantic -std=c99 -fsanitize=undefined -O0 -I./src

vig: main.c src/printer.c
	$(CC) $(CFLAGS) main.c src/printer.c -o vig


clean:
	rm -f vig

.PHONY: clean
