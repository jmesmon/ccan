#include <ccan/array_size/array_size.h>
#include <ccan/bheap/bheap.h>
#include <ccan/tap/tap.h>

#include "helper_X.h"

int main(void)
{
	plan_tests(4);
	total_order_by_field(x_ord, int, struct X, v);
	BHEAP(struct X, bhn) bh;
	bheap_init(&bh, x_ord.cb, x_ord.ctx);

	struct X foo[] = {{.v=50},{.v = 2},{.v=20},{.v = 4}, {.v = 1}, {.v=11}, {.v=5}};
	size_t i;
	for (i = 0; i < ARRAY_SIZE(foo); i++)
		bheap_push(&bh, &foo[i]);
	struct X *foo_b = bheap_pop(&bh);
	ok1(foo_b->v == 1);
	ok1(bheap_pop(&bh)->v == 2);
	ok1(bheap_pop(&bh)->v == 4);

	bheap_push(&bh, &foo[1]);
	struct X *x = bheap_pop(&bh);
	ok1(x->v == 2);

	return exit_status();
}
