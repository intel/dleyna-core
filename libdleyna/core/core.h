/*
 * dLeyna
 *
 * Copyright (C) 2012-2015 Intel Corporation. All rights reserved.
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
 * Rick Bell <richard.s.bell@intel.com>
 *
 */

#include <string.h>

#include "core.h"

gchar *dleyna_core_prv_convert_udn_to_path(const gchar *udn)
{
	gchar *uuid;
	size_t len;
	size_t dest_len;
	size_t i;

	/* This function will generate a valid dbus path from the udn
	 * We are not going to check the UDN validity. We will try to
	 * convert it anyway. To avoid any security problem, we will
	 * check some limits and possibly return only a partial
	 * UDN. For a better understanding, a valid UDN should be:
	 * UDN = "uuid:4Hex-2Hex-2Hex-2Hex-6Hex"
	 *
	 * The convertion rules are:
	 * 1 - An invalid char will be escaped using its hexa representation
	 *     prefixed with '_': 
	 *         Example 1 ':  ' -> '_3A'
	 *         Example 2 '-  ' -> '_2D'
	 * 2 - The max size of the converted UDN can be 3 times the original
	 *     size (if all char are not dbus compliant).
	 *     The max size of a dbus path is an UINT32: G_MAXUINT32
	 *     We will limit the of the converted string size to G_MAXUINT32 / 2
	 *     otherwise we will never have space to generate object path.
	 */

	len = strlen(udn);
	dest_len = MIN(len * 3, G_MAXUINT32 / 2);

	uuid = g_malloc(dest_len + 1);
	i = 0;

	while (*udn && (i < dest_len))
	{
		if (g_ascii_isalnum(*udn) || (*udn == '_'))
			uuid[i++] = *udn;
		else
			i += g_snprintf(uuid + i, dest_len + 1,"_%02x", *udn);

		udn++;
	}


	uuid[i]=0;

	return uuid;
}
