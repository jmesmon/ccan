#include <ccan/pp/pp.h>
#include <ccan/tap/tap.h>
#include <ccan/str/str.h>

int main(void)
{
	plan_tests(1);

	ok1(streq(STR(EMPTY), ""));
	return exit_status();
}
