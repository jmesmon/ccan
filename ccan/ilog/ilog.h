/* CC0 (Public domain) - see LICENSE file for details */
#if !defined(_ilog_H)
# define _ilog_H (1)
# include "config.h"
# include <stdint.h>
# include <limits.h>
# include <ccan/ct/ct.h>
# include <ccan/compiler/compiler.h>
# include <ccan/pp/pp.h>
# include <ccan/pp_ti/pp_ti.h>

/**
 * ilog16 - Integer binary logarithm of a 16-bit value.
 * @_v: A 16-bit value.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 *
 * See Also:
 *	ilog16_nz(), ilog64()
 *
 * Example:
 *	// Rounds up to next power of 2 (if not a power of 2).
 *	static uint16_t round_up16(uint16_t i)
 *	{
 *		assert(i != 0);
 *		return 1U << ilog16(i-1);
 *	}
 */
int ilog16(uint16_t _v) CONST_FUNCTION;

/**
 * ilog16_nz - Integer binary logarithm of a non-zero 16-bit value.
 * @_v: A 16-bit value.
 * Returns floor(log2(_v))+1, or undefined if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog16(), ilog64_nz()
 * Example:
 *	// Find Last Set (ie. highest bit set, 0 to 31).
 *	static uint16_t fls16(uint16_t i)
 *	{
 *		assert(i != 0);
 *		return ilog16_nz(i) - 1;
 *	}
 */
int ilog16_nz(uint16_t _v) CONST_FUNCTION;

/**
 * ilog32 - Integer binary logarithm of a 32-bit value.
 * @_v: A 32-bit value.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 *
 * See Also:
 *	ilog32_nz(), ilog64()
 *
 * Example:
 *	// Rounds up to next power of 2 (if not a power of 2).
 *	static uint32_t round_up32(uint32_t i)
 *	{
 *		assert(i != 0);
 *		return 1U << ilog32(i-1);
 *	}
 */
int ilog32(uint32_t _v) CONST_FUNCTION;

/**
 * ilog32_nz - Integer binary logarithm of a non-zero 32-bit value.
 * @_v: A 32-bit value.
 * Returns floor(log2(_v))+1, or undefined if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog32(), ilog64_nz()
 * Example:
 *	// Find Last Set (ie. highest bit set, 0 to 31).
 *	static uint32_t fls32(uint32_t i)
 *	{
 *		assert(i != 0);
 *		return ilog32_nz(i) - 1;
 *	}
 */
int ilog32_nz(uint32_t _v) CONST_FUNCTION;

/**
 * ilog64 - Integer binary logarithm of a 64-bit value.
 * @_v: A 64-bit value.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog64_nz(), ilog32()
 */
int ilog64(uint64_t _v) CONST_FUNCTION;

/**
 * ilog64_nz - Integer binary logarithm of a non-zero 64-bit value.
 * @_v: A 64-bit value.
 * Returns floor(log2(_v))+1, or undefined if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog64(), ilog32_nz()
 */
int ilog64_nz(uint64_t _v) CONST_FUNCTION;

/**
 * STATIC_ILOG_16 - The integer logarithm of an (unsigned, 16-bit) constant.
 * @_v: A non-negative 16-bit constant.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * This macro should only be used when you need a compile-time constant,
 * otherwise ilog16 or ilog16_nz are just as fast and more flexible.
 */
#define STATIC_ILOG_16(_v) (STATIC_ILOG4((uint16_t)(_v)))

/**
 * STATIC_ILOG_32 - The integer logarithm of an (unsigned, 32-bit) constant.
 * @_v: A non-negative 32-bit constant.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * This macro should only be used when you need a compile-time constant,
 * otherwise ilog32 or ilog32_nz are just as fast and more flexible.
 *
 * Example:
 *	#define MY_PAGE_SIZE	4096
 *	#define MY_PAGE_BITS	(STATIC_ILOG_32(PAGE_SIZE) - 1)
 */
#define STATIC_ILOG_32(_v) (STATIC_ILOG5((uint32_t)(_v)))

/**
 * STATIC_ILOG_64 - The integer logarithm of an (unsigned, 64-bit) constant.
 * @_v: A non-negative 64-bit constant.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * This macro should only be used when you need a compile-time constant,
 * otherwise ilog64 or ilog64_nz are just as fast and more flexible.
 */
#define STATIC_ILOG_64(_v) (STATIC_ILOG6((uint64_t)(_v)))

/**
 * ilog8 - alias smaller sized ilog for convienience
 *
 * See ilog32(), ilog64() for usage.
 */
#define ilog8	ilog16
#define ilog8_nz  ilog16_nz

#define ILOG_(_v, _nz) \
	C_CHOOSE(VAR_COMPAT_TYPE(_v, uint8_t),  ilog8##_nz(_v), \
	C_CHOOSE(VAR_COMPAT_TYPE(_v, uint16_t), ilog16##_nz(_v), \
	C_CHOOSE(VAR_COMPAT_TYPE(_v, uint32_t), ilog32##_nz(_v), \
	C_CHOOSE(VAR_COMPAT_TYPE(_v, uint64_t), ilog64##_nz(_v), \
	C_CHOOSE_END)))

/**
 * ilog(v) - auto-select the appropriately sized ilog{32,64}() function
 * See Also:
 *	ilog64(), ilog32(), ilog_nz()
 */
#define ilog(_v) ILOG_(_v, EMPTY)

/**
 * ilog_nz(v) - auto-select the appropriately sized ilog{32,64}_nz() function
 * See Also:
 *	ilog64_nz(), ilog32_nz(), ilog()
 */
#define ilog_nz(_v) ILOG_(_v, _nz)

#define ILOG_SZ_(_sz) ilog##_sz
#define ILOG_SZ(_sz) ILOG_SZ(_sz)
#define ILOG_SZ_NZ_(_sz) ilog##_sz##_nz
#define ILOG_SZ_NZ(_sz) ILOG_SZ(_sz)

/**
 * ilog_u, ilog_lu, ilog_llu, ilog_zu (and _nz variants) - ilogs for non-bitsized types
 *
 * See ilog32(), ilog64() for usage.
 */
#define ilog_u		ILOG_SZ(INT_TO_BITS(EMPTY))
#define ilog_lu		ILOG_SZ(INT_TO_BITS(l))
#define ilog_llu	ILOG_SZ(INT_TO_BITS(ll))
#define ilog_zu		ILOG_SZ(INT_TO_BITS(z))
#define ilog_u_nz	ILOG_NZ_SZ(INT_TO_BITS(EMPTY))
#define ilog_lu_nz	ILOG_NZ_SZ(INT_TO_BITS(l))
#define ilog_llu_nz	ILOG_NZ_SZ(INT_TO_BITS(ll))
#define ilog_zu_nz	ILOG_NZ_SZ(INT_TO_BITS(z))

/* Private implementation details */

#define STATIC_ILOG_SZ(_sz) STATIC_ILOG_##_sz
/* ilog maybe-static */
#define ILOG_MS(_sz, _v) \
	(IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_SZ(_sz)(_v) : ilog##_sz(_v))

/* CAT3 used to split up HAVE_BUILTIN_CLZ (which would otherwise expand */
#define HAVE_CLZ(bits) CAT3(HAVE_BUILTI,N_CLZ,BITS_TO_INT_C(bits))

/*Note the casts to (int) below: this prevents "upgrading"
   the type of an entire expression to an (unsigned) size_t.*/
#if HAVE_CLZ(16)
# define builtin_clz_16(v) CAT2(__builtin_clz,BITS_TO_INT(16))(v)
# define builtin_ilog16_nz(v) \
	(((int)sizeof(unsigned)*CHAR_BIT) - builtin_clz_16(v))
# define builtin_ilog16(_v) (builtin_ilog16_nz(_v)&-!!(_v))
# define ilog16_nz(_v) builtin_ilog16_nz(_v)
# define ilog16(_v)    builtin_ilog16(_v)
#else /* !HAVE_CLZ(16) */
# define ilog16_nz(_v) ilog16(_v)
# define ilog16(_v)    ILOG_MS(16, _v)
#endif

#if HAVE_CLZ(32)
# define builtin_clz_32(v) CAT2(__builtin_clz,BITS_TO_INT(32))(v)
# define builtin_ilog32_nz(v) \
	(((int)sizeof(unsigned)*CHAR_BIT) - builtin_clz_32(v))
# define builtin_ilog32(_v) (builtin_ilog32_nz(_v)&-!!(_v))
# define ilog32_nz(_v) builtin_ilog32_nz(_v)
# define ilog32(_v)    builtin_ilog32(_v)
#else /* !HAVE_CLZ(32) */
# define ilog32_nz(_v) ilog32(_v)
# define ilog32(_v)    ILOG_MS(32, _v)
#endif

#if HAVE_CLZ(64)
# define builtin_clz_64(v) CAT2(__builtin_clz,BITS_TO_INT(64))(v)
# define builtin_ilog64_nz(v) \
	(((int)sizeof(unsigned)*CHAR_BIT) - builtin_clz_64(v))
# define builtin_ilog64(_v) (builtin_ilog64_nz(_v)&-!!(_v))
# define ilog64_nz(_v) builtin_ilog64_nz(_v)
# define ilog64(_v)    builtin_ilog64(_v)
#else /* !HAVE_CLZ(64) */
# define ilog64_nz(_v) ilog64(_v)
# define ilog64(_v)    ILOG_MS(64, _v)
#endif

/* Macros for evaluating compile-time constant ilog. */
# define STATIC_ILOG0(_v) (!!(_v))
# define STATIC_ILOG1(_v) (((_v)&0x2)?2:STATIC_ILOG0(_v))
# define STATIC_ILOG2(_v) (((_v)&0xC)?2+STATIC_ILOG1((_v)>>2):STATIC_ILOG1(_v))
# define STATIC_ILOG3(_v) \
 (((_v)&0xF0)?4+STATIC_ILOG2((_v)>>4):STATIC_ILOG2(_v))
# define STATIC_ILOG4(_v) \
 (((_v)&0xFF00)?8+STATIC_ILOG3((_v)>>8):STATIC_ILOG3(_v))
# define STATIC_ILOG5(_v) \
 (((_v)&0xFFFF0000)?16+STATIC_ILOG4((_v)>>16):STATIC_ILOG4(_v))
# define STATIC_ILOG6(_v) \
 (((_v)&0xFFFFFFFF00000000ULL)?32+STATIC_ILOG5((_v)>>32):STATIC_ILOG5(_v))

#endif /* _ilog_H */
