#include <ccan/bheap/bheap.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

#include "helper_X.h"

int main(void)
{
	plan_tests(4);
	struct bheap bh;
	bheap_init(&bh, X_ord);

	struct X foo[] = {{.v=50},{.v = 2},{.v=20},{.v = 4}, {.v = 1}, {.v=11}, {.v=5}};
	size_t i;
	for (i = 0; i < ARRAY_SIZE(foo); i++)
		bheap_push(&bh, &foo[i].bhn);
	struct bheap_node *foo_b = bheap_pop(&bh);
	ok1(bhn_to_X(foo_b)->v == 1);
	ok1(bhn_to_X(bheap_pop(&bh))->v == 2);
	ok1(bhn_to_X(bheap_pop(&bh))->v == 4);

	bheap_push(&bh, &foo[1].bhn);
	foo_b = bheap_pop(&bh);
	struct X *x = bhn_to_X(foo_b);
	ok1(x->v == 2);

	return exit_status();
}
