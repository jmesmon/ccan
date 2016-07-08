#include <ccan/signal/signal.h>
#include <ccan/tap/tap.h>

static void mult(int **a)
{
	**a *= 1.2;
}

static void add(int **a)
{
	**a += 2;
}

int main(void)
{
	/* This is how many tests you plan to run */
	plan_tests(3);

	SIGNAL0(void) notify = SIGNAL_INIT(notify);
	struct signal_connection *c[2];

	int a = 1;
	ok1(c[0] = signal0_connect(&notify, add, &a));
	ok1(c[1] = signal0_connect(&notify, mult, &a));

	signal_emit(&notify);
	ok1(a == (int)((1 + 2) * 1.2));

	signal_drop(&notify);
	return exit_status();
}
