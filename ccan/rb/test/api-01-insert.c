#include <ccan/rb/rb.h>
#include <ccan/array_size/array_size.h>
#include <ccan/tap/tap.h>

#include <string.h>

#include "helper_X.h"

int main(void)
{
	struct X foo[] = {{.v = "a"},{.v = "x"},{.v= "y"},{.v = "c"}, {.v = "b"}, {.v="n"}, {.v="g"}};
	plan_tests(1 + ARRAY_SIZE(foo) + 1);

	COLL_DECL(coll);

	struct X foo_ordered[ARRAY_SIZE(foo)];
	memcpy(foo_ordered, foo, sizeof(foo));
	qsort(foo_ordered, ARRAY_SIZE(foo_ordered), sizeof(foo_ordered[0]), (__compar_fn_t)X_cmp);

	size_t i;
	for (i = 0; i < ARRAY_SIZE(foo); i++) {
		rb_insert_ord(&coll, &foo[i].e, X_cmp_elem);
		rb_assert(&coll);
	}

	X_print_tree("inserted", &coll, stdout);

	size_t j;
	struct rb_node *n;
	for (j = 0, n = rb_first(&coll); n && (j < ARRAY_SIZE(foo)); n = rb_next(n), j++) {
		struct X *v = elem_to_X(n);
		ok_eq(v->v, foo_ordered[j].v);
	}

	ok1(n == NULL);
	ok1(i == j);

	rb_assert(&coll);

	return exit_status();
}
