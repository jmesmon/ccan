CFLAGS=-Wall -Werror -Os -march=native -ggdb3
ALL_CFLAGS=-I../../.. $(CFLAGS)

CC=gcc
LD=$(CC)
RM=rm -f

all: bench-x

bench-x : bench.c
	$(CC) $(ALL_CFLAGS) -o $@ $< -fwhole-program

clean:
	$(RM) bench *.o
