#include "irs_engine.h"

#include "irs-file-system/irs_file_system.h"
#include "irs-base/irs_allocator.h"
#include "irs-container/irs-string/irs_string.h"
#include "irs-container/irs-pair/irs_pair.h"

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
  irs_char * copy;
  irs_str_cpy(copy, word, length - 1);
  irs_pair_initialize(&dictionary_lang, dictionary, copy);

  while (status != G_IO_STATUS_EOF)
  {
    irs_str_cpy(copy, word, length - 1);
    irs_dictionary_append(dictionary, copy, length - 1, NULL_PTR);
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

void _irs_engine_clear_lang_dictionaries(irs_engine * engine)
{
  irs_iterator * it = irs_list_iterator(engine->lang_key_words);
  while (irs_iterator_next(it))
  {
    irs_pair * pair = irs_iterator_get(it);
    irs_dictionary_destroy(pair->first);
    irs_mem_free(pair->second);
    irs_mem_free(pair);
  }
  irs_iterator_destroy(it);
  irs_list_destroy(engine->lang_key_words);
  engine->lang_key_words = NULL_PTR;
}

irs_engine_status irs_engine_shutdown(irs_engine * engine)
{
  if (engine == NULL_PTR)
    return IRS_ENGINE_NO;

  irs_mem_free(engine->path);
  _irs_engine_clear_lang_dictionaries(engine);
  irs_mem_free(engine);

  return IRS_ENGINE_OK;
}

irs_list * _irs_storage_get_document_short_terms(irs_char * document)
{
  static const irs_char delim[] = " ,.-\0()[]_";
  irs_uint64 size = irs_str_len(document);
  irs_char string[size + 1];
  irs_mem_cpy(string, document, size);
  string[size] = '\0';

  irs_char * term = strtok(string, delim);
  irs_list * terms;
  irs_list_init(&terms);
  while (term != NULL_PTR)
  {
    irs_uint64 const term_length = irs_str_len(term);

    static const irs_uint64 MAX_LENGTH = 7;
    if (term_length <= MAX_LENGTH)
    {
      irs_char * term_copy;
      irs_str_cpy(term_copy, term, term_length);
      irs_list_push_back(terms, term_copy);
    }
    term = strtok(NULL_PTR, delim);
  }

  return terms;
}

irs_list * irs_engine_define_documents_languages(irs_engine * engine, irs_list * documents)
{
  irs_list * langs;
  irs_list_init(&langs);

  irs_iterator * it = irs_list_iterator(documents);
  while (irs_iterator_next(it))
  {
    irs_char * document = irs_iterator_get(it);

    irs_list * document_lang_common_coincidences;
    irs_list_init(&document_lang_common_coincidences);

    irs_list * terms = _irs_storage_get_document_short_terms(document);

    irs_iterator * lang_dict_it = irs_list_iterator(engine->lang_key_words);
    while (irs_iterator_next(lang_dict_it))
    {
      irs_uint32 common_coincidences = 0;

      irs_iterator * term_it = irs_list_iterator(terms);
      while (irs_iterator_next(term_it))
      {
        irs_char * term = irs_iterator_get(term_it);

        {
          irs_pair * dictionary_lang = irs_iterator_get(lang_dict_it);
          irs_dictionary * dictionary = dictionary_lang->first;
          if (irs_dictionary_is_in(dictionary, term))
            ++common_coincidences;
        }
      }
      irs_iterator_destroy(term_it);

      irs_list_push_back(document_lang_common_coincidences, (void *)common_coincidences);
    }
    irs_iterator_destroy(lang_dict_it);

    irs_list_clear(terms);
    irs_list_destroy(terms);

    irs_uint32 position = 0;
    irs_uint32 max_common_coincidences = 0;
    irs_uint32 max_common_coincidences_pos = 0;
    irs_iterator * coincidence_it = irs_list_iterator(document_lang_common_coincidences);
    while (irs_iterator_next(coincidence_it))
    {
      irs_uint32 common_coincidences = (irs_uint32)irs_iterator_get(coincidence_it);
      if (max_common_coincidences < common_coincidences)
      {
        max_common_coincidences = common_coincidences;
        max_common_coincidences_pos = position;
      }

      ++position;
    }
    irs_iterator_destroy(coincidence_it);
    irs_list_destroy(document_lang_common_coincidences);

    position = 0;
    lang_dict_it = irs_list_iterator(engine->lang_key_words);
    while (irs_iterator_next(lang_dict_it))
    {
      if (position == max_common_coincidences_pos)
      {
        irs_pair * dictionary_lang = irs_iterator_get(lang_dict_it);
        irs_char * defined_lang = dictionary_lang->second;
        irs_list_push_back(langs, defined_lang);
      }

      ++position;
    }
    irs_iterator_destroy(lang_dict_it);
  }

  return langs;
}
