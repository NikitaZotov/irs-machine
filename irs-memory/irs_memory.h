#ifndef _irs_memory_
#define _irs_memory_

#include "irs-base/irs_types.h"

typedef struct _irs_list irs_list;
typedef struct _irs_storage irs_storage;
typedef struct _irs_engine irs_engine;

typedef struct _irs_memory
{
  irs_storage * storage;
  irs_engine * engine;
} irs_memory;

irs_bool irs_memory_initialize(
  irs_memory ** memory, irs_char const * path, irs_char const * lang_key_words_path, irs_char const * lang_alpha_path);

irs_bool irs_memory_shutdown(irs_memory * memory);

irs_bool irs_memory_add_documents(irs_memory * memory, irs_list const * documents);

irs_bool irs_memory_get_documents(irs_memory * memory, irs_list const * terms, irs_list ** documents);

irs_bool irs_memory_load(irs_memory * memory);

irs_bool irs_memory_save(irs_memory * memory);

irs_bool irs_memory_get_documents_langs(
  irs_memory * memory, irs_uint8 type, irs_list const * documents, irs_list ** langs);

irs_bool irs_memory_get_documents_summarizations(irs_memory * memory, irs_list const * documents, irs_list ** summarizations);

#endif //_irs_memory_
