/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_string_h_
#define _irs_string_h_

#include "../../irs-base/irs_allocator.h"
#include "../../irs-base/irs_types.h"

#define irs_str_cpy(copy, string, size) \
  ({ \
    copy = irs_mem_new(irs_char, size + 1); \
    irs_mem_cpy(copy, string, size); \
  })

#define irs_str_len(string) strlen(string)

#endif
