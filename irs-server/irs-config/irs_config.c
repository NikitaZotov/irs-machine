#include "irs_config.h"

#include "irs-base/irs_allocator.h"

void value_table_destroy_key_value(void * data)
{
  irs_mem_free(data);
}

irs_char * value_table_create_hash_key(irs_char const * group, irs_char const * key)
{
  return g_strdup_printf("%s/%s", group, key);
}

irs_bool irs_config_initialize(irs_config ** config, irs_char const * file_path)
{
  GKeyFile * key_file = g_key_file_new();

  if (file_path == NULL_PTR || g_key_file_load_from_file(key_file, file_path, G_KEY_FILE_NONE, NULL_PTR) == IRS_FALSE)
  {
    g_key_file_free(key_file);
    return IRS_FALSE;
  }

  // load all values into hash table
  *config =
      g_hash_table_new_full(g_str_hash, g_str_equal, value_table_destroy_key_value, value_table_destroy_key_value);

  irs_uint64 groups_len = 0;
  irs_char ** groups = g_key_file_get_groups(key_file, &groups_len);
  for (irs_uint64 i = 0; i < groups_len; ++i)
  {
    // get all keys in group
    irs_uint64 keys_len = 0;
    irs_char ** keys = g_key_file_get_keys(key_file, groups[i], &keys_len, 0);
    for (irs_uint64 j = 0; j < keys_len; ++j)
    {
      // append keys into hash table
      irs_char * hash_key = value_table_create_hash_key(groups[i], keys[j]);
      g_hash_table_insert(*config, hash_key, g_key_file_get_string(key_file, groups[i], keys[j], 0));
    }
    g_strfreev(keys);
  }
  g_strfreev(groups);

  g_key_file_free(key_file);

  return IRS_TRUE;
}

void irs_config_shutdown(irs_config * config)
{
  // empty memory allocated for values table
  if (config != NULL_PTR)
  {
    g_hash_table_remove_all(config);
    g_hash_table_destroy(config);
  }

  config = NULL_PTR;
}

irs_char * irs_config_get_value_string(irs_config * config, irs_char const * group, irs_char const * key)
{
  irs_char * hash_key = value_table_create_hash_key(group, key);
  const void * res = g_hash_table_lookup(config, hash_key);
  g_free(hash_key);
  return (irs_char *)res;
}

irs_uint32 irs_config_get_value_int(irs_config * config, irs_char const * group, irs_char const * key)
{
  irs_char const * str_value = irs_config_get_value_string(config, group, key);
  if (str_value == NULL_PTR)
    return 0;

  return atoi(str_value);
}

float irs_config_get_value_float(irs_config * config, irs_char const * group, irs_char const * key)
{
  return (float)irs_config_get_value_int(config, group, key);
}

irs_bool irs_config_get_value_boolean(irs_config * config, irs_char const * group, irs_char const * key)
{
  irs_char const * str_value = irs_config_get_value_string(config, group, key);
  if (str_value == NULL_PTR)
    return IRS_FALSE;

  if (g_str_equal(str_value, "true") || g_str_equal(str_value, "True") || g_str_equal(str_value, "1"))
    return IRS_TRUE;

  return IRS_FALSE;
}
