#ifndef _irs_storage_
#define _irs_storage_

#include "irs-base/irs_types.h"
#include "irs-container/irs-list/irs_list.h"
#include "irs-container/irs-dictionary/irs_dictionary.h"

typedef struct _irs_storage
{
  irs_dictionary * term_offset_dictionary;
  irs_char * path;

  irs_char * terms_path;
  irs_uint64 terms_size;

  irs_char * documents_path;
  irs_uint64 documents_size;
} irs_storage;

typedef enum _irs_storage_status
{
  IRS_STORAGE_OK,

  IRS_STORAGE_NO,
  IRS_STORAGE_WRONG_PATH,
  IRS_STORAGE_WRITE_ERROR
} irs_storage_status;

irs_storage_status irs_storage_initialize(irs_storage ** storage, irs_char const * path);

irs_storage_status irs_storage_shutdown(irs_storage * storage);

irs_storage_status irs_storage_add_documents(irs_storage * storage, irs_list const * documents);

irs_storage_status irs_storage_get_documents(irs_storage * storage, irs_list const * terms, irs_list ** documents);

#endif //_irs_storage_
