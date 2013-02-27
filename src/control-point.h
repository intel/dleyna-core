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

#ifndef DLEYNA_CONTROL_POINT_H__
#define DLEYNA_CONTROL_POINT_H__

#include <glib.h>

#include "connector.h"
#include "settings.h"
#include "task-processor.h"

typedef void (*dleyna_control_point_initialize_t)(
					const dleyna_connector_t *connector,
					dleyna_task_processor_t *processor,
					dleyna_settings_t *settings);
typedef void (*dleyna_control_point_free_t)(void);

typedef const gchar *(*dleyna_control_point_server_name_t)(void);
typedef const gchar *(*dleyna_control_point_server_introspection_t)(void);
typedef const gchar *(*dleyna_control_point_root_introspection_t)(void);

typedef gboolean (*dleyna_control_point_start_service_t)(
					dleyna_connector_id_t connection);
typedef void (*dleyna_control_point_stop_service_t)(void);

typedef struct dleyna_control_point_t_ dleyna_control_point_t;
struct dleyna_control_point_t_ {
	dleyna_control_point_initialize_t initialize;
	dleyna_control_point_free_t free;
	dleyna_control_point_server_name_t server_name;
	dleyna_control_point_server_introspection_t server_introspection;
	dleyna_control_point_root_introspection_t root_introspection;
	dleyna_control_point_start_service_t start_service;
	dleyna_control_point_stop_service_t stop_service;
};

#endif /* DLEYNA_CONTROL_POINT_H__ */
