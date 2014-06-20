#ifndef CCAN_BITS_H_
#define CCAN_BITS_H_

#include <ccan/ilog/ilog.h>

#define DEF_RUP_P2(suffix, type)		\
static inline type round_up_pow2_##suffix(type i)	\
{						\
	if (!i)					\
		return 0;			\
	return 1U << ilog((type)(i-1));		\
}

DEF_RUP_P2(16, uint16_t);
DEF_RUP_P2(32, uint32_t);
DEF_RUP_P2(64, uint64_t);

#endif
