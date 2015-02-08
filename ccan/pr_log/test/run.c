#include <ccan/pr_log/pr_log.h>
#include <ccan/pr_log/pr_log.c>
#include <ccan/tap/tap.h>

int main(void)
{
	plan_tests(6);
	ok1(!debug_is(4));
	debug = 1;
	ok1(debug_is(0));
	ok1(debug_is(1));
	ok1(!debug_is(2));
	ok1(!debug_is(3));

	pr_log("N: notice\n");
	pr_log("-3: something important\n");
	pr_log("1: less important\n");
	pr_log("X: bad level is treated as HIGH\n");

	debug = INT_MIN;
	setenv("DEBUG", "1", 1);
	ok1(debug_is(1));
	return exit_status();
}
