#include "../bheap.c"

#include <ccan/tap/tap.h>
#include <ccan/bheap/bheap_poison.h>

#define P(x) BHN_P(x)

static void do_it(bool d)
{
	struct binary_heap_node *p1_slot;
	struct binary_heap_node n1 = P(0) , n2 = P(1), n1c = P(2), n2c1 = P(3), n2c2 = P(4);

	p1_slot  = &n1;
	n1.d[d]  = &n2;
	n1.d[!d] = &n1c;
	n2.d[0]  = &n2c1;
	n2.d[1]  = &n2c2;

	swap_node_with_parent(&n1, &n2, &p1_slot);

	ok1(p1_slot  == &n2);
	ok1(n2.d[d]  == &n1);
	ok1(n2.d[!d] == &n1c);
	ok1(n1.d[0]  == &n2c1);
	ok1(n1.d[1]  == &n2c2);
}

int main(void)
{
	plan_tests(10);

	do_it(1);
	do_it(0);

	return exit_status();
}
