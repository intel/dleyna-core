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

typedef struct dleyna_white_list_t_ dleyna_white_list_t;

dleyna_white_list_t *dleyna_white_list_new(GUPnPWhiteList *gupnp_wl);

void dleyna_white_list_delete(dleyna_white_list_t *wl);

void dleyna_white_list_enable(dleyna_white_list_t *wl, gboolean enabled);

void dleyna_white_list_add_entries(dleyna_white_list_t *wl, GVariant *entries);

void dleyna_white_list_remove_entries(dleyna_white_list_t *wl,
				      GVariant *entries);

void dleyna_white_list_clear(dleyna_white_list_t *wl);

#endif /* DLEYNA_WHITE_LIST_H__ */
