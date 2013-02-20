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

#ifndef DLEYNA_TASK_ATOM_H__
#define DLEYNA_TASK_ATOM_H__

typedef struct dleyna_task_queue_key_t_ dleyna_task_queue_key_t;

struct dleyna_task_atom_t_ {
	const dleyna_task_queue_key_t *queue_id;
};
typedef struct dleyna_task_atom_t_ dleyna_task_atom_t;

#endif /* DLEYNA_TASK_ATOM_H__ */
