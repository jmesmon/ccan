#include <ccan/inline/inline.h>
#include <ccan/tap/tap.h>

int main(void)
{
	/* This is how many tests you plan to run */
	plan_tests(1);
	skip(1, "No tests, sorry");
	/* This exits depending on whether all tests passed */
	return exit_status();
}
