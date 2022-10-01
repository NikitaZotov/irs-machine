#include "irs_file_system.h"

#include "glib.h"
#include "glib/gstdio.h"

irs_bool irs_fs_isdir(irs_char const * path)
{
  return g_file_test(path, G_FILE_TEST_IS_DIR);
}

irs_bool irs_fs_isfile(irs_char const * path)
{
  return g_file_test(path, G_FILE_TEST_IS_DIR);
}

irs_bool irs_fs_rmdir(irs_char const * path)
{
  if (irs_fs_isdir(path) == FALSE)
    return FALSE;

  GDir * directory = g_dir_open(path, 0, 0);

  // calculate files
  irs_char tmp_path[MAX_PATH_LENGTH];
  irs_char const * dir = g_dir_read_name(directory);
  while (dir != NULL_PTR)
  {
    g_snprintf(tmp_path, MAX_PATH_LENGTH, "%s/%s", path, dir);

    if (irs_fs_isfile(path) == TRUE)
    {
      if (g_remove(tmp_path) == -1)
        return FALSE;
    }
    else if (irs_fs_isdir(tmp_path) == TRUE)
      irs_fs_rmdir(tmp_path);

    dir = g_dir_read_name(directory);
  }

  g_dir_close(directory);
  g_rmdir(path);

  return TRUE;
}

irs_bool irs_fs_rmfile(irs_char const * path)
{
  return g_remove(path) != -1;
}

irs_bool irs_fs_mkdirs(irs_char const * path)
{
  irs_int32 const mode = 0755;
  if (g_mkdir_with_parents(path, mode) == -1)
    return FALSE;

  return TRUE;
}
