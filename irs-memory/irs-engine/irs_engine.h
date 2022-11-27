#ifndef _irs_engine_
#define _irs_engine_

#include "../irs-storage/irs-base/irs_types.h"
#include "../irs-storage/irs-container/irs-list/irs_list.h"
#include "../irs-storage/irs-container/irs-dictionary/irs_dictionary.h"

typedef struct _irs_engine
{
  irs_char * path;
} irs_engine;

typedef enum _irs_engine_status
{
  IRS_ENGINE_OK,

  IRS_ENGINE_NO,
  IRS_ENGINE_WRONG_PATH,
  IRS_ENGINE_WRITE_ERROR,
  IRS_ENGINE_READ_ERROR
} irs_engine_status;


irs_engine_status irs_engine_initialize(irs_engine ** engine, irs_char const * path);
