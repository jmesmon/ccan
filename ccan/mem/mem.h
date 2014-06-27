/* CC0 (Public domain) - see LICENSE file for details */
#ifndef CCAN_MEM_H
#define CCAN_MEM_H

#include "config.h"

#include <string.h>

#if !HAVE_MEMMEM
void *memmem(const void *haystack, size_t haystacklen,
	     const void *needle, size_t needlelen);
#endif

/**
 * mempbrkm - locates the first occurrence in @data of any bytes in @accept
 * @data: where we search
 * @len: length of data in bytes
 * @accept: array of bytes we search for
 * @accept_len: # of bytes in accept
 *
 * Returns a pointer to the byte in @data that matches one of the bytes in
 * @accept, or NULL if no such byte is found.
 */
char *mempbrkm(const char *data, size_t len, const char *accept, size_t accept_len);

/**
 * mempbrk - locates the first occurrence in @data of any bytes in @accept
 * @data: where we search
 * @len: length of data in bytes
 * @accept: NUL terminated string containing the bytes we search for
 *
 * Returns a pointer to the byte in @data that matches one of the bytes in
 * @accept, or NULL if no such byte is found.
 */
#define mempbrk(data, len, accept) mempbrkm(data, len, accept, strlen(accept))

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
 */
static inline bool memstarts(void const *data, size_t data_len,
		void const *prefix, size_t prefix_len)
{
	if (prefix_len > data_len)
		return false;
	return !memcmp(data, prefix, prefix_len);
}

/**
 * memeq -
 */
#define memeq(a, al, b, bl) (al == bl && !memcmp(a, b, bl))

/**
 * memeqstr -
 */
#define memeqstr(bytes, length, string) \
	memeq(bytes, length, string, strlen(string))

/*
 * memstarts_str -
 */
#define memstarts_str(a, al, s) memstarts(a, al, s, strlen(s))

#endif /* CCAN_MEM_H */
