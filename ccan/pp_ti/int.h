/* Licenced as CC0 (Public Domain) - see LICENSE file for details */

#ifndef CCAN_PP_TI_INT_H_
#define CCAN_PP_TI_INT_H_

#include <stdint.h> /* SIZE_MAX */
#include <limits.h> /* *_MAX */

/*
 * INT_8, INT_16, INT_32, INT_64, INT_z, Generally: INT_<size spec or bit count>
 * resolve to a long-size-specifier (one of '', 'l', or 'll') to indicate the smallest
 * unsigned type that encumpasses an integer of that size
 */

/* bit counts => types
 *
 * TODO: currently, only conversions to 'long' prefixed types are
 * generated. This should be split out and exact matches for smaller
 * types should also be provided.
 */

#if   0xffu <= UINT_MAX
# define INT_8
# define INT_C_8
#elif 0xffu <= ULONG_MAX
# define INT_8 l
# define INT_C_8 L
#elif 0xffu <= ULLONG_MAX
# define INT_8 ll
# define INT_C_8 LL
#else
# error "Unknown type for 8 bits"
#endif

#if   0xffffu <= UINT_MAX
# define INT_16
# define INT_C_16
#elif 0xffff <= ULONG_MAX
# define INT_16 l
# define INT_C_16 L
#elif 0xffffu <= ULLONG_MAX
# define INT_16 ll
# define INT_C_16 LL
#else
# error "Unknown type for 16 bits"
#endif

#if   0xffffffffu <= UINT_MAX
# define INT_32
# define INT_C_32
#elif 0xffffffffu <= ULONG_MAX
# define INT_32 l
# define INT_C_32 L
#elif 0xffffffffu <= ULLONG_MAX
# define INT_32 ll
# define INT_C_32 LL
#else
# error "Unknown type for 32 bits"
#endif

#if   0xffffffffffffffffu <= UINT_MAX
# define INT_64
# define INT_C_64
#elif 0xffffffffffffffffu <= ULONG_MAX
# define INT_64 l
# define INT_C_64 L
#elif 0xffffffffffffffffu <= ULLONG_MAX
# define INT_64 ll
# define INT_C_64 LL
#else
# error "Unknown type for 64 bits"
#endif

#if   SIZE_MAX <= UINT_MAX
# define INT_z
# define INT_C_z
#elif SIZE_MAX <= ULONG_MAX
# define INT_z l
# define INT_C_z L
#elif SIZE_MAX <= ULLONG_MAX
# define INT_z ll
# define INT_C_z LL
#else
# error "Unknown SIZE_MAX size"
#endif


#define INT__(bits) INT_##bits
#define BITS_TO_INT(bits) INT__(bits)

#define INT__C(bits) INT_C_##bits
#define BITS_TO_INT_C(bits) INT__C(bits)

#endif /* CCAN_PP_TI_INT_H_ */
