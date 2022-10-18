#ifndef _irs_memory_
#define _irs_memory_

#include "irs-storage/irs-base/irs_types.h"

typedef struct _irs_list irs_list;
typedef struct _irs_storage irs_storage;

irs_bool irs_memory_initialize(irs_storage ** storage, irs_char const * path);

irs_bool irs_memory_shutdown(irs_storage * storage);

irs_bool irs_memory_add_documents(irs_storage * storage, irs_list const * documents);

irs_bool irs_memory_get_documents(irs_storage * storage, irs_list const * terms, irs_list ** documents);

#endif //_irs_memory_
