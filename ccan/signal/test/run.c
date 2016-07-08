#include <ccan/signal/signal.h>
#include <ccan/tap/tap.h>

static int mult(double *ctx, double v)
{
	return *ctx * v;
}

static int add(int *x, double v)
{
	return *x + v;
}

int main(void)
{
	/* This is how many tests you plan to run */
	plan_tests(3);

	SIGNAL(int, double) notify = SIGNAL_INIT(notify);
	struct signal_connection *c[2], *i;

	ok1(c[0] = signal_connect(&notify, add, 3));
	ok1(c[1] = signal_connect(&notify, mult, 1.2));

	int r, a = 0;
	signal_for_each(&notify, 2, i, r)
		a += r;

	ok1(a == ((3 + 2) + (int)(1.2 * 2)));

	signal_drop(&notify);
	return exit_status();
}
