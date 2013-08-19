/* CC0 (Public domain) - see LICENSE file for details */
#if !defined(_ilog_H)
# define _ilog_H (1)
# include "config.h"
# include <stddef.h>
# include <stdint.h>
# include <limits.h>
# include <ccan/compiler/compiler.h>
# include <ccan/ret_type/ret_type.h>
# include <ccan/bits/bits.h>
# include <ccan/pp/pp.h>

/**
 * ceil_ilog_* - calculate ceiling(log2(_v))
 */
u8_ret_t ceil_ilog_8 (uint8_t  _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_16(uint16_t _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_32(uint32_t _v) CONST_FUNCTION;
u8_ret_t ceil_ilog_64(uint64_t _v) CONST_FUNCTION;

/* These are all aliased to the bit-width specific functions */
static inline u8_ret_t ceil_ilog_u  (unsigned           _v) CONST_FUNCTION;
static inline u8_ret_t ceil_ilog_ul (unsigned long      _v) CONST_FUNCTION;
static inline u8_ret_t ceil_ilog_ull(unsigned long long _v) CONST_FUNCTION;
static inline u8_ret_t ceil_ilog_zu (size_t             _v) CONST_FUNCTION;

#define CEIL_ILOG__(sz) ceil_ilog_##sz
#define CEIL_ILOG_(sz) CEIL_ILOG__(sz)
#define CEIL_ILOG(sz) CEIL_ILOG_(BITS_##sz)

/* These cannot be defines due to later defines to wrap for compile time
 * evaluation */
static inline u8_ret_t ceil_ilog_u(unsigned _v) {
	return CEIL_ILOG(u)(_v);
}
static inline u8_ret_t ceil_ilog_ul(unsigned long _v) {
	return CEIL_ILOG(ul)(_v);
}
static inline u8_ret_t ceil_ilog_ull(unsigned long long _v) {
	return CEIL_ILOG(ull)(_v);
}
static inline u8_ret_t ceil_ilog_zu(size_t _v) {
	return CEIL_ILOG(zu)(_v);
}

#if HAVE_C11_GENERIC
#define ceil_ilog(v)				\
	_Generic((v),				\
		uint8_t: ceil_ilog_8(v),	\
		uint16_t: ceil_ilog_16(v),	\
		uint32_t: ceil_ilog_32(v),	\
		uint64_t: ceil_ilog_64(v),	\
		default: (void)0 \
		)
#elif HAVE_BUILTIN_TYPES_COMPATIBLE_P
#define ceil_ilog(v)								\
	__builtin_types_compatible_p(typeof(v), uint8_t) ? ceil_ilog_8(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint16_t) ? ceil_ilog_16(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint32_t) ? ceil_ilog_32(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint64_t) ? ceil_ilog_64(v) :	\
	 (void)0 )))
#else
#define ceil_ilog(v)						\
		(sizeof(v) == sizeof(uint8_t) ? ceil_ilog_8(v)	\
	:	(sizeof(v) == sizeof(uint16_t)? ceil_ilog_16(v)	\
	:	(sizeof(v) == sizeof(uint32_t)? ceil_ilog_32(v)	\
	:	(sizeof(v) == sizeof(uint64_t)? ceil_ilog_64(v)	\
	:	(void)0))))
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

/* These are all aliased to the bit-width specific functions */
static inline u8_ret_t ilog_u  (unsigned           _v) CONST_FUNCTION;
static inline u8_ret_t ilog_ul (unsigned long      _v) CONST_FUNCTION;
static inline u8_ret_t ilog_ull(unsigned long long _v) CONST_FUNCTION;
static inline u8_ret_t ilog_zu (size_t             _v) CONST_FUNCTION;

#define ILOG__(sz) ilog_##sz
#define ILOG_(sz) ILOG__(sz)
#define ILOG(sz) ILOG_(BITS_##sz)
#define ILOG_NZ__(sz) ilog_##sz##_nz
#define ILOG_NZ_(sz) ILOG__(sz)
#define ILOG_NZ(sz) ILOG_(BITS_##sz)

/* These cannot be defines due to later defines to wrap for compile time
 * evaluation */
static inline u8_ret_t ilog_u(unsigned _v) {
	return ILOG(u)(_v);
}
static inline u8_ret_t ilog_ul(unsigned long _v) {
	return ILOG(ul)(_v);
}
static inline u8_ret_t ilog_ull(unsigned long long _v) {
	return ILOG(ull)(_v);
}
static inline u8_ret_t ilog_zu(size_t _v) {
	return ILOG(zu)(_v);
}
static inline u8_ret_t ilog_u_nz(unsigned _v) {
	return ILOG_NZ(u)(_v);
}
static inline u8_ret_t ilog_ul_nz(unsigned long _v) {
	return ILOG_NZ(ul)(_v);
}
static inline u8_ret_t ilog_ull_nz(unsigned long long _v) {
	return ILOG_NZ(ull)(_v);
}
static inline u8_ret_t ilog_zu_nz(size_t _v) {
	return ILOG_NZ(zu)(_v);
}

#if HAVE_C11_GENERIC
#define ilog(v)				\
	_Generic((v),				\
		uint8_t: ilog_8(v),	\
		uint16_t: ilog_16(v),	\
		uint32_t: ilog_32(v),	\
		uint64_t: ilog_64(v),	\
		default: (void)0 \
		)
#elif HAVE_BUILTIN_TYPES_COMPATIBLE_P
#define ilog(v)								\
	__builtin_types_compatible_p(typeof(v), uint8_t) ? ilog_8(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint16_t) ? ilog_16(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint32_t) ? ilog_32(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint64_t) ? ilog_64(v) :	\
	 (void)0 )))
#else
#define ilog(v)						\
		(sizeof(v) == sizeof(uint8_t) ? ilog_8(v)	\
	:	(sizeof(v) == sizeof(uint16_t)? ilog_16(v)	\
	:	(sizeof(v) == sizeof(uint32_t)? ilog_32(v)	\
	:	(sizeof(v) == sizeof(uint64_t)? ilog_64(v)	\
	:	(void)0))))
#endif

#if HAVE_C11_GENERIC
#define ilog_nz(v)				\
	_Generic((v),				\
		uint8_t: ilog_8_nz(v),	\
		uint16_t: ilog_16_nz(v),	\
		uint32_t: ilog_32_nz(v),	\
		uint64_t: ilog_64_nz(v),	\
		default: (void)0 \
		)
#elif HAVE_BUILTIN_TYPES_COMPATIBLE_P
#define ilog_nz(v)								\
	__builtin_types_compatible_p(typeof(v), uint8_t) ? ilog_8_nz(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint16_t) ? ilog_16_nz(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint32_t) ? ilog_32_nz(v) :	\
	(__builtin_types_compatible_p(typeof(v), uint64_t) ? ilog_64_nz(v) :	\
	 (void)0 )))
#else
#define ilog_nz(v)						\
		(sizeof(v) == sizeof(uint8_t) ? ilog_8_nz(v)	\
	:	(sizeof(v) == sizeof(uint16_t)? ilog_16_nz(v)	\
	:	(sizeof(v) == sizeof(uint32_t)? ilog_32_nz(v)	\
	:	(sizeof(v) == sizeof(uint64_t)? ilog_64_nz(v)	\
	:	(void)0))))
#endif

/**
 * ilog_32 - Integer binary logarithm of a 32-bit value.
 * @_v: A 32-bit value.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 *
 * See Also:
 *	ilog_32_nz(), ilog64()
 *
 * Example:
 *	// Rounds up to next power of 2 (if not a power of 2).
 *	static uint32_t round_up32(uint32_t i)
 *	{
 *		assert(i != 0);
 *		return 1U << ilog_32(i-1);
 *	}
 */

/**
 * ilog_32_nz - Integer binary logarithm of a non-zero 32-bit value.
 * @_v: A 32-bit value.
 * Returns floor(log2(_v))+1, or undefined if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog_32(), ilog_64_nz()
 * Example:
 *	// Find Last Set (ie. highest bit set, 0 to 31).
 *	static uint32_t fls32(uint32_t i)
 *	{
 *		assert(i != 0);
 *		return ilog_32_nz(i) - 1;
 *	}
 */

/**
 * ilog_64 - Integer binary logarithm of a 64-bit value.
 * @_v: A 64-bit value.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog64_nz(), ilog_32()
 */

/**
 * ilog_64_nz - Integer binary logarithm of a non-zero 64-bit value.
 * @_v: A 64-bit value.
 * Returns floor(log2(_v))+1, or undefined if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * Note that many uses will resolve to the fast macro version instead.
 * See Also:
 *	ilog_64(), ilog_32_nz()
 */

/**
 * STATIC_ILOG_32 - The integer logarithm of an (unsigned, 32-bit) constant.
 * @_v: A non-negative 32-bit constant.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * This macro should only be used when you need a compile-time constant,
 * otherwise ilog_32 or ilog_32_nz are just as fast and more flexible.
 *
 * Example:
 *	#define MY_PAGE_SIZE	4096
 *	#define MY_PAGE_BITS	(STATIC_ILOG_32(PAGE_SIZE) - 1)
 */

/**
 * STATIC_ILOG_64 - The integer logarithm of an (unsigned, 64-bit) constant.
 * @_v: A non-negative 64-bit constant.
 * Returns floor(log2(_v))+1, or 0 if _v==0.
 * This is the number of bits that would be required to represent _v in two's
 *  complement notation with all of the leading zeros stripped.
 * This macro should only be used when you need a compile-time constant,
 * otherwise ilog_64 or ilog_64_nz are just as fast and more flexible.
 */

#define STATIC_ILOG_8(_v)  STATIC_ILOG3((uint8_t)(_v))
#define STATIC_ILOG_16(_v) STATIC_ILOG4((uint16_t)(_v))
#define STATIC_ILOG_32(_v) STATIC_ILOG5((uint32_t)(_v))
#define STATIC_ILOG_64(_v) STATIC_ILOG6((uint64_t)(_v))

#define STATIC_ILOG__(tt) STATIC_ILOG_##tt
#define STATIC_ILOG_(tt)  STATIC_ILOG__(BITS_##tt)

#define STATIC_ILOG_zu(_v)  STATIC_ILOG_(zu)
#define STATIC_ILOG_u(_v)   STATIC_ILOG_(u)
#define STATIC_ILOG_ul(_v)  STATIC_ILOG_(ul)
#define STATIC_ILOG_ull(_v) STATIC_ILOG_(ull)

/* Private implementation details */

/*Note the casts to (int) below: this prevents "upgrading"
   the type of an entire expression to an (unsigned) size_t.*/
#if HAVE_BUILTIN_CLZ

# define __ILOG_B(v, type, type_suffix) \
	(((int)sizeof(type)*CHAR_BIT) - __builtin_clz##type_suffix(v))

# define __ILOG_C(v, type, type_suffix) \
	(((int)sizeof(type)*CHAR_BIT) - __builtin_clz##type_suffix(v))

# define ceil_ilog_from_ilog(x, log_of_x) ((log_of_x) + !!((x) ^ (1 << (log_of_x))))

# define builtin_ilog_u_nz(v)   __ILOG_B(v, unsigned, )
# define builtin_ilog_ul_nz(v)  __ILOG_B(v, unsigned long, l)
# define builtin_ilog_ull_nz(v) __ILOG_B(v, unsigned long long, ll)
# define builtin_ilog__nz(t,v)  CAT3(builtin_ilog_,EV(UINT_##t),_nz)(v)

# define builtin_ceil_ilog_u_nz(v)   __ILOG_C(v, unsigned, )
# define builtin_ceil_ilog_ul_nz(v)  __ILOG_C(v, unsigned long, l)
# define builtin_ceil_ilog_ull_nz(v) __ILOG_C(v, unsigned long long, ll)
# define builtin_ceil_ilog__nz(t,v) CAT3(builtin_ceil_ilog_,EV(UINT_##t),_nz)(v)

# define builtin_ilog_zu_nz(v) builtin_ilog__nz(zu,v)
# define builtin_ilog_8_nz(v)  builtin_ilog__nz(8 ,v)
# define builtin_ilog_16_nz(v) builtin_ilog__nz(16,v)
# define builtin_ilog_32_nz(v) builtin_ilog__nz(32,v)
# define builtin_ilog_64_nz(v) builtin_ilog__nz(64,v)

#endif /* HAVE_BUILTIN_CLZ */

/* given a uint type suffix (u, ul, ull), call the appropriate ilog() */
# define ilog__(t, _v) CAT2(ilog_,EV(BITS_##t))(_v)
# define ilog__nz(_v) CAT3(ilog_,EV(BITS_##t),_nz)(_v)

#ifdef builtin_ilog_u_nz
# define ilog_u(_v)	(builtin_ilog_u_nz(_v)&-!!(_v))
# define ilog_u_nz(_v)	builtin_ilog_u_nz(_v)
#else
# define ilog_u(_v)	ilog__(u,_v)
# define ilog_u_nz(_v)	ilog__nz(u,_v)
#endif

#ifdef builtin_ilog_ul_nz
# define ilog_ul(_v)	(builtin_ilog_ul_nz(_v)&-!!(_v))
# define ilog_ul_nz(_v)	builtin_ilog_ul_nz(_v)
#else
# define ilog_ul(_v)	ilog__(ul,_v)
# define ilog_ul_nz(_v) ilog__nz(ul,_v)
#endif

#ifdef builtin_ilog_ull_nz
# define ilog_ull(_v)		(builtin_ilog_ull_nz(_v)&-!!(_v))
# define ilog_ull_nz(_v)	builtin_ilog_ull_nz(_v)
#else
# define ilog_ull(_v)		ilog__(ull,_v)
# define ilog_ull_nz(_v)	ilog__nz(ull,_v)
#endif

#ifdef builtin_ilog_8_nz
# define ilog_8(_v)    (builtin_ilog_8_nz(_v)&-!!(_v))
# define ilog_8_nz(_v)  builtin_ilog_8_nz(_v)
#else
# define ilog_8_nz(_v)	ilog_8(_v)
# define ilog_8(_v)	(IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_8(_v) : ilog_8(_v))
#endif

#ifdef builtin_ilog_16_nz
# define ilog_16(_v)    (builtin_ilog_16_nz(_v)&-!!(_v))
# define ilog_16_nz(_v) builtin_ilog_16_nz(_v)
#else
# define ilog_16_nz(_v)	ilog_16(_v)
# define ilog_16(_v)	(IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_16(_v) : ilog_16(_v))
#endif

#ifdef builtin_ilog_32_nz
#define ilog_32(_v)	(builtin_ilog_32_nz(_v)&-!!(_v))
#define ilog_32_nz(_v)	builtin_ilog_32_nz(_v)
#else
#define ilog_32_nz(_v)	ilog_32(_v)
#define ilog_32(_v)	(IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_32(_v) : ilog_32(_v))
#endif /* builtin_ilog32_nz */

#ifdef builtin_ilog_64_nz
#define ilog_64(_v)	(builtin_ilog_64_nz(_v)&-!!(_v))
#define ilog_64_nz(_v)	builtin_ilog_64_nz(_v)
#else
#define ilog_64_nz(_v)	ilog_64(_v)
#define ilog_64(_v)	(IS_COMPILE_CONSTANT(_v) ? STATIC_ILOG_64(_v) : ilog_64(_v))
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

#endif /* _ilog_H */
