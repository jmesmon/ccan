/* GNU LGPL - see LICENSE file for details */
#ifndef CCAN_VEC_IO_H
#define CCAN_VEC_IO_H

#include <ccan/vec/vec.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * vec_vprintf - append a formatted string to a vec
 * @vec: &VEC(char): a character vector to append to
 * @fmt: char *: a printf-style format string
 * @ap: va_list: a va_list corresponding to @fmt
 *
 * NOTE: only works on VEC(char).
 *
 * Returns (int) number of characters appended on success, or a negative error
 * value on failure.
 *
 * At the moment, can only fail if either memory allocation fails or snprintf
 * fails.
 */
#define vec_vprintf(vec, fmt, ap) vec_vprintf_(tcon_unwrap(tcon_check_ptr(vec, elem, "")), fmt, ap)
WARN_UNUSED_RESULT
static inline int vec_vprintf_(struct vec_ *vec, const char *fmt, va_list ap)
{
    char n[1];
    va_list lap;
    va_copy(lap, ap);
    int ct = vsnprintf(n, sizeof(n), fmt, lap);
    if (ct < 0)
        return ct;

    /*
     * sprintf always appends a '\0', but we typically don't want to include
     * that. To avoid, allocate enough space for +1 extra, but don't add it to used
     */
    int r = vec_ensure_space_n_(vec, ct + 1, 1);
    if (r < 0)
        return r;

    vsprintf((char *)vec->data + vec->used, fmt, ap);
    vec->used += ct;
    return ct;
}

/**
 * vec_printf - append a formatted string to a vec
 * @vec: &VEC(char): a character vector to append to
 * @fmt: char *: a printf-style format string
 * @...: arguments appropriate
 *
 * NOTE: only works on VEC(char).
 *
 * Returns (int) number of characters appended on success, or a negative error
 * value on failure.
 *
 * At the moment, can only fail if either memory allocation fails or snprintf
 * fails.
 */
#define vec_printf(vec, ...) vec_printf_(tcon_unwrap(tcon_check_ptr(vec, elem, "")), __VA_ARGS__)
PRINTF_FMT(2, 3)
WARN_UNUSED_RESULT
static inline int vec_printf_(struct vec_ *vec, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int r = vec_vprintf_(vec, fmt, ap);
    va_end(ap);
    return r;
}

#endif
