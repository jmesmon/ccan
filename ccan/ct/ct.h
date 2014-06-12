/* CC0 (Public domain) - see LICENSE file for details */
#ifndef CCAN_CT_H_
#define CCAN_CT_H_

#include "config.h"

#if HAVE_TYPEOF && HAVE_BUILTIN_TYPES_COMPATIBLE_P
# define VAR_COMPAT_TYPE(_v, _t) __builtin_types_compatible_p(typeof(_v), _t)
#else
# define VAR_COMPAT_TYPE(_v, _t) (sizeof(_v) == sizeof(_t))
#endif

#if HAVE_BUILTIN_CHOOSE_EXPR
# define C_CHOOSE(q, a, b) __builtin_choose_expr(q, a, b)
# define C_CHOOSE_END (void)0
#else
# define C_CHOOSE(q, a, b) ((q) ? (a) : (b))
# define C_CHOOSE_END C_CHOOSE_FAILED_TO_RESOLVE_AT_COMPILE_TIME
extern int C_CHOOSE_FAILED_TO_RESOLVE_AT_COMPILE_TIME;
#endif

#endif
