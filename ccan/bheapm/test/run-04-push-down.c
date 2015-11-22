#include "../bheapm.c"
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>
#include <ccan/bheapm/bheapm_poison.h>

#include "helper_X.h"

#define N(i) (struct X){ .bhn = BHN_P(i), .v = i }

#define oke(e, fmt, ...) ((e) ?	\
		_gen_result(1, __func__, __FILE__, __LINE__, "%s : "  fmt, #e, __VA_ARGS__) :\
		_gen_result(0, __func__, __FILE__, __LINE__, "%s : "  fmt, #e, __VA_ARGS__))

/*
 *           7
 *          / \
 *         /   \
 *        2     1
 *       / \   / \
 *      4   5 6   3
 *
 *      to
 *
 *           1
 *          / \
 *         /   \
 *        2     3
 *       / \   / \
 *      4   5 6   7
 *
 *      But instead we get:
 *
 *         1
 *        / \
 *       /   \
 *      2     7
 *     / \
 *    4   5
 *
 *      3
 *     / \
 *    6   7
 *
 */


#define ok_eq(n1, n2) oke(n1 == n2, "%p (%zd) == %p (%zd)", n1, n1 ? (bhn_to_X(n1) - z) : -1, n2, n2 ? (bhn_to_X(n2) - z) : -1)
#define Z(x) z[x - 1].bhn


static void do_it(void)
{
	struct X z[7];
	size_t i;
	for (i = 0; i < ARRAY_SIZE(z); i++)
		z[i] = N(i);


	Z(7).d[0] = &Z(2);
		Z(2).d[0] = &Z(4);
			Z(4).d[0] = NULL;
			Z(4).d[1] = NULL;
		Z(2).d[1] = &Z(5);
			Z(5).d[0] = NULL;
			Z(5).d[1] = NULL;
	Z(7).d[1] = &Z(1);
		Z(1).d[0] = &Z(6);
			Z(6).d[0] = NULL;
			Z(6).d[1] = NULL;
		Z(1).d[1] = &Z(3);
			Z(3).d[0] = NULL;
			Z(3).d[1] = NULL;

	X_BHEAPM(b);
	tcon_unwrap(&b)->top = &Z(7);
	tcon_unwrap(&b)->sz = ARRAY_SIZE(z);

	push_down(tcon_unwrap(&b));

	ok_eq(tcon_unwrap(&b)->top, &Z(1));
	ok_eq(Z(1).d[0], &Z(2));
		ok_eq(Z(2).d[0], &Z(4));
			ok_eq(Z(4).d[0], NULL);
			ok_eq(Z(4).d[1], NULL);
		ok_eq(Z(2).d[1], &Z(5));
			ok_eq(Z(5).d[0], NULL);
			ok_eq(Z(5).d[1], NULL);
	ok_eq(Z(1).d[1], &Z(3));
		ok_eq(Z(3).d[0], &Z(6));
			ok_eq(Z(6).d[0], NULL);
			ok_eq(Z(6).d[1], NULL);
		ok_eq(Z(3).d[1], &Z(7));
			ok_eq(Z(7).d[0], NULL);
			ok_eq(Z(7).d[1], NULL);
}

int main(void)
{
	plan_tests(1 + 2 + 4 + 8);
	do_it();
	return exit_status();
}
