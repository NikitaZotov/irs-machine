#ifndef _irs_config_h_
#define _irs_config_h_

#include <glib.h>

#include "irs-base/irs_types.h"

// Config, that contains all configuration options: ['<group>/<key>'] = <value>
typedef GHashTable irs_config;
typedef GList irs_list;

#define irs_config_get_keys(config) g_hash_table_get_keys(config)
#define irs_list_get_first(list) g_list_first(list)
#define irs_list_free(list) g_list_free(list)

/*! Initialize irs-memory configuration from specified configuration file
 * @param config Config table
 * @param file_path Path to configuration file
 */
irs_bool irs_config_initialize(irs_config ** config, irs_char const * file_path);

/*! Shutdown irs-memory configuration. Free allocated data.
 */
void irs_config_shutdown(irs_config * config);

// --- api for extensions ---
/*!
 * Returns string value of \p key in specified \p group
 * @param config Config table
 * @param group Name of configuration group
 * @param key Name of key to return value
 * @returns Returns the value associated with the key as string, or null if the key was not found of could not be
 * parsed.
 * @attention Returned value managed by irs_config and shouldn't be freed
 */

irs_char * irs_config_get_value_string(irs_config * config, irs_char const * group, irs_char const * key);

/*!
 * Returns the value associated with the key as an integer, or 0 if the key was not found or could not be parsed.
 * @see irs_config_get_value_string
 */
irs_uint32 irs_config_get_value_int(irs_config * config, irs_char const * group, irs_char const * key);

/*!
 * Returns the value associated with the key as a boolean, or irs_FALSE if the key was not found or could not be parsed.
 * @see irs_config_get_value_string
 */
irs_bool irs_config_get_value_boolean(irs_config * config, irs_char const * group, irs_char const * key);

/*!
 * Returns the value associated with the key as a float, or 0.0 if the key was not found or could not be parsed.
 * @see irs_config_get_value_string
 */
irs_float irs_config_get_value_float(irs_config * config, irs_char const * group, irs_char const * key);

#endif
