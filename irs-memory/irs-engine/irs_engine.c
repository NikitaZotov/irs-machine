#include "irs_engine.h"

#include "../irs-storage/irs-file-system/irs_file_system.h"
#include "../irs-storage/irs-base/irs_allocator.h"
#include "../irs-storage/irs-container/irs-string/irs_string.h"

irs_engine_status irs_engine_initialize(irs_engine ** engine, irs_char const * path)
{
  if (path == NULL_PTR)
    return IRS_ENGINE_WRONG_PATH;

  if (irs_fs_isdir(path) == IRS_FALSE)
  {
    if (irs_fs_mkdirs(path) == IRS_FALSE)
      return IRS_ENGINE_WRONG_PATH;
  }

  *engine = irs_mem_new(irs_engine, 1);
  {
    irs_str_cpy((*engine)->path, path, irs_str_len(path));
  }

  return IRS_ENGINE_OK;
}

irs_engine_status irs_engine_shutdown(irs_engine * engine)
{
  if (engine == NULL_PTR)
    return IRS_ENGINE_NO;

  irs_mem_free(engine->path);
  irs_mem_free(engine);

  return IRS_ENGINE_OK;
}
