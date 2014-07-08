#ifndef CCAN_MASKN_H_
#define CCAN_MASKN_H_
/* LGPLv3 or later - see LICENSE file for details */
#include "config.h"
#include <ccan/compiler/compiler.h>
#include <stdint.h>
#include <stdbool.h>

/* assert(bits > 0) */
#define bit_mask_nz(bits) ((UINTMAX_C(1) << ((bits) - 1) << 1) - 1)

static inline uintmax_t bit_mask(unsigned bits)
{
	return bits ? bit_mask_nz(bits) : 0;
}

unsigned maskn_from_range_low(uint32_t base, uint32_t max) CONST_FUNCTION;
unsigned maskn_from_range_high(uint32_t base, uint32_t min) CONST_FUNCTION;
unsigned maskn_from_range(uint32_t base, uint32_t limit) CONST_FUNCTION;

static inline bool maskn_matches(uint32_t base, unsigned mask_bits, uint32_t v)
{
	uint32_t m = ~bit_mask(mask_bits);
	return (v & m) == (base & m);
}

static inline uint32_t maskn_max(uint32_t base, unsigned mask_bits)
{
	return base | bit_mask(mask_bits);
}

static inline uint32_t maskn_base(uint32_t base, unsigned mask_bits)
{
	return base & ~bit_mask(mask_bits);
}

#endif
