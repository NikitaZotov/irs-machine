/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_file_system_h_
#define _irs_file_system_h_

#include "../irs-base/irs_types.h"

#include "irs-io/irs_io.h"

irs_bool irs_fs_isdir(irs_char const * path);

irs_bool irs_fs_isfile(irs_char const * path);

irs_bool irs_fs_rmdir(irs_char const * path);

irs_bool irs_fs_rmfile(irs_char const * path);

irs_bool irs_fs_mkdirs(irs_char const * path);

#endif
