#include "irs_storage.h"

#include "irs-base/irs_allocator.h"
#include "irs-container/irs-string/irs_string.h"

#include "irs-file-system/irs_file_system.h"

inline irs_uint8 _irs_dictionary_children_size()
{
  irs_uint8 const max_irs_char = 255;
  irs_uint8 const min_irs_char = 1;

  return max_irs_char - min_irs_char + 1;
}

void _irs_char_to_irs_int(irs_char ch, irs_uint8 * ch_num, irs_uint8 * mask)
{
  *ch_num = 128 + (irs_uint8)ch;
}

void _irs_init_db_path(irs_char const * path, irs_char const * postfix, irs_char ** out_path)
{
  irs_uint32 size = irs_str_len(path) + irs_str_len(postfix);
  *out_path = irs_mem_new(irs_char, size + 1);
  irs_str_printf(*out_path, size, "%s/%s", path, postfix);
}

irs_storage_status irs_storage_initialize(irs_storage ** storage, irs_char const * path)
{
  if (path == NULL_PTR)
    return IRS_STORAGE_WRONG_PATH;

  if (irs_fs_isdir(path) == FALSE)
  {
    if (irs_fs_mkdirs(path) == FALSE)
      return IRS_STORAGE_WRONG_PATH;
  }

  *storage = irs_mem_new(irs_storage, 1);
  {
    irs_str_cpy((*storage)->path, path, irs_str_len(path));

    static irs_char const *terms_postfix = "terms.irsdb";
    _irs_init_db_path(path, terms_postfix, &(*storage)->terms_path);
    (*storage)->terms_size = 0;

    static irs_char const *strings_postfix = "strings.irsdb";
    _irs_init_db_path(path, strings_postfix, &(*storage)->strings_path);
    (*storage)->strings_size = 0;

    if (irs_dictionary_initialize(
        &(*storage)->terms_significancy_dictionary, _irs_dictionary_children_size(), _irs_char_to_irs_int) == FALSE)
      return IRS_STORAGE_NO;
  }

  return IRS_STORAGE_OK;
}

irs_storage_status irs_storage_shutdown(irs_storage * storage)
{
  if (storage == NULL_PTR)
    return IRS_STORAGE_NO;
  {
    if (irs_dictionary_destroy(storage->terms_significancy_dictionary) == FALSE)
      return IRS_STORAGE_NO;

    irs_mem_free(storage->path);
    irs_mem_free(storage->terms_path);
    irs_mem_free(storage->strings_path);
  }
  irs_mem_free(storage);

  return IRS_STORAGE_OK;
}

irs_list * _irs_storage_pick_out_terms_in_string(irs_storage * storage, irs_list const * strings) {

  irs_iterator * it = irs_list_iterator((irs_list *)strings);


  while (irs_iterator_next(it))
  {
    irs_char * string = irs_iterator_get(it);
    irs_uint32 size = irs_str_len(string);

    for (irs_uint32 i = 0; i < size; ++i)
    {

    }
  }
}

irs_storage_status irs_storage_add(irs_storage * storage, irs_list const * strings)
{
  irs_iterator * it = irs_list_iterator((irs_list *)strings);

  irs_io_channel * channel = irs_io_new_channel(storage->strings_path, "r", NULL_PTR);
  irs_io_channel_seek(channel, storage->strings_size, IRS_IO_SEEK_SET, NULL_PTR);

  while (irs_iterator_next(it))
  {
    irs_char * string = irs_iterator_get(it);
    irs_uint32 size = irs_str_len(string);
    irs_uint64 written_bytes = 0;

    if (irs_io_channel_write_chars(channel, string, size, &written_bytes, NULL_PTR) != IRS_IO_STATUS_NORMAL
      || size != written_bytes)
      return IRS_STORAGE_WRITE_ERROR;

    irs_char * key;
    irs_int_to_str_int(storage->strings_size, key);
    irs_uint32 key_size = irs_str_len(key);

    irs_dictionary_append(storage->terms_significancy_dictionary, key, key_size, NULL_PTR);

    storage->strings_size += size;
  }

  irs_iterator_destroy(it);

  return IRS_STORAGE_OK;
}

irs_storage_status irs_storage_get(irs_storage * storage, irs_list const * terms, irs_list ** strings)
{

}
