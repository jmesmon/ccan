#ifndef CCAN_BITOPS_H_
#define CCAN_BITOPS_H_

#include "config.h"
#include <ccan/ilog/ilog.h>

#include <stdint.h>

#define bit_mask_nz(bits) ((UINTMAX_C(1) << ((bits) - 1) << 1) - 1)
static inline uintmax_t bit_mask(unsigned bits)
{
	return bits ? bit_mask_nz(bits) : 0;
}

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
