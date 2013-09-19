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

struct dleyna_white_list_t_ {
	GUPnPWhiteList *gupnp_wl;
};

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

dleyna_white_list_t *dleyna_white_list_new(GUPnPWhiteList *gupnp_wl)
{
	dleyna_white_list_t *wl;

	if (gupnp_wl != NULL) {
		wl =  g_new0(dleyna_white_list_t, 1);

		wl->gupnp_wl = gupnp_wl;
	} else {
		wl = NULL;
		DLEYNA_LOG_DEBUG("Parameter must not be NULL");
	}


	return wl;
}

void dleyna_white_list_delete(dleyna_white_list_t *wl)
{
	g_free(wl);
}

void dleyna_white_list_enable(dleyna_white_list_t *wl,
			      gboolean enabled)
{
	if (wl->gupnp_wl != NULL) {
		gupnp_white_list_set_enabled(wl->gupnp_wl, enabled);

		DLEYNA_LOG_DEBUG("White List enabled: %d", enabled);
	}
}

void dleyna_white_list_add_entries(dleyna_white_list_t *wl,
				   GVariant *entries)
{
	GVariantIter viter;
	gchar *entry;

	DLEYNA_LOG_DEBUG("Enter");

	if ((entries != NULL) && (wl->gupnp_wl != NULL)) {
		(void) g_variant_iter_init(&viter, entries);

		while (g_variant_iter_next(&viter, "&s", &entry))
			(void) gupnp_white_list_add_entry(wl->gupnp_wl, entry);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
		prv_dump_wl_entries(wl->gupnp_wl);
#endif
	}

	DLEYNA_LOG_DEBUG("Exit");
}

void dleyna_white_list_remove_entries(dleyna_white_list_t *wl,
				      GVariant *entries)
{
	GVariantIter viter;
	gchar *entry;

	if ((entries != NULL) && (wl->gupnp_wl != NULL)) {
		(void) g_variant_iter_init(&viter, entries);

		while (g_variant_iter_next(&viter, "&s", &entry))
			(void) gupnp_white_list_remove_entry(wl->gupnp_wl,
							     entry);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
		prv_dump_wl_entries(wl->gupnp_wl);
#endif
	}
}

void dleyna_white_list_clear(dleyna_white_list_t *wl)
{
	if (wl->gupnp_wl != NULL) {
		DLEYNA_LOG_DEBUG("Clear white list");
		gupnp_white_list_clear(wl->gupnp_wl);

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_DEBUG
		prv_dump_wl_entries(wl->gupnp_wl);
#endif
	}
}
