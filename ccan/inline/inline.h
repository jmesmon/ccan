/* CC0 license (public domain) - see LICENSE file for details */
#ifndef CCAN_INLINE_H_
#define CCAN_INLINE_H_

#include "config.h"

/**
 * INLINE_DECL - prefix on inline declarations
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
 *	INLINE_DECL(int do_something(int a)) {
 *		return a * 2;
 *	}
 */

/**
 * INLINE_DEFN - prefix on inline definitions
 *
 * Put this in the source file that would normally contain the symbol for the
 * function. Follow it with the function signature.
 *
 * If the compiler doesn't support that, we subsitute a prefix so it looks like
 * a re-definition of a static inline function, which should be mostly ignored
 *
 * Example:
 *	INLINE_DEFN(int do_something(int));
 */
#if HAVE_GNU_INLINE
# define INLINE_DECL(sig) extern inline sig; extern inline sig
# define INLINE_DEFN(sig) inline sig
#elif HAVE_STD_INLINE
# define INLINE_DECL(sig) inline sig
# define INLINE_DEFN(sig) extern inline sig
#else
/* do some fallback magic */
# define INLINE_DECL(sig) static inline sig
# define INLINE_DEFN(sig) static inline sig
#endif

#endif
