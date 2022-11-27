/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "irs_struct_node.h"

#include "../irs-base/irs_allocator.h"

inline irs_struct_node * irs_struct_node_init(void * data)
{
  irs_struct_node * node = irs_mem_new(irs_struct_node, 1);
  node->next = NULL_PTR;
  node->prev = NULL_PTR;

  node->data = data;

  return node;
}
