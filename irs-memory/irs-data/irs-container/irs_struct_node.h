/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_struct_node_h_
#define _irs_struct_node_h_

#include "../irs-base/irs_types.h"

typedef struct _irs_struct_node
{
  struct _irs_struct_node * next;
  struct _irs_struct_node * prev;
  void * data;
} irs_struct_node;

irs_struct_node * irs_struct_node_init(void * value);

#endif
