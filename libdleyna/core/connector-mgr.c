/*
 * dLeyna
 *
 * Copyright (C) 2012-2017 Intel Corporation. All rights reserved.
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
 * Regis Merlino <regis.merlino@intel.com>
 *
 */

#include <gmodule.h>
#include <string.h>

#include "connector-mgr.h"
#include "log.h"

static GHashTable *g_connectors;

const dleyna_connector_t *dleyna_connector_mgr_load(const gchar *name)
{
	GModule *module;
	const dleyna_connector_t *connector;
	dleyna_connector_get_interface_t get_interface;
	gchar *path;
	const gchar *connector_path;
	gchar **connector_path_list;
	gsize i;

	DLEYNA_LOG_DEBUG("Enter");

	connector_path = g_getenv ("DLEYNA_CONNECTOR_PATH");
	if (!connector_path) {
		DLEYNA_LOG_DEBUG ("DLEYNA_CONNECTOR_PATH not set");
		connector_path = CONNECTOR_DIR;
	} else {
		DLEYNA_LOG_DEBUG ("DLEYNA_CONNECTOR_PATH set to %s", connector_path);
	}

	connector_path_list = g_strsplit (connector_path, G_SEARCHPATH_SEPARATOR_S, 0);

	for (i = 0; connector_path_list[i]; i++) {
		path = g_strdup_printf("%s/%s%s.so", connector_path_list[i],
				       DLEYNA_CONNECTOR_LIB_PATTERN, name);
		module = g_module_open(path, G_MODULE_BIND_LAZY);
		g_free(path);

		if (module) {
			if (!g_connectors)
				g_connectors = g_hash_table_new(g_direct_hash,
								g_direct_equal);

			if (g_module_symbol(module, "dleyna_connector_get_interface",
					    (gpointer *)&get_interface)) {
				connector = get_interface();
				g_hash_table_insert(g_connectors, (gpointer)connector,
						    module);

				break;
			} else {
				connector = NULL;
				g_module_close(module);
				DLEYNA_LOG_CRITICAL(
						"Connector '%s' entry point not found",
						name);
			}

		}
	}

	g_strfreev (connector_path_list);

	if (!module) {
		connector = NULL;
		DLEYNA_LOG_CRITICAL("Connector '%s' not found", name);
	}

	DLEYNA_LOG_DEBUG("Exit");

	return connector;
}

void dleyna_connector_mgr_release(const dleyna_connector_t *connector)
{
	GModule *module;

	DLEYNA_LOG_DEBUG("Enter");

	module = g_hash_table_lookup(g_connectors, connector);
	if (module) {
		g_module_close(module);

		g_hash_table_remove(g_connectors, connector);
		if (g_hash_table_size(g_connectors) == 0) {
			g_hash_table_unref(g_connectors);
			g_connectors = NULL;
		}
	}

	DLEYNA_LOG_DEBUG("Exit");
}
