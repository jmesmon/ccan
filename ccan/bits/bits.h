#ifndef CCAN_BITS_H_
#define CCAN_BITS_H_

#include <limits.h>

/*
 * BITS_u, BITS_ul, BITS_ull, BITS_zu, Generally: BITS_<typeid>
 * resolve at preprocessor time to the number of bits in the type corresponding
 * to the type id.
 *
 * UINT_8, UINT_16, UINT_32, UINT_64, UINT_zu, Generally: UINT_<typeid or bit count>
 * resolve to a typeid (one of 'u', 'ul', or 'ull') to indicate the smallest
 * unsigned type that encumpasses an integer of that size
 */

/* Provide mappings from BITs to suffixes, and suffixes to bits */

#if   UINT_MAX == 0xff
# define BITS_u 8
#elif UINT_MAX == 0xffff
# define BITS_u 16
#elif UINT_MAX == 0xffffffff
# define BITS_u 32
#elif UINT_MAX == 0xffffffffffffffff
# define BITS_u 64
#else
# error "Unknown UINT_MAX size"
#endif

#if   ULONG_MAX == 0xff
# define BITS_ul 8
#elif ULONG_MAX == 0xffff
# define BITS_ul 16
#elif ULONG_MAX == 0xffffffff
# define BITS_ul 32
#elif ULONG_MAX == 0xffffffffffffffff
# define BITS_ul 64
#else
# error "Unknown ULONG_MAX size"
#endif

#if   ULLONG_MAX == 0xff
# define BITS_ull 8
#elif ULLONG_MAX == 0xffff
# define BITS_ull 16
#elif ULLONG_MAX == 0xffffffff
# define BITS_ull 32
#elif ULLONG_MAX == 0xffffffffffffffff
# define BITS_ull 64
#else
# error "Unknown ULLONG_MAX size"
#endif

/* size_t might be unsigned, so we relax the matching for our
 * purposes.
 * TODO: different naming to set this off.
 */
#if   SIZE_MAX <= 0xff
# define BITS_zu 8
#elif SIZE_MAX <= 0xffff
# define BITS_zu 16
#elif SIZE_MAX <= 0xffffffff
# define BITS_zu 32
#elif SIZE_MAX <= 0xffffffffffffffff
# define BITS_zu 64
#else
# error "Unknown SIZE_MAX size"
#endif

#if   SIZE_MAX <= UINT_MAX
# define UINT_zu u
#elif SIZE_MAX <= ULONG_MAX
# define UINT_zu ul
#elif SIZE_MAX <= ULLONG_MAX
# define UINT_zu ull
#else
# error "Unknown SIZE_MAX size"
#endif


/* bit counts => types
 *
 * TODO: currently, only conversions to 'long' prefixed types are
 * generated. This should be split out and exact matches for smaller
 * types should also be provided.
 */

#if   0xff <= UINT_MAX
# define UINT_8 u
#elif 0xff <= ULONG_MAX
# define UINT_8 ul
#elif 0xff <= ULLONG_MAX
# define UINT_8 ull
#else
# error "Unknown type for 8 bits"
#endif

#if   0xffff <= UINT_MAX
# define UINT_16 u
#elif 0xffff <= ULONG_MAX
# define UINT_16 ul
#elif 0xffff <= ULLONG_MAX
# define UINT_16 ull
#else
# error "Unknown type for 16 bits"
#endif

#if   0xffffffff <= UINT_MAX
# define UINT_32 u
#elif 0xffffffff <= ULONG_MAX
# define UINT_32 ul
#elif 0xffffffff <= ULLONG_MAX
# define UINT_32 ull
#else
# error "Unknown type for 32 bits"
#endif

#if   0xffffffffffffffff <= UINT_MAX
# define UINT_64 u
#elif 0xffffffffffffffff <= ULONG_MAX
# define UINT_64 ul
#elif 0xffffffffffffffff <= ULLONG_MAX
# define UINT_64 ull
#else
# error "Unknown type for 64 bits"
#endif

#endif
