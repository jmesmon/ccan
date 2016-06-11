#define CCAN_RB_DEBUG 1
#include <ccan/rb/rb.h>
#include <ccan/array_size/array_size.h>
#include <ccan/tap/tap.h>

#include <ccan/rb/rb.c>

#include <string.h>

#include "helper_X.h"

int main(void)
{
	struct X foo[] = {{.v = "a"},{.v = "x"},{.v= "y"},{.v = "c"}, {.v = "b"}, {.v="n"}, {.v="g"}};
	plan_tests(1);

	COLL_DECL(coll);

	struct X foo_ordered[ARRAY_SIZE(foo)];
	memcpy(foo_ordered, foo, sizeof(foo));
	qsort(foo_ordered, ARRAY_SIZE(foo_ordered), sizeof(foo_ordered[0]), (__compar_fn_t)X_cmp);

	size_t i;
	for (i = 0; i < ARRAY_SIZE(foo); i++) {
		rb_insert_ord(&coll, &foo[i].e, X_cmp_elem);
		rb_assert_tree(&coll);
		X_print_tree("inserted_1", &coll, stdout);
		rb_assert(&coll);
	}

	X_print_tree("inserted", &coll, stdout);

	/* check if we hit all the nodes */

	size_t j;
	char name[256];
	for (j = 0; j < ARRAY_SIZE(foo); j++) {
		rb_remove(&coll, &foo[j].e);
		sprintf(name, "remove_%s", foo[j].v);
		rb_assert_tree(&coll);
		X_print_tree(name, &coll, stdout);
		struct rb_node *x;
		for (x = rb_first(&coll); x; x = rb_next(x)) {
			struct X *t = elem_to_X(x);
			assert_ne_s(t->v, foo[j].v);
		}
		rb_assert(&coll);
	}

	rb_assert(&coll);

	skip(1, "all tests are via assert");
	return exit_status();
}
