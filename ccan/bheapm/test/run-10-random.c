#define CCAN_BHEAPM_DEBUG 1

#define do_it(x) do {		\
	do_test(x);		\
	ok(1, "do_test(" #x ")");\
} while(0)

#include "../bheapm.c"
#include "../bheapm_poison.h"
#include "../bheapm_check.h"

#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>
#include <ccan/container_of/container_of.h>

#include <stdlib.h>
#include <string.h>

#include "helper_X.h"

static void poison_node(struct bheapm_node *b, int i)
{
		b->d[0] = (void *)(uintptr_t)(2 * i + BH_POISON_NODE);
		b->d[1] = (void *)(uintptr_t)(2 * i + BH_POISON_NODE + 1);
}

static void do_test(int len)
{
	X_BHEAPM(bh);
	struct X x[len], xc[len];
	size_t i;
	int min = INT_MAX;
	for (i = 0; i < ARRAY_SIZE(x); i++) {
		x[i].v = rand() % (len * 4);
		poison_node(&x[i].bhn, i);

		if (x[i].v < min) {
			min = x[i].v;
		}


		bheapm_push(&bh, &x[i]);

		struct X *n = container_of(tcon_unwrap(&bh)->top, struct X, bhn);
		assert(n->v == min);

		bheapm_check_depth(&bh);
	}

	memcpy(xc, x, sizeof(xc));

	qsort(xc, ARRAY_SIZE(xc), sizeof(xc[0]), (__compar_fn_t) X_cmp);

	int max = INT_MIN;
	for (i = 0; i < ARRAY_SIZE(x); i++) {
		struct X *f = bheapm_pop(&bh);
		assert(f);
		struct X *next = bheapm_peek(&bh);
		if (max < f->v) {
			max = f->v;
		}
		if (next)
			assert(max <= next->v);
		assert(f->v == xc[i].v);
		bheapm_check_depth(&bh);
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
