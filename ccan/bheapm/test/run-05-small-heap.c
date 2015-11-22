#include "../bheapm.c"

#include "helper_X.h"

#include <ccan/tap/tap.h>

int main(void)
{
	plan_tests(3);

	X_BHEAPM(h);
	struct X xs[2];
	xs[0].v = 10;
	xs[1].v = 3;

	ok1(bheapm_pop(&h) == NULL);

	bheapm_push(&h, &xs[1]);
	bheapm_push(&h, &xs[0]);

	ok1(bheapm_peek(&h) == &xs[1]);
	ok1(tcon_unwrap(&h)->top->d[0] == &xs[0].bhn);

	return exit_status();
}
