
#include <ccan/maskn/maskn.c>
#include <ccan/tap/tap.h>

#define ok_eq(a, b) ok((a) == (b), "%s (%#jx) != %s (%#jx)", #a, (uintmax_t)(a), #b, (uintmax_t)(b))
int main(void)
{
	plan_tests(17);

	ok_eq(0, maskn_from_range_low(0xfff1, 0xfff1));

#define MFRL(base, max) maskn_base(base, maskn_from_range_low(base, max)), maskn_from_range_low(base, max)
	ok1( maskn_matches(MFRL(0xfff0, 0xfff1), 0xfff1));
	ok1(!maskn_matches(MFRL(0xfff0, 0xfff1), 0xfff2));
	ok1(!maskn_matches(MFRL(0xfff1, 0xfff1), 0xfff2));
	ok1(!maskn_matches(MFRL(0xfff1, 0xfff1), 0xfff0));

	ok_eq(4,	maskn_from_range_low(0xffe0, 0xfff0));

	ok_eq(5,	maskn_from_range_low(0xffe0, 0xffff));
	ok_eq(1,	maskn_from_range_low(0xffe0, 0xffe1));

	ok_eq(16,	maskn_from_range_high(0xffff, 0));
	ok_eq(15,	maskn_from_range_high(0xffff, 1));
	ok_eq(0,	maskn_from_range_high(0xfffe, 1));

	ok_eq(32,	maskn_from_range_low(0, UINT32_MAX));

	ok_eq(0,	maskn_from_range(0, 0));
	ok_eq(32,	maskn_from_range(0, UINT32_MAX));
	ok_eq(32,	maskn_from_range(UINT32_MAX, 0));

	ok_eq(0,	maskn_from_range(UINT32_MAX & ~INT32_C(1), 0));
	ok_eq(31,	maskn_from_range(UINT32_MAX >> 1, 0));

	return exit_status();
}
