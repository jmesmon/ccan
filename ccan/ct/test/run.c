#include <ccan/ct/ct.h>
#include <ccan/tap/tap.h>

enum {
	e_int,
	e_double,
};

static int _int(int x)
{
	return e_int;
}

static int _double(double x)
{
	return e_double;
}

#define X(x) \
	C_CHOOSE(VAR_COMPAT_TYPE(x, double), _double(x),	\
	C_CHOOSE(VAR_COMPAT_TYPE(x, int), _int(x),		\
	C_CHOOSE_END))

int main(void)
{
	plan_tests(2);
	ok1(X(1) == e_int);
	ok1(X(1.) == e_double);
	return exit_status();
}
