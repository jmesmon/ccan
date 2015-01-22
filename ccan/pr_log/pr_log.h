#ifndef CCAN_DEBUG_H_
#define CCAN_DEBUG_H_

#include <stdbool.h>
#include <ccan/compiler/compiler.h>

#ifdef NDEBUG
static PRINTF_FMT(1,2) inline void pr_log(char const *fmt, ...) {
	(void)fmt;
}
static inline bool debug_is(int lvl) { (void)lvl; return false; }
static inline int debug_level(void) { return 0; }
#else
void PRINTF_FMT(1,2) pr_log(char const *fmt, ...);
bool debug_is(int lvl);
int debug_level(void);
#endif

/* TODO: generate the following defines somehow */
#define LOG_EMERG "-4:"
#define LOG_ALERT "-3:"
#define LOG_CRIT  "-2:"
#define LOG_ERROR "-1:"
#define LOG_NOTICE "0:"
#define LOG_INFO   "1:"
#define LOG_DEBUG  "2:"

#define pr_emerg(...)  pr_log(LOG_EMERG  __VA_ARGS__)
#define pr_alert(...)  pr_log(LOG_ALERT  __VA_ARGS__)
#define pr_crit(...)   pr_log(LOG_CRIT   __VA_ARGS__)
#define pr_error(...)  pr_log(LOG_ERROR  __VA_ARGS__)
#define pr_notice(...) pr_log(LOG_NOTICE __VA_ARGS__)
#define pr_info(...)   pr_log(LOG_INFO   __VA_ARGS__)
#define pr_debug(...)  pr_log(LOG_DEBUG  __VA_ARGS__)

#endif
