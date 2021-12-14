CC=gcc

build:
	$(CC) -Werror -o computer computer.c -lncurses
	$(CC) -Werror -o asm asm.c -lncurses
