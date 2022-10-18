/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "irs_iterator.h"

#include "../../irs-base/irs_allocator.h"

irs_iterator * irs_iterator_init(irs_struct_node * begin, irs_struct_node * end)
{
  irs_iterator * it = irs_mem_new(irs_iterator, 1);
  it->begin = begin;
  it->end = end;
  it->current = NULL_PTR;

  return it;
}

irs_bool irs_iterator_next(irs_iterator * it)
{
  if (it == NULL_PTR || it->begin == NULL_PTR)
    return IRS_FALSE;

  if (it->current == NULL_PTR)
  {
    it->current = it->begin;
    return it->current != it->end;
  }

  it->current = it->current->next;
  return it->current != it->end;
}

irs_bool irs_iterator_prev(irs_iterator * it)
{
  if (it == NULL_PTR || it->end == NULL_PTR)
    return IRS_FALSE;

  if (it->current == NULL_PTR)
  {
    it->current = it->end->prev;
    return it->current != NULL_PTR;
  }

  it->current = it->current->prev;
  return it->current != NULL_PTR;
}

void irs_iterator_reset(irs_iterator * it)
{
  it->current = NULL_PTR;
}

void * irs_iterator_get(irs_iterator * it)
{
  if (it == NULL_PTR || it->current == NULL_PTR)
    return NULL_PTR;

  return it->current->data;
}

irs_bool irs_iterator_destroy(irs_iterator * it)
{
  if (it == NULL_PTR)
    return IRS_FALSE;

  irs_mem_free(it);
  return IRS_TRUE;
}
