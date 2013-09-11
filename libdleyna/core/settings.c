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

#include <string.h>
#include <gio/gio.h>

#include "log.h"
#include "settings.h"
#include "white-list.h"

struct dleyna_settings_t_ {
	GKeyFile *keyfile;
	gchar *file_name;
	gchar *file_path;

	/* Global section */
	gboolean never_quit;
	gchar *connector_name;

	/* Log section */
	dleyna_log_type_t log_type;
	int log_level;

	/* netf section */
	gboolean netf_enabled;
	GVariant *netf_entries;
};

#define DLEYNA_SETTINGS_GROUP_GENERAL		"general"
#define DLEYNA_SETTINGS_KEY_NEVER_QUIT		"never-quit"
#define DLEYNA_SETTINGS_KEY_CONNECTOR_NAME	"connector-name"

#define DLEYNA_SETTINGS_GROUP_LOG		"log"
#define DLEYNA_SETTINGS_KEY_LOG_TYPE		"log-type"
#define DLEYNA_SETTINGS_KEY_LOG_LEVEL		"log-level"

#define DLEYNA_SETTINGS_GROUP_NETF		"netf"
#define DLEYNA_SETTINGS_KEY_NETF_ENABLED	"netf-enabled"
#define DLEYNA_SETTINGS_KEY_NETF_LIST		"netf-list"

#define DLEYNA_SETTINGS_DEFAULT_NEVER_QUIT	DLEYNA_NEVER_QUIT
#define DLEYNA_SETTINGS_DEFAULT_CONNECTOR_NAME	DLEYNA_CONNECTOR_NAME
#define DLEYNA_SETTINGS_DEFAULT_LOG_TYPE	DLEYNA_LOG_TYPE
#define DLEYNA_SETTINGS_DEFAULT_LOG_LEVEL	DLEYNA_LOG_LEVEL

#if DLEYNA_LOG_LEVEL & DLEYNA_LOG_LEVEL_INFO
#define DLEYNA_SETTINGS_LOG_KEYS(sys, loc, settings) \
do { \
	gchar *str = NULL; \
	\
	DLEYNA_LOG_DEBUG_NL(); \
	DLEYNA_LOG_INFO("Load file [%s]", (settings)->file_path); \
	DLEYNA_LOG_DEBUG_NL(); \
	\
	DLEYNA_LOG_INFO("[General settings]"); \
	DLEYNA_LOG_INFO("Never Quit: %s", (settings)->never_quit ? "T" : "F");\
	DLEYNA_LOG_DEBUG_NL(); \
	DLEYNA_LOG_INFO("Connector Name: %s", (settings)->connector_name);\
	DLEYNA_LOG_DEBUG_NL(); \
	\
	DLEYNA_LOG_INFO("[Logging settings]"); \
	DLEYNA_LOG_INFO("Log Type : %d", (settings)->log_type); \
	DLEYNA_LOG_INFO("Log Level: 0x%02X", (settings)->log_level); \
	DLEYNA_LOG_DEBUG_NL(); \
	\
	if ((settings)->netf_entries != NULL) \
		str = g_variant_print((settings)->netf_entries, FALSE); \
	DLEYNA_LOG_INFO("[Network filtering settings]"); \
	DLEYNA_LOG_INFO("Enabled : %s", (settings)->netf_enabled ? "T" : "F");\
	DLEYNA_LOG_INFO("Entries: %s", str); \
	g_free(str); \
	DLEYNA_LOG_DEBUG_NL(); \
} while (0)
#else
#define DLEYNA_SETTINGS_LOG_KEYS(sys, loc, settings)
#endif

static void prv_get_keyfile_path(const gchar *file, gchar **sys_path,
				 gchar **loc_path)
{
	const gchar *loc_dir;

	if (sys_path != NULL) {
		*sys_path = NULL;

		if (*SYS_CONFIG_DIR)
			*sys_path = g_strdup_printf("%s/%s", SYS_CONFIG_DIR,
						    file);
	}

	if (loc_path != NULL) {
		loc_dir =  g_get_user_config_dir();
		*loc_path = NULL;

		if (loc_dir && *loc_dir)
			*loc_path = g_strdup_printf("%s/%s", loc_dir,
						    file);
	}
}

static void prv_check_local_keyfile(const gchar *sys_path,
				    const gchar *loc_path)
{
	GFile *sys_file = NULL;
	GFile *loc_file;
	GFile *loc_dir;

	loc_file = g_file_new_for_path(loc_path);
	loc_dir = g_file_get_parent(loc_file);

	if (g_file_query_exists(loc_file, NULL) || (sys_path == NULL))
		goto exit;

	if (!g_file_query_exists(loc_dir, NULL)) {
		if (!g_file_make_directory(loc_dir, NULL, NULL))
			goto exit;
	}

	sys_file = g_file_new_for_path(sys_path);

	(void) g_file_copy(sys_file, loc_file, G_FILE_COPY_TARGET_DEFAULT_PERMS,
			   NULL, NULL, NULL, NULL);

exit:
	if (loc_dir)
		g_object_unref(loc_dir);

	if (loc_file)
		g_object_unref(loc_file);

	if (sys_file)
		g_object_unref(sys_file);
}

static GKeyFile *prv_load_keyfile(const gchar *filepath)
{
	GKeyFile *keyfile = NULL;

	if (filepath == NULL)
		goto exit;

	keyfile = g_key_file_new();

	if (!g_key_file_load_from_file(keyfile, filepath,
				       G_KEY_FILE_KEEP_COMMENTS,
				       NULL)) {
		g_key_file_free(keyfile);
		keyfile = NULL;
	}

exit:
	return keyfile;
}

static GVariant *prv_to_netf_entries(char **list)
{
	GVariantBuilder vb;
	GVariant *result = NULL;

	if ((list != NULL) && (*list != NULL)) {
		g_variant_builder_init(&vb, G_VARIANT_TYPE("as"));

		while (*list != NULL) {
			g_variant_builder_add(&vb, "s", *list);
			list++;
		}

		result = g_variant_ref_sink(g_variant_builder_end(&vb));
	}

	return result;
}

static int prv_to_log_level(gint *int_list, gsize length)
{
	gsize i;
	int log_level_value;
	int level;
	int log_level_array[] = {
		DLEYNA_LOG_LEVEL_DISABLED, DLEYNA_LOG_LEVEL_ERROR,
		DLEYNA_LOG_LEVEL_CRITICAL, DLEYNA_LOG_LEVEL_WARNING,
		DLEYNA_LOG_LEVEL_MESSAGE, DLEYNA_LOG_LEVEL_INFO,
		DLEYNA_LOG_LEVEL_DEBUG, DLEYNA_LOG_LEVEL_DEFAULT,
		DLEYNA_LOG_LEVEL_ALL };

	log_level_value = 0;

	/* Take all valid values, even duplicated ones, and skip all others.
	 * Priority is single value (0,7,8) over multi value (1..6)
	 * Priority for single values is first found */
	for (i = 0; i < length; ++i) {
		level = int_list[i];

		if (level > 0 && level < 7) {
			log_level_value |= log_level_array[level];
		} else if ((level == 0) || (level == 7) || (level == 8)) {
			log_level_value = log_level_array[level];
			break;
		}
	}

	return log_level_value;
}

static dleyna_log_type_t prv_to_log_type(int type)
{
	dleyna_log_type_t log_type = DLEYNA_LOG_TYPE_SYSLOG;

	switch (type) {
	case 0:
		break;
	case 1:
		log_type = DLEYNA_LOG_TYPE_GLIB;
		break;
	default:
		break;
	}

	return log_type;
}

static void prv_read_keys(dleyna_settings_t *settings)
{
	GError *error = NULL;
	GKeyFile *keyfile = settings->keyfile;
	gboolean b_val;
	gint int_val;
	gchar *s_val;
	gint *int_star;
	gsize length;
	gchar **list;

	b_val = g_key_file_get_boolean(keyfile,
				       DLEYNA_SETTINGS_GROUP_GENERAL,
				       DLEYNA_SETTINGS_KEY_NEVER_QUIT,
				       &error);

	if (error == NULL) {
		settings->never_quit = b_val;
	} else {
		g_error_free(error);
		error = NULL;
	}

	s_val = g_key_file_get_string(keyfile,
				      DLEYNA_SETTINGS_GROUP_GENERAL,
				      DLEYNA_SETTINGS_KEY_CONNECTOR_NAME,
				       &error);

	if (error == NULL) {
		g_free(settings->connector_name);
		settings->connector_name = s_val;
	} else {
		g_error_free(error);
		error = NULL;
	}

	int_val = g_key_file_get_integer(keyfile,
					 DLEYNA_SETTINGS_GROUP_LOG,
					 DLEYNA_SETTINGS_KEY_LOG_TYPE,
					 &error);

	if (error == NULL) {
		settings->log_type = prv_to_log_type(int_val);
	} else {
		g_error_free(error);
		error = NULL;
	}

	g_key_file_set_list_separator(keyfile, ',');

	int_star = g_key_file_get_integer_list(keyfile,
					       DLEYNA_SETTINGS_GROUP_LOG,
					       DLEYNA_SETTINGS_KEY_LOG_LEVEL,
					       &length,
					       &error);

	if (error == NULL) {
		settings->log_level = prv_to_log_level(int_star, length);
		g_free(int_star);
	} else {
		g_error_free(error);
		error = NULL;
	}

	b_val = g_key_file_get_boolean(keyfile,
				       DLEYNA_SETTINGS_GROUP_NETF,
				       DLEYNA_SETTINGS_KEY_NETF_ENABLED,
				       &error);

	if (error == NULL) {
		settings->netf_enabled = b_val;
	} else {
		g_error_free(error);
		error = NULL;
	}

	list = g_key_file_get_string_list(keyfile,
					  DLEYNA_SETTINGS_GROUP_NETF,
					  DLEYNA_SETTINGS_KEY_NETF_LIST,
					  NULL,
					  &error);

	if (error == NULL) {
		settings->netf_entries = prv_to_netf_entries(list);
		g_strfreev(list);
	} else {
		g_error_free(error);
		error = NULL;
	}
}

static void prv_init_default(dleyna_settings_t *settings)
{
	settings->never_quit = DLEYNA_SETTINGS_DEFAULT_NEVER_QUIT;
	settings->connector_name = g_strdup(
					DLEYNA_SETTINGS_DEFAULT_CONNECTOR_NAME);

	settings->log_type = DLEYNA_SETTINGS_DEFAULT_LOG_TYPE;
	settings->log_level = DLEYNA_SETTINGS_DEFAULT_LOG_LEVEL;

	settings->netf_enabled = FALSE;
	settings->netf_entries = NULL;
}

static void prv_keyfile_init(dleyna_settings_t *settings,
			     const gchar *sys_path,
			     const gchar *loc_path)
{
	const gchar *path = loc_path;

	settings->keyfile = prv_load_keyfile(loc_path);

	if (settings->keyfile == NULL) {
		path = sys_path;
		settings->keyfile = prv_load_keyfile(sys_path);
	}

	if (settings->keyfile != NULL) {
		prv_read_keys(settings);
		dleyna_log_update_type_level(settings->log_type,
					     settings->log_level);
		settings->file_path = g_strdup(path);
	}
}

static void prv_keyfile_finalize(dleyna_settings_t *settings)
{
	if (settings->netf_entries != NULL) {
		g_variant_unref(settings->netf_entries);
		settings->netf_entries = NULL;
	}

	if (settings->keyfile != NULL) {
		g_key_file_free(settings->keyfile);
		settings->keyfile = NULL;
	}

	g_free(settings->file_path);
	settings->file_path = NULL;
}

void dleyna_settings_new(const gchar *server, dleyna_settings_t **settings)
{
	gchar *sys_path = NULL;
	gchar *loc_path = NULL;
	const gchar *server_name;

	*settings = g_malloc0(sizeof(**settings));

	server_name = strrchr(server, '/');
	if (server_name)
		server_name++;
	else
		server_name = server;

	(*settings)->file_name = g_strdup_printf("%s%s", server_name, ".conf");

	prv_init_default(*settings);

	prv_get_keyfile_path((*settings)->file_name, &sys_path, &loc_path);

	if (loc_path)
		prv_check_local_keyfile(sys_path, loc_path);

	if (sys_path || loc_path)
		prv_keyfile_init(*settings, sys_path, loc_path);

	DLEYNA_SETTINGS_LOG_KEYS(sys_path, loc_path, *settings);

	g_free(sys_path);
	g_free(loc_path);
}

void dleyna_settings_delete(dleyna_settings_t *settings)
{
	g_free(settings->connector_name);
	g_free(settings->file_name);

	prv_keyfile_finalize(settings);

	g_free(settings);
}

const gchar *dleyna_settings_connector_name(dleyna_settings_t *settings)
{
	return settings->connector_name;
}

static void prv_save_settings_to_file(dleyna_settings_t *settings,
				      GError **error)
{
	gchar *data;
	gsize length;

	DLEYNA_LOG_DEBUG("Enter");

	data = g_key_file_to_data(settings->keyfile, &length , NULL);

	DLEYNA_LOG_DEBUG_NL();
	DLEYNA_LOG_DEBUG("\n%s", data);
	DLEYNA_LOG_DEBUG_NL();

	(void) g_file_set_contents(settings->file_path, data, length, error);

	g_free(data);
}

gboolean dleyna_settings_is_never_quit(dleyna_settings_t *settings)
{
	return settings->never_quit;
}

void dleyna_settings_set_never_quit(dleyna_settings_t *settings,
				    gboolean never_quit,
				    GError **error)
{
	DLEYNA_LOG_DEBUG("Enter");

	g_key_file_set_boolean(settings->keyfile,
			       DLEYNA_SETTINGS_GROUP_GENERAL,
			       DLEYNA_SETTINGS_KEY_NEVER_QUIT,
			       never_quit);

	prv_save_settings_to_file(settings, error);

	if (*error == NULL)
		settings->never_quit = never_quit;

	DLEYNA_LOG_DEBUG("Exit");
}

gboolean dleyna_settings_is_white_list_enabled(dleyna_settings_t *settings)
{
	return settings->netf_enabled;
}

void dleyna_settings_set_white_list_enabled(dleyna_settings_t *settings,
					    gboolean enabled,
					    GError **error)
{
	DLEYNA_LOG_DEBUG("Enter");

	g_key_file_set_boolean(settings->keyfile,
			       DLEYNA_SETTINGS_GROUP_NETF,
			       DLEYNA_SETTINGS_KEY_NETF_ENABLED,
			       enabled);

	prv_save_settings_to_file(settings, error);

	if (*error == NULL)
		settings->netf_enabled = enabled;

	DLEYNA_LOG_DEBUG("Exit");
}

GVariant *dleyna_settings_white_list_entries(dleyna_settings_t *settings)
{
	return settings->netf_entries;
}

static const gchar **prv_filter_null_str(const gchar **orig_strv,
					 gsize orig_len,
					 gsize *new_len)
{
	const gchar **strv = NULL;
	gsize i;
	gsize n = 0;

	if (orig_strv && orig_len) {
		strv = g_new(const gchar *, orig_len + 1);

		for (i = 0; i < orig_len; i++) {
			if (*orig_strv[i])
				strv[n++] = orig_strv[i];
		}

		strv[n] = NULL;
	}

	*new_len = n;

	return strv;
}

void dleyna_settings_set_white_list_entries(dleyna_settings_t *settings,
					    GVariant *entries,
					    GError **error)
{
	const gchar **list;
	gsize length;
	const gchar **list2;
	gsize length2;

	DLEYNA_LOG_DEBUG("Enter");

	list = g_variant_get_strv(entries, &length);

	list2 = prv_filter_null_str(list, length, &length2);

	g_key_file_set_string_list(settings->keyfile,
				   DLEYNA_SETTINGS_GROUP_NETF,
				   DLEYNA_SETTINGS_KEY_NETF_LIST,
				   list2, length2);

	prv_save_settings_to_file(settings, error);

	if (*error == NULL) {
		g_variant_unref(settings->netf_entries);
		settings->netf_entries = g_variant_ref(entries);
	}

	g_free(list);
	g_free(list2);

	DLEYNA_LOG_DEBUG("Exit");
}
