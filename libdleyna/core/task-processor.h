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
 * Regis Merlino <regis.merlino@intel.com>
 *
 */

#ifndef DLEYNA_TASK_PROCESSOR_H__
#define DLEYNA_TASK_PROCESSOR_H__

#include <gio/gio.h>
#include <glib.h>

#include "task-atom.h"

enum dleyna_task_queue_flag_mask_ {
	DLEYNA_TASK_QUEUE_FLAG_NONE = 0,
	DLEYNA_TASK_QUEUE_FLAG_AUTO_START = 1,
	DLEYNA_TASK_QUEUE_FLAG_AUTO_REMOVE = 1 << 1,
};
typedef enum dleyna_task_queue_flag_mask_ dleyna_task_queue_flag_mask;

typedef struct dleyna_task_processor_t_ dleyna_task_processor_t;

typedef void (*dleyna_task_process_cb_t)(dleyna_task_atom_t *task,
					 gpointer user_data);

typedef void (*dleyna_task_cancel_cb_t)(dleyna_task_atom_t *task,
					gpointer user_data);

typedef void (*dleyna_task_delete_cb_t)(dleyna_task_atom_t *task,
					gpointer user_data);

typedef void (*dleyna_task_finally_cb_t)(gboolean cancelled,
					 gpointer user_data);

dleyna_task_processor_t *dleyna_task_processor_new(GSourceFunc on_quit_cb);

void dleyna_task_processor_free(dleyna_task_processor_t *processor);

void dleyna_task_processor_set_quitting(dleyna_task_processor_t *processor);

const dleyna_task_queue_key_t *dleyna_task_processor_add_queue(
				dleyna_task_processor_t *processor,
				const gchar *source,
				const gchar *sink,
				guint32 flags,
				dleyna_task_process_cb_t task_process_cb,
				dleyna_task_cancel_cb_t task_cancel_cb,
				dleyna_task_delete_cb_t task_delete_cb);

const dleyna_task_queue_key_t *dleyna_task_processor_lookup_queue(
				const dleyna_task_processor_t *processor,
				const gchar *source,
				const gchar *sink);

void dleyna_task_processor_cancel_queue(
				const dleyna_task_queue_key_t *queue_id);

void dleyna_task_processor_remove_queues_for_source(
					dleyna_task_processor_t *processor,
					const gchar *source);

void dleyna_task_processor_remove_queues_for_sink(
					dleyna_task_processor_t *processor,
					const gchar *sink);

void dleyna_task_queue_start(const dleyna_task_queue_key_t *queue_id);

void dleyna_task_queue_add_task(const dleyna_task_queue_key_t *queue_id,
			     dleyna_task_atom_t *task);

void dleyna_task_queue_task_completed(const dleyna_task_queue_key_t *queue_id);

void dleyna_task_queue_set_finally(const dleyna_task_queue_key_t *queue_id,
				   dleyna_task_finally_cb_t finally_cb);

void dleyna_task_queue_set_user_data(const dleyna_task_queue_key_t *queue_id,
				     gpointer user_data);

gpointer dleyna_task_queue_get_user_data(
				const dleyna_task_queue_key_t *queue_id);

const gchar *dleyna_task_queue_get_source(
				const dleyna_task_queue_key_t *queue_id);

#endif /* DLEYNA_TASK_PROCESSOR_H__ */
