#include "irs_memory.h"

#include "irs-storage/irs_storage.h"

irs_bool irs_memory_initialize(irs_storage ** storage, irs_char const * path)
{
  return irs_storage_initialize(storage, path) == IRS_STORAGE_OK;
}

irs_bool irs_memory_shutdown(irs_storage * storage)
{
  return irs_storage_shutdown(storage) == IRS_STORAGE_OK;
}

irs_bool irs_memory_add_documents(irs_storage * storage, irs_list const * documents)
{
  return irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK;
}

irs_bool irs_memory_get_documents(irs_storage * storage, irs_list const * terms, irs_list ** documents)
{
  return irs_storage_get_documents(storage, terms, documents) == IRS_STORAGE_OK;
}
