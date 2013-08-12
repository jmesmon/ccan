/* Licenced under LGPLv3+ - see LICENSE file for details */
#ifndef CCAN_BHEAP_PRINT_H_
#define CCAN_BHEAP_PRINT_H_

#include <ccan/bheap/bheap.h>
#include <stdio.h>

static inline void bheap_print(struct binary_heap *b, FILE *f)
{
	fprintf(f, "%c N=%zu\n", b->top ? '+' : '_', b->sz);
	if (b->sz == 0)
		return;

	size_t z;
	int l_ct = 1;
	for (z = 1; z < b->sz; l_ct <<= 1, z += l_ct) {
		int i;
		for (i = 0; i < l_ct; i++) {
			putc('+', f);
		}
		putc('\n', f);
	}
}

typedef int (*binary_heap_node_to_str_fn)(struct binary_heap_node *b, char *str, size_t l);

static inline void bheap_print_horz(struct binary_heap_node *b, int depth, binary_heap_node_to_str_fn to_str, FILE *f)
{
	unsigned i;
	for (i = 0; i < depth; i++)
		putc(' ', f);

	char buf[32];
	if (b) {
		to_str(b, buf, sizeof(buf));
		fputs(buf, f);
		putc('\n', f);
		bheap_print_horz(b->d[0], depth+1, to_str, f);
		bheap_print_horz(b->d[1], depth+1, to_str, f);
	} else {
		fputs("(null)\n", f);
	}
}

#endif
