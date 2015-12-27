#include <ccan/tap/tap.h>
#include "../rb.c"
#include "helper_X.h"

static
void rot_left(void)
{
	/*
	 * x: the parent node
	 * right: if true, rotate right, if false rotate left
	 *
	 * y is the child to the right of x
	 *
	 *     y         x
	 *    / \       / \
	 *   x   c     a   y
	 *  / \           / \
	 * a   b         b   c
	 *
	 *          <- left rotate
	 *          -> right rotate
	 */

	struct X a, b, x, y, c;

	a.v = "a";
	a.e.c[0] = a.e.c[1] = NULL;
	a.e.parent_and_color = (uintptr_t)&x;

	b.v = "b";
	b.e.c[0] = b.e.c[1] = NULL;
	b.e.parent_and_color = (uintptr_t)&y;

	c.v = "c";
	c.e.c[0] = c.e.c[1] = NULL;
	c.e.parent_and_color = (uintptr_t)&y;

	x.v = "x";
	x.e.c[0] = &a.e;
	x.e.c[1] = &y.e;
	x.e.parent_and_color = 0;

	y.v = "y";
	y.e.c[0] = &b.e;
	y.e.c[1] = &c.e;
	y.e.parent_and_color = (uintptr_t)&x;

	struct rb_tree t = { &x.e };

	rb_assert_parents(t.top, NULL);
	rb_rotate(&t, &x.e, false);
	rb_assert_parents(t.top, NULL);

	ok1(t.top == &y.e);

	ok1(y.e.c[0] == &x.e);
	ok1(y.e.c[1] == &c.e);
	ok1(x.e.c[0] == &a.e);
	ok1(x.e.c[1] == &b.e);

	ok1(rb_is_leaf(&a.e));
	ok1(rb_is_leaf(&b.e));
	ok1(rb_is_leaf(&c.e));

}

static void rot_right(void)
{
	/*
	 * y: the parent node
	 * right: if true, rotate right, if false rotate left
	 *
	 * y is the child to the right of x
	 *
	 *     y         x
	 *    / \       / \
	 *   x   c     a   y
	 *  / \           / \
	 * a   b         b   c
	 *
	 *          <- left rotate
	 *          -> right rotate
	 */
	struct X a, b, x, y, c;

	a.v = "a";
	a.e.c[0] = a.e.c[1] = NULL;
	a.e.parent_and_color = (uintptr_t)&x;

	b.v = "b";
	b.e.c[0] = b.e.c[1] = NULL;
	b.e.parent_and_color = (uintptr_t)&x;

	c.v = "c";
	c.e.c[0] = c.e.c[1] = NULL;
	c.e.parent_and_color = (uintptr_t)&y;

	x.v = "x";
	x.e.c[0] = &a.e;
	x.e.c[1] = &b.e;
	x.e.parent_and_color = (uintptr_t)&y;

	y.v = "y";
	y.e.c[0] = &x.e;
	y.e.c[1] = &c.e;
	y.e.parent_and_color = 0;

	struct rb_tree t = { &y.e };

	rb_assert_parents(t.top, NULL);
	rb_rotate(&t, &y.e, true);
	rb_assert_parents(t.top, NULL);

	ok1(t.top == &x.e);

	ok1(x.e.c[0] == &a.e);
	ok1(x.e.c[1] == &y.e);
	ok1(y.e.c[0] == &b.e);
	ok1(y.e.c[1] == &c.e);

	ok1(rb_is_leaf(&a.e));
	ok1(rb_is_leaf(&b.e));
	ok1(rb_is_leaf(&c.e));
}

int main(void)
{
	plan_tests(8*2);

	rot_left();
	rot_right();

	return exit_status();
}
