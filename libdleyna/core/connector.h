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

#ifndef DLEYNA_CONNECTOR_H__
#define DLEYNA_CONNECTOR_H__

#include <glib.h>

typedef void *dleyna_connector_id_t;
typedef void *dleyna_connector_msg_id_t;

typedef gboolean (*dleyna_connector_initialize_t)(const gchar *server_info,
						  const gchar *root_info,
						  GQuark error_quark,
						  gpointer user_data);
typedef void (*dleyna_connector_shutdown_t)(void);

typedef void (*dleyna_connector_connected_cb_t)
					(dleyna_connector_id_t connection);
typedef void (*dleyna_connector_disconnected_cb_t)(
					dleyna_connector_id_t connection);
typedef void (*dleyna_connector_connect_t)(
			const gchar *server_name,
			dleyna_connector_connected_cb_t connected_cb,
			dleyna_connector_disconnected_cb_t disconnected_cb);
typedef void (*dleyna_connector_disconnect_t)(void);

typedef void (*dleyna_connector_client_new_cb_t)(const gchar *client_name);
typedef void (*dleyna_connector_client_lost_cb_t)(const gchar *client_name);
typedef gboolean (*dleyna_connector_watch_client_t)(const gchar *client_name);
typedef void (*dleyna_connector_unwatch_client_t)(const gchar *client_name);
typedef void (*dleyna_connector_set_client_lost_cb_t)
				(dleyna_connector_client_lost_cb_t lost_cb);

typedef void (*dleyna_connector_dispatch_cb_t)(
					dleyna_connector_id_t connection,
					const gchar *sender,
					const gchar *object_id,
					const gchar *interface,
					const gchar *method,
					GVariant *parameters,
					dleyna_connector_msg_id_t message_id);
typedef gboolean (*dleyna_connector_interface_filter_cb_t)(
						const gchar *object_path,
						const gchar *node,
						const gchar *interface);
typedef guint (*dleyna_connector_publish_object_t)(
			dleyna_connector_id_t connection,
			const gchar *object_path,
			gboolean root,
			const gchar *interface_name,
			const dleyna_connector_dispatch_cb_t *cb_table_1);
typedef guint (*dleyna_connector_publish_subtree_t)(
				dleyna_connector_id_t connection,
				const gchar *object_path,
				const dleyna_connector_dispatch_cb_t *cb_table,
				guint cb_table_size,
				dleyna_connector_interface_filter_cb_t cb);
typedef void (*dleyna_connector_unpublish_object_t)(
					dleyna_connector_id_t connection,
					guint object_id);
typedef void (*dleyna_connector_unpublish_subtree_t)(
					dleyna_connector_id_t connection,
					guint object_id);

typedef void (*dleyna_connector_return_response_t)(
					dleyna_connector_msg_id_t message_id,
					GVariant *parameters);
typedef void (*dleyna_connector_return_error_t)(
					dleyna_connector_msg_id_t message_id,
					const GError *error);
typedef gboolean (*dleyna_connector_notify_t)(dleyna_connector_id_t connection,
					   const gchar *object_path,
					   const gchar *interface_name,
					   const gchar *notification_name,
					   GVariant *parameters,
					   GError **error);

typedef struct dleyna_connector_t_ dleyna_connector_t;
struct dleyna_connector_t_ {
	dleyna_connector_initialize_t initialize;
	dleyna_connector_shutdown_t shutdown;
	dleyna_connector_connect_t connect;
	dleyna_connector_disconnect_t disconnect;
	dleyna_connector_watch_client_t watch_client;
	dleyna_connector_unwatch_client_t unwatch_client;
	dleyna_connector_set_client_lost_cb_t set_client_lost_cb;
	dleyna_connector_publish_object_t publish_object;
	dleyna_connector_publish_subtree_t publish_subtree;
	dleyna_connector_unpublish_object_t unpublish_object;
	dleyna_connector_unpublish_subtree_t unpublish_subtree;
	dleyna_connector_return_response_t return_response;
	dleyna_connector_return_error_t return_error;
	dleyna_connector_notify_t notify;
};

typedef const dleyna_connector_t *(*dleyna_connector_get_interface_t)(void);

#endif /* DLEYNA_CONNECTOR_H__ */
