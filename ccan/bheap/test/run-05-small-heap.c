#include "../bheap.c"

#include "helper_X.h"

#include <ccan/tap/tap.h>

int main(void)
{
	plan_tests(3);

	X_BHEAP(h);
	struct X xs[2];
	xs[0].v = 10;
	xs[1].v = 3;

	ok1(bheap_pop(&h) == NULL);

	bheap_push(&h, &xs[1]);
	bheap_push(&h, &xs[0]);

	ok1(bheap_peek(&h) == &xs[1]);
	ok1(tcon_unwrap(&h)->top->d[0] == &xs[0].bhn);

	return exit_status();
}
