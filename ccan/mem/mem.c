/* CC0 (Public domain) - see LICENSE file for details */

#include "config.h"

#include <string.h>
#include <ccan/mem/mem.h>

#if !HAVE_MEMMEM
void *memmem(const void *haystack, size_t haystacklen,
	     const void *needle, size_t needlelen)
{
	const char *p;

	if (needlelen > haystacklen)
		return NULL;

	p = haystack;

	for (p = haystack;
	     (p + needlelen) <= ((const char *)haystack + haystacklen);
	     p++)
		if (memcmp(p, needle, needlelen) == 0)
			return (void *)p;

	return NULL;
}
#endif

char *mempbrkm(const char *data, size_t len, const char *accept, size_t accept_len)
{
	size_t i, j;
	for (i = 0; i < len; i++)
		for (j = 0; j < accept_len; j++)
			if (accept[j] == data[i])
				return (void *)&data[i];
	return NULL;
}

void *memcchr(void const *data, int c, size_t data_len)
{
	char const *p = data;
	while((size_t)(p - ((char const *)data)) < data_len) {
		if (*p != c)
			return (char *)p;
		p++;
	}

	return NULL;
}
