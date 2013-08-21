/*
 * dLeyna
 *
 * Copyright (C) 2013 Intel Corporation. All rights reserved.
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

#include <string.h>

#include "white-list.h"
#include "log.h"

static dleyna_white_list_t g_wl_cb;

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
static void prv_dump_wl_entries(GUPnPWhiteList *wl)
{
	GList *l;

	l = gupnp_white_list_get_entries(wl);

	DLEYNA_LOG_DEBUG_NL();
	DLEYNA_LOG_DEBUG("White List entries:");

	if (l != NULL) {
		while (l != NULL) {
			DLEYNA_LOG_DEBUG("     Entry: [%s].", (char *)l->data);
			l = l->next;
		}
	} else {
		DLEYNA_LOG_DEBUG("     White List Empty.");
	}

	DLEYNA_LOG_DEBUG_NL();
}
#endif

void dleyna_white_list_enable(gboolean enabled, gboolean notify)
{
	if (g_wl_cb.wl != NULL) {
		gupnp_white_list_set_enabled(g_wl_cb.wl, enabled);

		DLEYNA_LOG_DEBUG("White List enabled: %d", enabled);

		if (notify && (g_wl_cb.cb_enabled != NULL))
			g_wl_cb.cb_enabled(g_wl_cb.user_data);
	}
}

void dleyna_white_list_add_entries(GVariant *entries, gboolean notify)
{
	GVariantIter viter;
	gchar *entry;

	if ((entries != NULL) && (g_wl_cb.wl != NULL)) {
		(void) g_variant_iter_init(&viter, entries);

		while (g_variant_iter_next(&viter, "&s", &entry))
			(void) gupnp_white_list_add_entry(g_wl_cb.wl, entry);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
		prv_dump_wl_entries(g_wl_cb.wl);
#endif

		if (notify && (g_wl_cb.cb_entries != NULL))
			g_wl_cb.cb_entries(g_wl_cb.user_data);
	}
}

void dleyna_white_list_remove_entries(GVariant *entries, gboolean notify)
{
	GVariantIter viter;
	gchar *entry;

	if ((entries != NULL) && (g_wl_cb.wl != NULL)) {
		(void) g_variant_iter_init(&viter, entries);

		while (g_variant_iter_next(&viter, "&s", &entry))
			(void) gupnp_white_list_remove_entry(g_wl_cb.wl, entry);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
		prv_dump_wl_entries(g_wl_cb.wl);
#endif

		if (notify && (g_wl_cb.cb_entries != NULL))
			g_wl_cb.cb_entries(g_wl_cb.user_data);
	}
}

void dleyna_white_list_clear(gboolean notify)
{
	if (g_wl_cb.wl != NULL) {
		gupnp_white_list_clear(g_wl_cb.wl);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
		prv_dump_wl_entries(g_wl_cb.wl);
#endif

		if (notify && (g_wl_cb.cb_entries != NULL))
			g_wl_cb.cb_entries(g_wl_cb.user_data);
	}
}

void dleyna_white_list_set_info(dleyna_white_list_t *data)
{
	if (data != NULL)
		memcpy(&g_wl_cb, data, sizeof(dleyna_white_list_t));
	else
		memset(&g_wl_cb, 0, sizeof(dleyna_white_list_t));
}
