#ifndef CCAN_INLINE_TEST_EXTRA_1_H_
#define CCAN_INLINE_TEST_EXTRA_1_H_
#include <ccan/inline/inline.h>

INLINE_FUNC(int foo(int a)) {
	return 2 * a;
}

#endif
