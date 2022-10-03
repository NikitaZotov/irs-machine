/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_string_h_
#define _irs_string_h_

#include <stdio.h>

#include "../../irs-base/irs_allocator.h"
#include "../../irs-base/irs_types.h"

#define irs_str_cpy(copy, string, size) \
  ({ \
    copy = irs_mem_new(irs_char, size + 1); \
    irs_mem_cpy(copy, string, size); \
  })

#define irs_str_len(string) strlen(string)

#define irs_str_printf(out, size, format, ...) g_snprintf(out, size, "%s/%s", __VA_ARGS__)

#define irs_int_to_str_int(number, string) \
  irs_uint64 length = snprintf(NULL_PTR, 0, "%lu", number); \
  string = irs_mem_new(irs_char, length + 1); \
  snprintf(string, length, "%lu", number)

#endif
