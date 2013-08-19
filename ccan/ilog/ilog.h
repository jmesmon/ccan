/* CC0 (Public domain) - see LICENSE file for details */
#if !defined(_ilog_H)
# define _ilog_H (1)
# include "config.h"
# include <stdint.h>
# include <limits.h>
# include <ccan/compiler/compiler.h>
# include <ccan/ret_type/ret_type.h>

/**
 * ceil_ilog_* - calculate ceiling(log2(_v))
 */
u8_ret_t ceil_ilog_8 (uint8_t  _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_16(uint16_t _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_32(uint32_t _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_64(uint64_t _v) CONST_FUNCTION;

#if 0
/* These are all aliased to the bit-width specific functions */
u8_ret_t ceil_ilog_u  (unsigned           _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_ul (unsigned long      _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_ull(unsigned long long _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_zu (size_t             _v) CONST_FUNCTION;
#endif

#define CEIL_ILOG_(sz) ceil_ilog_##sz
#define CEIL_ILOG(sz) CEIL_ILOG_(BITS_##sz)

#define ceil_ilog_u   CEIL_ILOG(u)
#define ceil_ilog_ul  CEIL_ILOG(ul)
#define ceil_ilog_ull CEIL_ILOG(ull)
#define ceil_ilog_zu  CEIL_ILOG(zu)

#if HAVE_C11_GENERIC
#define ceil_ilog(v)				\
	_Generic((v),				\
		uint8_t: ceil_ilog_8(v),	\
		uint16_t: ceil_ilog_16(v),	\
		uint32_t: ceil_ilog_32(v),	\
		uint64_t: ceil_ilog_64(v),	\
		default: BUILD_FAILURE_WITH_FALLBACK_RUNTIME \
		)
#elif HAVE_BUILTIN_TYPES_COMPATIBLE_P
#define ceil_ilog(v)								\
	__builtin_types_compatible_p(typeof(v), uint8_t) ? ceil_ilog_8(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint16_t) ? ceil_ilog_16(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint32_t) ? ceil_ilog_32(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint64_t) ? ceil_ilog_64(v) :	\
	 BUILD_FAILURE_WITH_FALLBACK_RUNTIME )))
#else
#define ceil_ilog(v)						\
		(sizeof(v) == sizeof(uint8_t) ? ceil_ilog_8(v)	\
	:	(sizeof(v) == sizeof(uint16_t)? ceil_ilog_16(v)	\
	:	(sizeof(v) == sizeof(uint32_t)? ceil_ilog_32(v)	\
	:	(sizeof(v) == sizeof(uint64_t)? ceil_ilog_64(v)	\
	:	BUILD_FAILURE_WITH_FALLBACK_RUNTIME))))
#endif

/*** floor(log2(_v)) + 1 ***/

u8_ret_t ilog_8 (uint8_t  _v) CONST_FUNCTION;
u8_ret_t ilog_16(uint16_t _v) CONST_FUNCTION;
u8_ret_t ilog_32(uint32_t _v) CONST_FUNCTION;
u8_ret_t ilog_64(uint64_t _v) CONST_FUNCTION;

u8_ret_t ilog_8_nz (uint8_t  _v) CONST_FUNCTION;
u8_ret_t ilog_16_nz(uint16_t _v) CONST_FUNCTION;
u8_ret_t ilog_32_nz(uint32_t _v) CONST_FUNCTION;
u8_ret_t ilog_64_nz(uint64_t _v) CONST_FUNCTION;

#if 0
/* These are all aliased to the bit-width specific functions */
u8_ret_t ilog_u  (unsigned           _v) CONST_FUNCTION;
u8_ret_t ilog_ul (unsigned long      _v) CONST_FUNCTION;
u8_ret_t ilog_ull(unsigned long long _v) CONST_FUNCTION;
u8_ret_t ilog_zu (size_t             _v) CONST_FUNCTION;
#endif

#define ILOG_(sz) ilog_##sz
#define ILOG(sz) ILOG_(BITS_##sz)
#define ILOG_NZ_(sz) ilog_##sz##_nz
#define ILOG_NZ(sz) ILOG_(BITS_##sz)

#define ilog_u   ILOG(u)
#define ilog_ul  ILOG(ul)
#define ilog_ull ILOG(ull)
#define ilog_zu  ILOG(zu)

#define ilog_u_nz   ILOG_NZ(u)
#define ilog_ul_nz  ILOG_NZ(ul)
#define ilog_ull_nz ILOG_NZ(ull)
#define ilog_zu_nz  ILOG_NZ(zu)

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

/* Private implementation details */

/*Note the casts to (int) below: this prevents "upgrading"
   the type of an entire expression to an (unsigned) size_t.*/
#if HAVE_BUILTIN_CLZ

# define __ILOG_B(v, type, type_suffix) \
	(((int)sizeof(type)*CHAR_BIT) - __builtin_clz##type_suffix(v))

# define __ILOG_C(v, type, type_suffix) \
	(((int)sizeof(type)*CHAR_BIT) - __builtin_clz##type_suffix(v))

# define builtin_ilog_u_nz(v)   __ILOG_B(v, unsigned, )
# define builtin_ilog_ul_nz(v)  __ILOG_B(v, unsigned long, l)
# define builtin_ilog_ull_nz(v) __ILOG_B(v, unsigned long long, ll)

# if   UINT_MAX   >= SIZE_MAX
#  define builtin_ilog_zu_nz(v) builtin_ilog_u_nz(v)
# elif ULONG_MAX  >= SIZE_MAX
#  define builtin_ilog_zu_nz(v) builtin_ilog_ul_nz(v)
# elif ULLONG_MAX >= SIZE_MAX
#  define builtin_ilog_zu_nz(v) builtin_ilog_ull_nz(v)
# endif

# if   UINT_MAX   >= 0xff
#  define builtin_ilog8_nz(v) builtin_ilog_u_nz(v)
# endif

# if   UINT_MAX   >= 0xffff
#  define builtin_ilog16_nz(v) builtin_ilog_u_nz(v)
# elif ULONG_MAX  >= 0xffff
#  define builtin_ilog16_nz(v) builtin_ilog_ul_nz(v)
# endif

# if   UINT_MAX   >= 0xffffffff
#  define builtin_ilog32_nz(v) builtin_ilog_u_nz(v)
# elif ULONG_MAX  >= 0xffffffff
#  define builtin_ilog32_nz(v) builtin_ilog_ul_nz(v)
# elif ULLONG_MAX >= 0xffffffff
#  define builtin_ilog32_nz(v) builtin_ilog_ull_nz(v)
# endif

# if    UINT_MAX   >= 0xffffffffffffffff
#  define builtin_ilog64_nz(v) builtin_ilog_u_nz(v)
# elif  ULONG_MAX  >= 0xffffffffffffffff
#  define builtin_ilog64_nz(v) builtin_ilog_ul_nz(v)
# elif  ULLONG_MAX >= 0xffffffffffffffff
#  define builtin_ilog64_nz(v) builtin_ilog_ull_nz(v)
# endif

#endif /* HAVE_BUILTIN_CLZ */


#ifdef builtin_ilog_u_nz
# define ilog_u(_v)   (builtin_ilog_u_nz(_v)&-!!(_v))
# define ilog_u_nz(_v)    builtin_ilog_u_nz(_v)
#else
# if   0xff       >= UINT_MAX
#  define ilog_u(_v) ilog8(_v)
#  define ilog_u_nz(_v) ilog8_nz(_v)
# elif 0xffff     >= UINT_MAX
#  define ilog_u(_v) ilog16(_v)
#  define ilog_u_nz(_v) ilog16_nz(_v)
# elif 0xffffffff >= UINT_MAX
#  define ilog_u(_v) ilog32(_v)
#  define ilog_u_nz(_v) ilog32_nz(_v)
# elif 0xffffffffffffffff >= UINT_MAX
#  define ilog_u(_v) ilog64(_v)
#  define ilog_u_nz(_v) ilog64_nz(_v)
# endif
#endif

#ifdef builtin_ilog_ul_nz
# define ilog_ul(_v)   (builtin_ilog_ul_nz(_v)&-!!(_v))
# define ilog_ul_nz(_v)    builtin_ilog_ul_nz(_v)
#else
# if   0xff       >= UINT_MAX
#  define ilog_ul(_v) ilog8(_v)
#  define ilog_ul_nz(_v) ilog8_nz(_v)
# elif 0xffff     >= UINT_MAX
#  define ilog_ul(_v) ilog16(_v)
#  define ilog_ul_nz(_v) ilog16_nz(_v)
# elif 0xffffffff >= UINT_MAX
#  define ilog_ul(_v) ilog32(_v)
#  define ilog_ul_nz(_v) ilog32_nz(_v)
# elif 0xffffffffffffffff >= UINT_MAX
#  define ilog_ul(_v) ilog64(_v)
#  define ilog_ul_nz(_v) ilog64_nz(_v)
# endif
#endif

#ifdef builtin_ilog_ull_nz
# define ilog_ull(_v)   (builtin_ilog_ull_nz(_v)&-!!(_v))
# define ilog_ull_nz(_v)    builtin_ilog_ull_nz(_v)
#else
# if   0xff       >= UINT_MAX
#  define ilog_ull(_v) ilog8(_v)
#  define ilog_ull_nz(_v) ilog8_nz(_v)
# elif 0xffff     >= UINT_MAX
#  define ilog_ull(_v) ilog16(_v)
#  define ilog_ull_nz(_v) ilog16_nz(_v)
# elif 0xffffffff >= UINT_MAX
#  define ilog_ull(_v) ilog32(_v)
#  define ilog_ull_nz(_v) ilog32_nz(_v)
# elif 0xffffffffffffffff >= UINT_MAX
#  define ilog_ull(_v) ilog64(_v)
#  define ilog_ull_nz(_v) ilog64_nz(_v)
# endif
#endif

#ifdef builtin_ilog8_nz
# define ilog8(_v)    (builtin_ilog8_nz(_v)&-!!(_v))
# define ilog8_nz(_v)  builtin_ilog8_nz(_v)
#else
  /* TODO: provide optimized 8bit versions of non-static ilog */
# define ilog8_nz(_v) ilog8(_v)
# define ilog8(_v) (IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_32(_v) : ilog8(_v))
#endif

#ifdef builtin_ilog16_nz
# define ilog16(_v)    (builtin_ilog16_nz(_v)&-!!(_v))
# define ilog16_nz(_v) builtin_ilog16_nz(_v)
#else
  /* TODO: provide optimized 16bit versions of non-static ilog */
# define ilog16_nz(_v) ilog16(_v)
# define ilog16(_v) (IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_32(_v) : ilog16(_v))
#endif

#ifdef builtin_ilog32_nz
#define ilog32(_v) (builtin_ilog32_nz(_v)&-!!(_v))
#define ilog32_nz(_v) builtin_ilog32_nz(_v)
#else
#define ilog32_nz(_v) ilog32(_v)
#define ilog32(_v) (IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_32(_v) : ilog32(_v))
#endif /* builtin_ilog32_nz */

#ifdef builtin_ilog64_nz
#define ilog64(_v) (builtin_ilog64_nz(_v)&-!!(_v))
#define ilog64_nz(_v) builtin_ilog64_nz(_v)
#else
#define ilog64_nz(_v) ilog64(_v)
#define ilog64(_v) (IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_64(_v) : ilog64(_v))
#endif /* builtin_ilog64_nz */

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

/**
 * ilog -
 */
#if HAVE_BUILTIN_CHOOSE_EXPR && HAVE_BUILTIN_TYPES_COMPATIBLE_P && HAVE_TYPEOF
#define ilog(_v) (\
	__builtin_choose_expr(__builtin_types_compatible_p(__typeof__(_v), uint64_t),	\
		ilog64(_v),								\
	__builtin_choose_expr(__builtin_types_compatible_p(__typeof__(_v), uint32_t),	\
		ilog32(_v),								\
	__builtin_choose_expr(__builtin_types_compatible_p(__typeof__(_v), uint16_t),	\
		ilog16(_v),								\
	__builtin_choose_expr(__builtin_types_compatible_p(__typeof__(_v), uint8_t),	\
		ilog8(_v),								\
	(void)0)))))
#endif

/**
 * ilog_n - Calculate the interger log of _v given we know at most _bits bits are set in _v
 *
 */
#define ilog_n(_v, _bits)

#endif /* _ilog_H */
