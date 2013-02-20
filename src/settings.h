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

#ifndef DLEYNA_SETTINGS_H__
#define DLEYNA_SETTINGS_H__

#include <glib.h>

typedef struct dleyna_settings_t_ dleyna_settings_t;

void dleyna_settings_new(const gchar *server, dleyna_settings_t **settings);
void dleyna_settings_delete(dleyna_settings_t *settings);

gboolean dleyna_settings_is_never_quit(dleyna_settings_t *settings);
const gchar *dleyna_settings_connector_name(dleyna_settings_t *settings);

#endif /* DLEYNA_SETTINGS_H__ */
