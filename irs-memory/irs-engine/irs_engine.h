#ifndef _irs_engine_
#define _irs_engine_

#include "irs-base/irs_types.h"
#include "irs-container/irs-list/irs_list.h"
#include "irs-container/irs-dictionary/irs_dictionary.h"

typedef struct _irs_engine
{
  irs_char * lang_key_words_path;
  irs_list * lang_key_words;

  irs_char * lang_alpha_path;
  irs_list * lang_alphabets;

} irs_engine;

typedef enum _irs_engine_status
{
  IRS_ENGINE_OK,

  IRS_ENGINE_NO,
  IRS_ENGINE_WRONG_PATH,
  IRS_ENGINE_WRITE_ERROR,
  IRS_ENGINE_READ_ERROR
} irs_engine_status;


irs_engine_status irs_engine_initialize(
  irs_engine ** engine,
  irs_char const * path,
  irs_char const * lang_alphabets_path);

irs_engine_status irs_engine_shutdown(irs_engine * engine);

irs_list * irs_engine_define_documents_languages(irs_engine * engine, irs_list * documents);

irs_list * irs_engine_define_documents_languages_by_alphabets(irs_engine * engine, irs_list * documents);

irs_list * irs_engine_get_documents_summarizations(irs_engine * engine, irs_list * documents);

#endif //_irs_engine_
