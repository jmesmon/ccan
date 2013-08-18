/* Licenced as CC0 (Public Domain) - see LICENSE file for details */

#ifndef CCAN_PP_TI_BITS_H_
#define CCAN_PP_TI_BITS_H_

#include <stdint.h> /* SIZE_MAX */
#include <limits.h> /* *_MAX */

/*
 * BITS_, BITS_l, BITS_ll, BITS_z, Generally: BITS_<size-specifier>
 * resolve at preprocessor time to the number of bits in the type corresponding
 * to the type id.
 */

/* Provide mappings from BITs to suffixes, and suffixes to bits */

#if   UINT_MAX == 0xffu
# define BITS_ 8
#elif UINT_MAX == 0xffffu
# define BITS_ 16
#elif UINT_MAX == 0xffffffffu
# define BITS_ 32
#elif UINT_MAX == 0xffffffffffffffffu
# define BITS_ 64
#else
# error "Unknown UINT_MAX size"
#endif

#if   ULONG_MAX == 0xffu
# define BITS_l 8
#elif ULONG_MAX == 0xffffu
# define BITS_l 16
#elif ULONG_MAX == 0xffffffffu
# define BITS_l 32
#elif ULONG_MAX == 0xffffffffffffffffu
# define BITS_l 64
#else
# error "Unknown ULONG_MAX size"
#endif

#if   ULLONG_MAX == 0xffu
# define BITS_ll 8
#elif ULLONG_MAX == 0xffffu
# define BITS_ll 16
#elif ULLONG_MAX == 0xffffffffu
# define BITS_ll 32
#elif ULLONG_MAX == 0xffffffffffffffffu
# define BITS_ll 64
#else
# error "Unknown ULLONG_MAX size"
#endif

/* size_t might be unsigned, so we relax the matching for our
 * purposes.
 * TODO: different naming to set this off.
 */
#if   SIZE_MAX <= 0xffu
# define BITS_z 8
#elif SIZE_MAX <= 0xffffu
# define BITS_z 16
#elif SIZE_MAX <= 0xffffffffu
# define BITS_z 32
#elif SIZE_MAX <= 0xffffffffffffffffu
# define BITS_z 64
#else
# error "Unknown SIZE_MAX size"
#endif


#define BITS__(td) BITS_##td
#define INT_TO_BITS(td) BITS__(td)

#endif /* CCAN_PP_TI_BITS_H_ */
