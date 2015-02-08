/* Licensed under LGPLv2.1+ - see LICENSE file for details */
#include "pr_log.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <ccan/str/str.h>

#define DEBUG_NEED_INIT INT_MIN
static int debug = DEBUG_NEED_INIT;

bool debug_is(int lvl)
{
	return lvl <= debug_level();
}

int debug_level(void)
{
	if (debug != DEBUG_NEED_INIT)
		return debug;
	char *c = getenv("DEBUG");
	if (!c) {
		debug = CCAN_PR_LOG_DEFAULT_LEVEL;
		return debug;
	}

	debug = atoi(c);
	return debug;
}

static int map_char_to_level(char i)
{
	switch (i) {
# define DL(name, ch, lvl)	\
	case ch:		\
		return lvl;
# include "pr_log_levels.def"
# undef DL
	default:
		return INT_MIN;
	}
}

void pr_log(char const fmt[static 1], ...)
{
	int level = INT_MIN;
	if (fmt[0] == '-' && cisdigit(fmt[1]) && fmt[2] == ':')
		level = - (fmt[1] - '0');
	else if (fmt[0] && fmt[1] == ':') {
		if (cisdigit(fmt[0]))
			level = fmt[0] - '0';
		else
			level = map_char_to_level(fmt[0]);
	}

	if (!debug_is(level))
		return;

	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
}
