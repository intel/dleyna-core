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

#ifndef DLEYNA_LOG_H__
#define DLEYNA_LOG_H__

#include <glib.h>
#include <syslog.h>

enum dleyna_log_type_t_ {
	DLEYNA_LOG_TYPE_SYSLOG = 0,
	DLEYNA_LOG_TYPE_GLIB,
	DLEYNA_LOG_TYPE_FILE
};
typedef enum dleyna_log_type_t_ dleyna_log_type_t;

void dleyna_log_init(const char *program, const char *version);

void dleyna_log_finalize(void);

void dleyna_log_update_type_level(dleyna_log_type_t log_type, int log_level);

void dleyna_log_trace(int priority, GLogLevelFlags flags,
		      const char *format, ...)
					__attribute__((format(printf, 3, 4)));

/* Generic Logging macro
 */
#ifdef DLEYNA_DEBUG_ENABLED
	#define DLEYNA_LOG_HELPER(priority, flags, fmt, ...)    \
			dleyna_log_trace(priority, flags, "%s : %s() --- " fmt,\
				      __FILE__, __func__, ## __VA_ARGS__);
#else
	#define DLEYNA_LOG_HELPER(priority, flags, fmt, ...) \
			dleyna_log_trace(priority, flags, fmt, ## __VA_ARGS__);
#endif


/* Logging macro for error messages
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_ERROR
	#define DLEYNA_LOG_ERROR(...) \
		DLEYNA_LOG_HELPER(LOG_ERR, G_LOG_LEVEL_ERROR, __VA_ARGS__, 0)
#else
	#define DLEYNA_LOG_ERROR(...)
#endif


/* Logging macro for critical messages
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_CRITICAL
	#define DLEYNA_LOG_CRITICAL(...) \
		DLEYNA_LOG_HELPER(LOG_CRIT, G_LOG_LEVEL_CRITICAL, \
				  __VA_ARGS__, 0)
#else
	#define DLEYNA_LOG_CRITICAL(...)
#endif


/* Logging macro for warning messages
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_WARNING
	#define DLEYNA_LOG_WARNING(...) \
		DLEYNA_LOG_HELPER(LOG_WARNING, G_LOG_LEVEL_WARNING, \
				  __VA_ARGS__, 0)
#else
	#define DLEYNA_LOG_WARNING(...)
#endif


/* Logging macro for messages
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_MESSAGE
	#define DLEYNA_LOG_MESSAGE(...) \
		DLEYNA_LOG_HELPER(LOG_NOTICE, G_LOG_LEVEL_MESSAGE, \
				  __VA_ARGS__, 0)
#else
	#define DLEYNA_LOG_MESSAGE(...)
#endif


/* Logging macro for informational messages
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_INFO
	#define DLEYNA_LOG_INFO(...) \
		DLEYNA_LOG_HELPER(LOG_INFO, G_LOG_LEVEL_INFO, __VA_ARGS__, 0)
#else
	#define DLEYNA_LOG_INFO(...)
#endif


/* Logging macro for debug messages
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
	#define DLEYNA_LOG_DEBUG(...) \
		DLEYNA_LOG_HELPER(LOG_DEBUG, G_LOG_LEVEL_DEBUG, __VA_ARGS__, 0)
#else
	#define DLEYNA_LOG_DEBUG(...)
#endif


/* Logging macro to display an empty line
 */
#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
	#define DLEYNA_LOG_DEBUG_NL() \
			dleyna_log_trace(LOG_DEBUG, G_LOG_LEVEL_DEBUG, " ");
#else
	#define DLEYNA_LOG_DEBUG_NL()
#endif

#endif /* DLEYNA_LOG_H__ */
