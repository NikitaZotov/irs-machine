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

#define irs_str_printf(out, size, format, ...) g_snprintf(out, size, format, __VA_ARGS__)

#define irs_int_to_str_int(number, string) \
  irs_uint64 length = (number == 0) ? 1 : snprintf(NULL_PTR, 0, "%lu", number); \
  string = irs_mem_new(irs_char, length + 1); \
  gcvt(number, length, string)

#define irs_float_to_str_float(number, string) \
  irs_uint64 digit_size = snprintf(NULL_PTR, 0, "%lu", (irs_uint64)number); \
  static irs_uint64 mnemonic_size = 4; \
  string = irs_mem_new(irs_char, digit_size + mnemonic_size + 1); \
  gcvt(number, digit_size, string)

#define irs_str_find(str, substring) strstr(str, substring) != NULL_PTR

#define irs_str_find_position(str, substring) \
  (irs_uint64)(str - strstr(str, substring) + 1)

#define irs_str_find_get(str, substring) \
  strstr(str, substring)

#define irs_str_cmp(str, other) strcmp(str, other) == 0

#endif
