/* CC0 (Public domain) - see LICENSE file for details */
#ifndef CCAN_MEM_H
#define CCAN_MEM_H

#include "config.h"

#include <string.h>
#include <stdbool.h>
#include <ccan/memmem/memmem.h>

/**
 * mempbrk - locates the first occurrence in @data of any bytes in @accept
 * @data: where we search
 * @len: length of data in bytes
 * @accept: array of bytes we search for
 * @accept_len: # of bytes in accept
 *
 * Returns a pointer to the byte in @data that matches one of the bytes in
 * @accept, or NULL if no such byte is found.
 *
 * Also see: strpbrk()
 *
 * Example:
 *	char somebytes[] = "HI";
 *	char otherbytes[] = "Hello world";
 *	size_t bytes_len, otherbytes_len;
 *	bytes_len = strlen(somebytes);
 *	otherbytes_len = strlen(otherbytes);
 *	char *r;
 *
 *	r = mempbrk(somebytes, bytes_len, otherbytes, otherbytes_len);
 *	if (r)
 *		printf("Found %c\n", *r);
 *	else
 *		printf("Nada\n");
 */
char *mempbrk(const char *data, size_t len, const char *accept, size_t accept_len);

/**
 * mempbrk_str - locates the first occurrence in @data of any bytes in @accept
 * @data: where we search
 * @len: length of data in bytes
 * @accept: NUL terminated string containing the bytes we search for
 *
 * Returns a pointer to the byte in @data that matches one of the bytes in
 * @accept, or NULL if no such byte is found.
 *
 * Example:
 *	r = mempbrks(somebytes, bytes_len, "world");
 *	if (r)
 *		printf("Found %c\n", *r);
 *	else
 *		printf("Nada\n");
 */
#define mempbrk_str(data, len, accept) mempbrk(data, len, accept, strlen(accept))

/**
 * memcchr - scan memory until a character does _not_ match @c
 * @data: pointer to memory to scan
 * @data_len: length of data
 * @c: character to scan for
 *
 * The compliment of memchr().
 *
 * Returns a pointer to the first character which is _not_ @c. If all memory in
 * @data is @c, returns NULL.
 *
 * Example:
 *	r = memcchr(somebytes, ' ', bytes_len);
 *	if (r)
 *		printf("Found %c after trimming spaces\n", *r);
 */
void *memcchr(void const *data, int c, size_t data_len);

/**
 * memstarts - determine if @data starts with @prefix
 * @data: does this begin with @prefix?
 * @data_len: bytes in @data
 * @prefix: does @data begin with these bytes?
 * @prefix_len: bytes in @prefix
 *
 * Returns true if @data starts with @prefix, otherwise return false.
 *
 * Example:
 *	if (memstarts(somebytes, bytes_len, otherbytes, otherbytes_len))
 *		printf("somebytes starts with otherbytes!\n");
 */
static inline bool memstarts(void const *data, size_t data_len,
		void const *prefix, size_t prefix_len)
{
	if (prefix_len > data_len)
		return false;
	return !memcmp(data, prefix, prefix_len);
}

/**
 * memeq - Are two byte arrays equal?
 * @a: first array
 * @al: bytes in first array
 * @b: second array
 * @bl: bytes in second array
 *
 * Example:
 *	if (memeq(somebytes, bytes_len, otherbytes, otherbytes_len))
 *		printf("memory blocks are the same!\n");
 */
#define memeq(a, al, b, bl) (al == bl && !memcmp(a, b, bl))

/**
 * memeqstr - Is a byte array equal to a NUL terminated string?
 * @bytes: byte array
 * @length: byte array length in bytes
 * @string: NUL terminated string
 *
 * The '\0' byte is ignored when checking if @bytes == @string.
 *
 * Example:
 *	if (memeqstr(somebytes, bytes_len, "foo"))
 *		printf("somebytes == 'foo'!\n");
 */
#define memeq_str(bytes, length, string) \
	memeq(bytes, length, string, strlen(string))

/**
 * memstarts_str - Does this byte array start with a string prefix?
 * @a: byte array
 * @al: length in bytes
 * @s: string prefix
 *
 * Example:
 *	if (memstarts_str(somebytes, bytes_len, "It"))
 *		printf("somebytes starts with 'It'\n");
 */
#define memstarts_str(a, al, s) memstarts(a, al, s, strlen(s))

/*
 * memspn
 * memcspn
 */

#endif /* CCAN_MEM_H */
