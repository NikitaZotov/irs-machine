#include "irs_engine.h"

#include "../irs-storage/irs-file-system/irs_file_system.h"
#include "../irs-storage/irs-base/irs_allocator.h"
#include "../irs-storage/irs-container/irs-string/irs_string.h"
#include "../irs-storage/irs-container/irs-pair/irs_pair.h"

irs_uint8 _irs_dictionary_children_size()
{
  irs_uint8 const max_irs_char = 255;
  irs_uint8 const min_irs_char = 1;

  return max_irs_char - min_irs_char + 1;
}

void _irs_char_to_irs_int(irs_char ch, irs_uint8 * ch_num, irs_uint8 * mask)
{
  *ch_num = 128 + (irs_uint8)ch;
}

irs_pair * _irs_engine_read_lang_file(irs_dictionary * dictionary, irs_char const * path)
{
  irs_io_channel * channel = irs_io_new_channel(path, "r", NULL_PTR);
  irs_io_channel_set_encoding(channel, NULL_PTR, NULL_PTR);

  irs_char * word;
  irs_uint64 length;

  irs_pair * dictionary_lang;
  GIOStatus status = g_io_channel_read_line(channel, &word, &length, NULL_PTR, NULL_PTR);
  irs_pair_initialize(&dictionary_lang, dictionary, word);

  while (status != G_IO_STATUS_EOF)
  {
    irs_dictionary_append(dictionary, word, length, NULL_PTR);
    irs_mem_free(word);

    status = g_io_channel_read_line(channel, &word, &length, NULL_PTR, NULL_PTR);
  }
  irs_io_channel_shutdown(channel, IRS_TRUE, NULL_PTR);

  return dictionary_lang;
}

irs_char * _irs_init_lang_file_path(irs_char const * path, irs_char const * postfix)
{
  irs_uint32 size = irs_str_len(path) + irs_str_len(postfix) + 2;
  irs_char * out_path = irs_mem_new(irs_char, size + 1);
  irs_str_printf(out_path, size, "%s/%s", path, postfix);
  return out_path;
}

void _irs_engine_read_lang_dictionaries(irs_engine * engine, irs_char const * path)
{
  if (engine->lang_key_words == NULL_PTR)
  {
    irs_list_init(&engine->lang_key_words);
  }

  GDir * dir;
  GError * error;
  irs_char const * filename;

  dir = g_dir_open(path, 0, &error);
  while ((filename = g_dir_read_name(dir)))
  {
    irs_dictionary * dict;
    irs_dictionary_initialize(&dict, _irs_dictionary_children_size(), _irs_char_to_irs_int);

    irs_char * file_path = _irs_init_lang_file_path(path, filename);
    irs_pair * pair = _irs_engine_read_lang_file(dict, file_path);
    irs_list_push_back(engine->lang_key_words, pair);
  }
}

irs_engine_status irs_engine_initialize(irs_engine ** engine, irs_char const * path)
{
  if (path == NULL_PTR)
    return IRS_ENGINE_WRONG_PATH;

  if (irs_fs_isdir(path) == IRS_FALSE)
  {
    if (irs_fs_mkdirs(path) == IRS_FALSE)
      return IRS_ENGINE_WRONG_PATH;
  }

  *engine = irs_mem_new(irs_engine, 1);
  {
    irs_str_cpy((*engine)->path, path, irs_str_len(path));
    _irs_engine_read_lang_dictionaries(*engine, path);
  }

  return IRS_ENGINE_OK;
}

irs_engine_status irs_engine_shutdown(irs_engine * engine)
{
  if (engine == NULL_PTR)
    return IRS_ENGINE_NO;

  irs_mem_free(engine->path);
  irs_mem_free(engine);

  return IRS_ENGINE_OK;
}
