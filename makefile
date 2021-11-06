CC=gcc

build:
	$(CC) -Werror -o computer.bin computer.c -lncurses
	$(CC) -Werror -o asm.bin asm.c -lncurses
