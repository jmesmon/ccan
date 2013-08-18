#include <ccan/pp_ti/pp_ti.h>
#include <ccan/pp/pp.h>
#include <ccan/tap/tap.h>

typedef unsigned u;
typedef unsigned long ul;
typedef unsigned long long ull;

#define c1(bits) ok1(sizeof(CAT2(u,BITS_TO_INT(bits))) >= sizeof(uint##bits##_t))
#define c2(bits) ok1(INT_TO_BITS(BITS_TO_INT(bits)) >= bits)

#define Pa(td) diag("INT_TO_BITS(%s) = %s", STR(td), STR(INT_TO_BITS(td)))
#define Pb(bits) diag("BITS_TO_INT(%s) = %s", STR(bits), STR(BITS_TO_INT(bits)))

int main(void)
{
	plan_tests(8);

#define A(x) Pa(x);
#include "_int.h"
#undef A

#define A(x) do { \
	Pb(x);	\
	c1(x);	\
	c2(x);	\
} while (0);

#include "_bits.h"
#undef A

	return exit_status();
}
