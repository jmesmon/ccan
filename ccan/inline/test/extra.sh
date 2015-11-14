#! /bin/sh -x

: ${CC:=gcc}
: ${CFLAGS:=-Wall -Wextra}
cd "$(dirname "$0")"

$CC $CFLAGS extra-1.c -I ../../../ -c -o extra-1.o
$CC $CFLAGS extra-1-main.c -I ../../../ -c -o extra-1-main.o
$CC $CFLAGS extra-1.o extra-1-main.o -o extra-1

