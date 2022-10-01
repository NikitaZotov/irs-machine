#ifndef _irs_storage_
#define _irs_storage_

#include "irs-base/irs_types.h"
#include "irs-container/irs-list/irs_list.h"

typedef struct _irs_storage
{
  irs_char * path;
} irs_storage;

irs_bool irs_storage_initialize(irs_storage ** storage, irs_char const * path);

irs_bool irs_storage_shutdown(irs_storage * storage);

irs_bool irs_storage_add(irs_storage * storage, irs_list const * strings);

irs_list * irs_storage_get(irs_storage * storage, irs_list const * terms);

#endif //_irs_storage_
