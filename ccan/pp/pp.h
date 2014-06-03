#ifndef CCAN_PP_
#define CCAN_PP_
/* Licenced as CC0 (Public Domain) - see LICENSE file for details */

#define EMPTY /* nothing */

#define EV_(a) a
#define EV(a) EV_(a)

#define CAT2_(a,b) a##b
#define CAT2(a,b) CAT2_(a,b)

#define CAT3_(a,b,c) a##b##c
#define CAT3(a,b,c) CAT3_(a,b,c)

#define STR_(a) #a
#define STR(a) STR_(a)

#endif
