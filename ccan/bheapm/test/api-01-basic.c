#include <ccan/array_size/array_size.h>
#include <ccan/bheapm/bheapm.h>
#include <ccan/tap/tap.h>

#include "helper_X.h"

int main(void)
{
	plan_tests(4);
	BHEAPM(struct X, bhn) bh;
	bheapm_init(&bh, X_cmp_bh);

	struct X foo[] = {{.v=50},{.v = 2},{.v=20},{.v = 4}, {.v = 1}, {.v=11}, {.v=5}};
	size_t i;
	for (i = 0; i < ARRAY_SIZE(foo); i++)
		bheapm_push(&bh, &foo[i]);
	struct X *foo_b = bheapm_pop(&bh);
	ok1(foo_b->v == 1);
	ok1(bheapm_pop(&bh)->v == 2);
	ok1(bheapm_pop(&bh)->v == 4);

	bheapm_push(&bh, &foo[1]);
	struct X *x = bheapm_pop(&bh);
	ok1(x->v == 2);

	return exit_status();
}
