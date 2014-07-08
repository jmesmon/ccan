/* LGPLv3 or later - see LICENSE file for details */
#include <ccan/maskn/maskn.h>
#include <limits.h>
#include <assert.h>

#if defined(__GNUC__)
/* gcc's __builtin_{clz,ctz}() are undefined if v == 0 */
# define builtin_clz(v) (v ? __builtin_clz(v) : (sizeof(v) * CHAR_BIT))
# define builtin_ctz(v) (v ? __builtin_ctz(v) : (sizeof(v) * CHAR_BIT))
#elif defined(__ICCARM__)
# include <intrinsics.h>
/* emits the ARM instruction, which returns 32 if no bits are set */
# define builtin_clz(v) __CLZ(v)
# define builtin_ctz(v) __CLZ(__RBIT(v))
#else
# warning "Unrecognized compiler"
#endif

/* from https://graphics.stanford.edu/~seander/bithacks.html */
static unsigned ctz_32(uint32_t v)
{
#ifdef builtin_ctz
	return builtin_ctz(v);
#else
	unsigned c;
	if (!v)
		return 32;
	if (v & 0x1)
		return 0;
	c = 1;
	if ((v & 0xffff) == 0) {
		v >>= 16;
		c += 16;
	}
	if ((v & 0xff) == 0) {
		v >>= 8;
		c += 8;
	}
	if ((v & 0xf) == 0) {
		v >>= 4;
		c += 4;
	}
	if ((v & 0x3) == 0) {
		v >>= 2;
		c += 2;
	}
	c -= v & 0x1;
	return c;
#endif
}

static unsigned clz_32(uint32_t v)
{
#ifdef builtin_clz
	return builtin_clz(v);
#else
	unsigned c = 0;
	if (!v)
		return 32;
	if ((v & 0xffff0000) == 0) {
		c += 16;
		v <<= 16;
	}
	if ((v & 0xff000000) == 0) {
		c += 8;
		v <<= 8;
	}
	if ((v & 0xf0000000) == 0) {
		c += 4;
		v <<= 4;
	}
	if ((v & 0xc0000000) == 0) {
		c += 2;
		v <<= 2;
	}
	if ((v & 0x80000000) == 0) {
		c += 1;
	}
	return c;
#endif
}

static unsigned fls_m1_32(uint32_t v)
{
	if (!v)
		return 32;
	return ((CHAR_BIT * sizeof(v)) - clz_32(v)) - 1;
}

unsigned maskn_from_range_low(uint32_t base, uint32_t max)
{
	assert(base <= max);
	unsigned base_tz = ctz_32(base);
	uint32_t mask_1 = bit_mask(base_tz);
	uint32_t masked_max = max & mask_1;
	unsigned log_of_max_masked = fls_m1_32(masked_max + 1);

	return log_of_max_masked;
}

unsigned maskn_from_range_high(uint32_t base, uint32_t min)
{
	assert(base >= min);
	unsigned base_ones = ctz_32(~base);
	uint32_t diff = base - min;
	uint32_t masked_diff = diff & bit_mask(base_ones);
	unsigned mask_bits = fls_m1_32(masked_diff + 1);
	return mask_bits;
}

unsigned maskn_from_range(uint32_t base, uint32_t limit)
{
	if (base < limit)
		return maskn_from_range_low(base, limit);
	else
		return maskn_from_range_high(base, limit);
}
