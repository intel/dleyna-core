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
 * Mark Ryan <mark.d.ryan@intel.com>
 *
 */

#ifndef DLEYNA_ERROR_H__
#define DLEYNA_ERROR_H__

#include <glib.h>

enum dleyna_error_t_ {
	DLEYNA_ERROR_BAD_PATH,
	DLEYNA_ERROR_OBJECT_NOT_FOUND,
	DLEYNA_ERROR_BAD_QUERY,
	DLEYNA_ERROR_OPERATION_FAILED,
	DLEYNA_ERROR_BAD_RESULT,
	DLEYNA_ERROR_UNKNOWN_INTERFACE,
	DLEYNA_ERROR_UNKNOWN_PROPERTY,
	DLEYNA_ERROR_DEVICE_NOT_FOUND,
	DLEYNA_ERROR_DIED,
	DLEYNA_ERROR_CANCELLED,
	DLEYNA_ERROR_NOT_SUPPORTED,
	DLEYNA_ERROR_LOST_OBJECT,
	DLEYNA_ERROR_BAD_MIME,
	DLEYNA_ERROR_HOST_FAILED,
	DLEYNA_ERROR_IO
};
typedef enum dleyna_error_t_ dleyna_error_t;

#define DLEYNA_SERVER_ERROR (dleyna_error_quark())
GQuark dleyna_error_quark(void);

#endif /* DLEYNA_ERROR_H__ */
