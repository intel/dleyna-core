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

void dleyna_control_point_initialize(const dleyna_connector_t *connector,
				     dleyna_task_processor_t *processor,
				     dleyna_settings_t *settings);
void dleyna_control_point_free(void);

const gchar *dleyna_control_point_server_name(void);
const gchar *dleyna_control_point_server_introspection(void);
const gchar *dleyna_control_point_root_introspection(void);

gboolean dleyna_control_point_start_service(dleyna_connector_id_t connection);

#endif /* DLEYNA_CONTROL_POINT_H__ */
