/*
 * dLeyna
 *
 * Copyright (C) 2012-2013 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Ludovic Ferrandis <ludovic.ferrandis@intel.com>
 *
 */

#define _GNU_SOURCE

#include <stdarg.h>
#include <string.h>

#include "log.h"

typedef struct dleyna_log_t_ dleyna_log_t;
struct dleyna_log_t_ {
	int old_mask;
	int mask;
	dleyna_log_type_t log_type;
	GLogLevelFlags flags;
	GLogFunc old_handler;
	gchar *log_domain;
};

static dleyna_log_t s_log_context;

static void prv_get_mf(int log_level, int *mask, GLogLevelFlags *flags)
{
	*mask = 0;
	*flags = 0;

	if (log_level & DLEYNA_LOG_LEVEL_ERROR) {
		*mask |= LOG_MASK(LOG_ERR);
		*flags |= G_LOG_LEVEL_ERROR;
	}

	if (log_level & DLEYNA_LOG_LEVEL_CRITICAL) {
		*mask |= LOG_MASK(LOG_CRIT);
		*flags |= G_LOG_LEVEL_CRITICAL;
	}

	if (log_level & DLEYNA_LOG_LEVEL_WARNING) {
		*mask |= LOG_MASK(LOG_WARNING);
		*flags |= G_LOG_LEVEL_WARNING;
	}

	if (log_level & DLEYNA_LOG_LEVEL_MESSAGE) {
		*mask |= LOG_MASK(LOG_NOTICE);
		*flags |= G_LOG_LEVEL_MESSAGE;
	}

	if (log_level & DLEYNA_LOG_LEVEL_INFO) {
		*mask |= LOG_MASK(LOG_INFO);
		*flags |= G_LOG_LEVEL_INFO;
	}

	if (log_level & DLEYNA_LOG_LEVEL_DEBUG) {
		*mask |= LOG_MASK(LOG_DEBUG);
		*flags |= G_LOG_LEVEL_DEBUG;
	}

	if (*flags)
		*flags |= G_LOG_FLAG_RECURSION | G_LOG_FLAG_FATAL;
}

static void prv_set_flags_from_param(void)
{
	int mask;
	GLogLevelFlags flags;

	prv_get_mf(DLEYNA_LOG_LEVEL, &mask, &flags);

	s_log_context.mask = mask;
	s_log_context.flags = flags;
	s_log_context.log_type = DLEYNA_LOG_TYPE;
}

static void prv_handler(const gchar *log_domain,
			GLogLevelFlags log_level,
			const gchar *message,
			gpointer data)
{
	dleyna_log_t *log_context = (dleyna_log_t *)(data);

	if (g_strcmp0(log_domain, s_log_context.log_domain))
		return;

	if (log_context->flags & log_level)
		g_log_default_handler(log_domain, log_level, message, data);
}

void dleyna_log_update_type_level(dleyna_log_type_t log_type, int log_level)
{
	int mask;
	int compile_mask;
	GLogLevelFlags flags;
	GLogLevelFlags compile_flags;

	s_log_context.log_type = log_type;

	prv_get_mf(log_level, &mask, &flags);
	prv_get_mf(DLEYNA_LOG_LEVEL, &compile_mask, &compile_flags);

	/* log level read from conf file is a subset of log level
	 * set at compile time.
	 * Only keep subset flags from compile flags */

	mask &= compile_mask;
	flags &= compile_flags;

	s_log_context.mask = mask;
	s_log_context.flags = flags;

	DLEYNA_LOG_INFO("Type[%d] Level[0x%02X] Mask[0x%02X] Flags[0x%02X]",
			log_type, log_level, mask, flags);

	(void) setlogmask(mask);
}

void dleyna_log_init(const char *program)
{
	int option = LOG_NDELAY | LOG_PID;
	int old;

#ifdef DLEYNA_DEBUG_ENABLED
	option |= LOG_PERROR | LOG_CONS;
#endif

	memset(&s_log_context, 0, sizeof(s_log_context));
	s_log_context.log_domain = g_strdup(program);
	prv_set_flags_from_param();

	openlog(basename(program), option, LOG_DAEMON);

	old = setlogmask(LOG_MASK(LOG_INFO));
	syslog(LOG_INFO, "dLeyna version %s", VERSION);
	(void) setlogmask(s_log_context.mask);

	s_log_context.old_mask = old;
	s_log_context.old_handler = g_log_set_default_handler(
							prv_handler,
							&s_log_context);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_INFO
	if (s_log_context.log_type != DLEYNA_LOG_TYPE_SYSLOG)
		DLEYNA_LOG_INFO("dLeyna version %s", VERSION);
#endif
}

void dleyna_log_finalize(void)
{
	(void) setlogmask(LOG_MASK(LOG_INFO));
	syslog(LOG_INFO, "dLeyna: Exit");

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_INFO
	if (s_log_context.log_type != DLEYNA_LOG_TYPE_SYSLOG)
		DLEYNA_LOG_INFO("dLeyna: Exit");
#endif

	(void) g_log_set_default_handler(s_log_context.old_handler, NULL);

	(void) setlogmask(s_log_context.old_mask);
	closelog();

	g_free(s_log_context.log_domain);

	memset(&s_log_context, 0, sizeof(s_log_context));
}

void dleyna_log_trace(int priority, GLogLevelFlags flags,
		      const char *format, ...)
{
	va_list args;

	va_start(args, format);

	switch (s_log_context.log_type) {
	case DLEYNA_LOG_TYPE_SYSLOG:
		if (s_log_context.mask)
			vsyslog(priority, format, args);
		break;
	case DLEYNA_LOG_TYPE_GLIB:
		if (s_log_context.flags)
			g_logv(s_log_context.log_domain, flags, format, args);
		break;
	case DLEYNA_LOG_TYPE_FILE:
		break;
	default:
		break;
	}

	va_end(args);
}
