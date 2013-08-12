#define CCAN_BHEAP_DEBUG 1

#define do_it(x) do {		\
	do_test(x);		\
	ok(1, "do_test(" #x ")");\
} while(0)

#include <ccan/bheap/bheap.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>
#include <ccan/container_of/container_of.h>

#include <ccan/bheap/bheap.c>
#include <ccan/bheap/bheap_poison.h>
#include <ccan/bheap/bheap_check.h>

#include <stdlib.h>
#include <string.h>

#include "helper_X.h"

static void poison_node(struct bheap_node *b, int i)
{
		b->d[0] = (void *)(uintptr_t)(2 * i + BH_POISON_NODE);
		b->d[1] = (void *)(uintptr_t)(2 * i + BH_POISON_NODE + 1);
}

static void do_test(int len)
{
	struct bheap bh = BHEAP_INIT(X_ord);
	struct X x[len], xc[len];
	size_t i;
	int min = INT_MAX;
	for (i = 0; i < ARRAY_SIZE(x); i++) {
		x[i].v = rand() % (len * 4);
		poison_node(&x[i].bhn, i);

		if (x[i].v < min) {
			min = x[i].v;
		}


		bheap_push(&bh, &x[i].bhn);

		struct X *n = container_of(bh.top, struct X, bhn);
		assert(n->v == min);

		bheap_check_depth(&bh);
	}

	memcpy(xc, x, sizeof(xc));

	qsort(xc, ARRAY_SIZE(xc), sizeof(xc[0]), (__compar_fn_t) X_cmp);

	int max = INT_MIN;
	for (i = 0; i < ARRAY_SIZE(x); i++) {
		struct bheap_node *b = bheap_pop(&bh);
		assert(b);
		struct X *f = container_of(b, struct X, bhn);
		struct X *next = container_of(bh.top, struct X, bhn);
		if (max < f->v) {
			max = f->v;
		}
		if (next)
			assert(max <= next->v);
		assert(f->v == xc[i].v);
		bheap_check_depth(&bh);
	}
}

#define LEN 10
int main(void)
{
	plan_tests(3);
	do_it(1);
	do_it(10);
	do_it(300);
	return exit_status();
}
