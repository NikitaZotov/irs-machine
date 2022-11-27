#include "irs_memory.h"

#include "irs-storage/irs-base/irs_allocator.h"
#include "irs-storage/irs_storage.h"
#include "irs-engine/irs_engine.h"

irs_bool irs_memory_initialize(irs_memory ** memory, irs_char const * path, irs_char const * data_path)
{
  *memory = irs_mem_new(irs_memory, 1);
  return (irs_storage_initialize(&(*memory)->storage, path) == IRS_STORAGE_OK
    && irs_engine_initialize(&(*memory)->engine, data_path));
}

irs_bool irs_memory_shutdown(irs_memory * memory)
{
  irs_bool status = (irs_storage_shutdown(memory->storage) == IRS_STORAGE_OK
    && irs_engine_shutdown(memory->engine));
  irs_mem_free(memory);
  return status;
}

irs_bool irs_memory_add_documents(irs_memory * memory, irs_list const * documents)
{
  return irs_storage_add_documents(memory->storage, documents) == IRS_STORAGE_OK;
}

irs_bool irs_memory_get_documents(irs_memory * memory, irs_list const * terms, irs_list ** documents)
{
  return irs_storage_get_documents(memory->storage, terms, documents) == IRS_STORAGE_OK;
}

irs_bool irs_memory_load(irs_memory * memory)
{
  return irs_storage_load(memory->storage) == IRS_STORAGE_OK;
}

irs_bool irs_memory_save(irs_memory * memory)
{
  return irs_storage_save(memory->storage) == IRS_STORAGE_OK;
}

irs_bool irs_memory_get_documents_langs(irs_memory * memory, irs_list const * documents, irs_list ** langs)
{
  *langs = irs_engine_define_documents_languages(memory->engine, documents);
  return (*langs)->size;
}
