CC=gcc

build:
	$(CC) -Werror -o computer.out computer.c -lncurses
