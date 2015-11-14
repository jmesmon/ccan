/* CC0 license (public domain) - see LICENSE file for details */
#ifndef CCAN_INLINE_H_
#define CCAN_INLINE_H_

#include "config.h"

/**
 * INLINE_FUNC - prefix on inline declarations, placed where the body of the
 *               function is.
 *
 * Put this in the header file that would normally contain the body of the
 * inline function, and follow it with a signature & function body.
 *
 * If avaliable, this will use 'inline' + 'extern inline' support to give the
 * compiler the most flexibility possible. If we can't detect that support,
 * we'll instead try to use 'static inline', which is more widely supported
 * (even though it is not technically c89).
 *
 * Example:
 *	#include <ccan/inline/inline.h>
 *	INLINE_FUNC(int do_something(int a)) {
 *		return a * 2;
 *	}
 */

/**
 * INLINE_SYM - prefix on inline definitions, placed where the symbol will be
 *              emitted
 *
 * Put this in the source file that would normally contain the symbol for the
 * function. Follow it with the function signature.
 *
 * If the compiler doesn't support that, we subsitute a prefix so it looks like
 * a re-definition of a static inline function, which should be mostly ignored
 *
 * Example:
 *	INLINE_SYM(int do_something(int));
 */
#if HAVE_GNU_INLINE
# define INLINE_FUNC(sig) extern inline sig; extern inline sig
# define INLINE_SYM(sig) inline sig
#elif HAVE_STD_INLINE
# define INLINE_FUNC(sig) inline sig
# define INLINE_SYM(sig) extern inline sig
#else
/* do some fallback magic */
# define INLINE_FUNC(sig) static inline sig
# define INLINE_SYM(sig) static inline sig
#endif

#if HAVE_GNU_INLINE
#warning "inline: GNU"
#endif

#if HAVE_STD_INLINE
#warning "inline: STD"
#endif

#endif
