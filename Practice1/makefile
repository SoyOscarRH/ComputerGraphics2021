CC=gcc
CFLAGS=-std=c11 -Wall -Wshadow -pedantic -O2 -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=address -fsanitize=integer-divide-by-zero -fsanitize=float-divide-by-zero -fsanitize=pointer-overflow -fsanitize=shift-exponent 

buid:
	$(CC) $(CFLAGS) main.c ../lib/line.c -o practice1
