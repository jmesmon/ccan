CFLAGS=-Wall -Werror -O3 -march=native -DNDEBUG -fno-inline-functions
ALL_CFLAGS=-I../../.. $(CFLAGS)

CC=gcc
LD=$(CC)
RM=rm -f

all: bench

bench : bench.c
	$(CC) $(ALL_CFLAGS) -o $@ $< -fwhole-program

clean:
	$(RM) bench *.o
