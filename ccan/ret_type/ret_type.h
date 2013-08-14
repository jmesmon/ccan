#ifndef CCAN_RET_TYPE_H
#define CCAN_RET_TYPE_H
#include <limits.h>

#ifndef __WORDSIZE
# ifdef __AVR__
#   define __WORDSIZE 8
# else
#   error "Unknown __WORDSIZE"
# endif
#endif

#if __WORDSIZE == 8
typedef uint8_t u8_ret_t;
#elif __WORDSIZE == 32 || __WORDSIZE == 64
typedef int     u8_ret_t;
#else
# error "I don't know how to handle this __WORDSIZE"
#endif

#endif
