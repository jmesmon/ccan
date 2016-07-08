#include <ccan/signal/signal.h>
#include <ccan/tap/tap.h>

static int mult(double *ctx)
{
	return *ctx * 2;
}

static int add(int *x)
{
	return *x + 2;
}

int main(void)
{
	/* This is how many tests you plan to run */
	plan_tests(3);

	SIGNAL0(int) notify = SIGNAL_INIT(notify);
	struct signal_connection *c[2], *i;

	ok1(c[0] = signal0_connect(&notify, add, 3));
	ok1(c[1] = signal0_connect(&notify, mult, 1.2));

	int r, a = 0;
	signal_for_each(&notify, i, r)
		a += r;

	ok1(a == ((3 + 2) + (int)(1.2 * 2)));

	signal_drop(&notify);
	return exit_status();
}
