#include "irs_storage.h"

#include "irs-base/irs_allocator.h"
#include "irs-container/irs-string/irs_string.h"

#include "irs-file-system/irs_file_system.h"

inline irs_uint8 _irs_dictionary_children_size()
{
  irs_uint8 const max_irs_char = 9;
  irs_uint8 const min_irs_char = 0;

  return max_irs_char - min_irs_char + 1;
}

void irs_char_to_irs_int(irs_char ch, irs_uint8 * ch_num, irs_uint8 * mask)
{
  *ch_num = (irs_uint8)ch - 48;
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
  irs_str_cpy((*storage)->path, path, irs_str_len(path));

  if (irs_dictionary_initialize(&(*storage)->dictionary, _irs_dictionary_children_size(), irs_char_to_irs_int) == FALSE)
    return IRS_STORAGE_NO;

  return IRS_STORAGE_OK;
}

irs_storage_status irs_storage_shutdown(irs_storage * storage)
{
  if (storage == NULL_PTR)
    return IRS_STORAGE_NO;

  if (irs_dictionary_destroy(storage->dictionary) == FALSE)
    return IRS_STORAGE_NO;

  irs_mem_free(storage->path);
  irs_mem_free(storage);

  return IRS_STORAGE_OK;
}

irs_storage_status irs_storage_add(irs_storage * storage, irs_list const * strings)
{

}

irs_storage_status irs_storage_get(irs_storage * storage, irs_list const * terms, irs_list ** strings)
{

}
