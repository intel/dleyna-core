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

#ifndef DLEYNA_WHITE_LIST_H__
#define DLEYNA_WHITE_LIST_H__

#include <glib.h>
#include <libgupnp/gupnp-white-list.h>

typedef void (*dleyna_white_list_notify_t)(gpointer user_data);

typedef struct dleyna_white_list_t_ dleyna_white_list_t;

struct dleyna_white_list_t_ {
	GUPnPWhiteList *wl;
	dleyna_white_list_notify_t cb_enabled;
	dleyna_white_list_notify_t cb_entries;
	gpointer user_data;
};

void dleyna_white_list_enable(gboolean enabled, gboolean notify);

void dleyna_white_list_add_entries(GVariant *entries, gboolean notify);

void dleyna_white_list_remove_entries(GVariant *entries, gboolean notify);

void dleyna_white_list_clear(gboolean notify);

void dleyna_white_list_set_info(dleyna_white_list_t *data);

#endif /* DLEYNA_WHITE_LIST_H__ */
