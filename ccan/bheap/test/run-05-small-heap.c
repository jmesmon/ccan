#include "../bheap.c"

#include "helper_X.h"

#include <ccan/tap/tap.h>

int main(void)
{
	plan_tests(3);

	struct binary_heap h;
	struct X xs[2];
	xs[0].v = 10;
	xs[1].v = 3;

	bheap_init(&h, X_ord);

	ok1(bheap_pop(&h) == NULL);

	bheap_push(&h, &xs[1].bhn);
	bheap_push(&h, &xs[0].bhn);

	ok1(h.top == &xs[1].bhn);
	ok1(h.top->d[0] == &xs[0].bhn);

	return exit_status();
}
