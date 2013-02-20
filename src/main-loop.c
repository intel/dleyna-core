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
 * Regis Merlino <regis.merlino@intel.com>
 *
 */

#include <glib.h>
#include <string.h>

#include "connector-mgr.h"
#include "control-point.h"
#include "error.h"
#include "log.h"
#include "main-loop.h"
#include "settings.h"
#include "task-processor.h"

typedef struct dleyna_daemon_context_t_ dleyna_daemon_context_t;
struct dleyna_daemon_context_t_ {
	gboolean error;
	GMainLoop *main_loop;
	dleyna_connector_id_t connection;
	dleyna_task_processor_t *processor;
	dleyna_settings_t *settings;
	const dleyna_connector_t *connector;
};

static dleyna_daemon_context_t g_context;

static gboolean prv_context_quit_cb(gpointer user_data)
{
	DLEYNA_LOG_DEBUG("Quitting");

	g_main_loop_quit(g_context.main_loop);

	return FALSE;
}

static gboolean prv_context_init(const gchar *server)
{
	gboolean retval = TRUE;
	const gchar *connector;

	memset(&g_context, 0, sizeof(g_context));

	g_context.processor = dleyna_task_processor_new(prv_context_quit_cb);

	dleyna_settings_new(server, &g_context.settings);

	connector = dleyna_settings_connector_name(g_context.settings);
	g_context.connector = dleyna_connector_mgr_load(connector);

	if (!g_context.connector)
		retval = FALSE;

	return retval;
}

static void prv_connector_acquired(dleyna_connector_id_t connection)
{
	g_context.connection = connection;

	if (!dleyna_control_point_start_service(connection)) {
		g_context.error = TRUE;
		g_main_loop_quit(g_context.main_loop);
	}
}

static void prv_name_lost(dleyna_connector_id_t connection)
{
	g_context.connection = NULL;
	dleyna_task_processor_set_quitting(g_context.processor);
}

static void prv_msu_context_free(void)
{
	dleyna_task_processor_free(g_context.processor);

	if (g_context.connector) {
		g_context.connector->shutdown();
		dleyna_connector_mgr_release(g_context.connector);
	}

	if (g_context.main_loop)
		g_main_loop_unref(g_context.main_loop);

	if (g_context.settings)
		dleyna_settings_delete(g_context.settings);
}

int dleyna_main_loop_start(char *server, gpointer user_data)
{
	int retval = 1;

	g_type_init();

	dleyna_log_init(server);

	if (!prv_context_init(server))
		goto out;

	if (!g_context.connector->initialize(
				dleyna_control_point_server_introspection(),
				dleyna_control_point_root_introspection(),
				dleyna_error_quark(),
				user_data))
		goto out;

	g_context.main_loop = g_main_loop_new(NULL, FALSE);

	g_context.connector->connect(dleyna_control_point_server_name(),
				     prv_connector_acquired,
				     prv_name_lost);


	dleyna_control_point_initialize(g_context.connector,
					g_context.processor,
					g_context.settings);

	g_main_loop_run(g_context.main_loop);
	if (g_context.error)
		goto on_error;

	retval = 0;

on_error:

	dleyna_control_point_free();

out:

	prv_msu_context_free();

	dleyna_log_finalize();

	return retval;
}

static gboolean prv_set_quitting(gpointer user_data)
{
	dleyna_task_processor_set_quitting(g_context.processor);

	return FALSE;
}

void dleyna_main_loop_quit(void)
{
	g_idle_add_full(G_PRIORITY_HIGH_IDLE, prv_set_quitting, NULL, NULL);
}
