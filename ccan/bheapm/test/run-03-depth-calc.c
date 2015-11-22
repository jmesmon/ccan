
/* N=1  + D=1   -- parent_stack = +
 *
 * N=2  + D=2   -- parent_stack = +
 *     /			  |
 *    +				  +
 *
 * N=3  +  D=2   (log2(N + 1) = 2 +
 *     / \			  |
 *    +   +			  +
 *
 * N=4  + D=3 (log2(N+1) = 2.321  +(0, points into the struct binary_heap)
 *     / \			  |
 *    +   +			  +(1)
 *   /				  |
 *  +				  +(2)
 *
 * N=5 + D=3 (log2(N+1) = 2.584  +
 *    / \			 |
 *   +   +			 +
 *  / \				 |
 * +   +			 +
 *
 * N=7 + D=3 (log2(N + 1) = 3)
 *    / \
 *   +   +
 *  / \  |\
 * +   + + +
 *
 * N=6, D=3
 * N=7, D=3
 * N=8, D=4
 * N=9-15 D=4
 *
 * Level @l of a tree has nodes L(l) = 2**(l-1)
 * A tree with @l levels has nodes T(l) = 2**l
 * A tree with @n nodes as depth = ceil(log2(n+1))
 *	If the lowest level is full, the value is `log2(n+1)`
 *	In other words: it is exact.
 *
 *
 */
#include <ccan/ilog/ilog.h>
#include <ccan/tap/tap.h>

#include <stdbool.h>

#include "../bheapm.c"

#define ok_eq(a, b) ok_eq_(false, a, #a, b, #b)
#define bad_eq(a, b) ok_eq_(true, a, #a, b, #b)
static void ok_eq_(bool inv, size_t a, const char *as, size_t b, const char *bs)
{
	ok(inv ? a != b : a == b,
		"%s <%zu> %s <%zu> %s",
		as, a, inv ? "!=" : "==", b, bs);
}

int main(void)
{
	plan_tests(16);

	ok_eq(bheapm_depth(1), 1);

	ok_eq(bheapm_depth(2), 2);
	ok_eq(bheapm_depth(3), 2);

	ok_eq(bheapm_depth(4), 3);
	ok_eq(bheapm_depth(5), 3);
	ok_eq(bheapm_depth(6), 3);
	ok_eq(bheapm_depth(7), 3);

	ok_eq(bheapm_depth(8), 4);
	ok_eq(bheapm_depth(9), 4);
	ok_eq(bheapm_depth(10), 4);
	ok_eq(bheapm_depth(11), 4);
	ok_eq(bheapm_depth(12), 4);
	ok_eq(bheapm_depth(13), 4);
	ok_eq(bheapm_depth(14), 4);
	ok_eq(bheapm_depth(15), 4);

	ok_eq(bheapm_depth(16), 5);

	return exit_status();
}
