#include <ccan/bheap/bheap.h>
#include <ccan/tap/tap.h>

#include "helper_X.h"

int main(void)
{
	plan_tests(1);
	struct binary_heap bh;
	bheap_init(&bh, X_ord);

	struct X foo = {
		.v = 2
	};

	bheap_push(&bh, &foo.bhn);
	struct binary_heap_node *foo_b = bheap_pop(&bh);
	ok1(foo_b == &foo.bhn);

	return exit_status();
}
