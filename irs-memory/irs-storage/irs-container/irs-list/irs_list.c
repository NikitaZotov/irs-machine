/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "irs_list.h"

#include "../../irs-base/irs_allocator.h"

irs_bool irs_list_init(irs_list ** list)
{
  if (list == NULL_PTR)
    return FALSE;

  *list = irs_mem_new(irs_list, 1);
  (*list)->begin = NULL_PTR;
  (*list)->end = NULL_PTR;
  (*list)->size = 0;

  return TRUE;
}

irs_bool irs_list_destroy(irs_list * list)
{
  if (list == NULL_PTR)
    return FALSE;

  irs_struct_node * node = list->begin;
  irs_struct_node * temp;
  while (node != NULL_PTR)
  {
    temp = node;
    node = node->next;

    irs_mem_free(temp);
  }

  irs_mem_free(list);

  return TRUE;
}

irs_bool irs_list_clear(irs_list * list)
{
  if (list == NULL_PTR)
    return FALSE;

  irs_struct_node * node = list->begin;
  while (node != NULL_PTR)
  {
    irs_mem_free(node->data);
    node->data = NULL_PTR;
    node = node->next;
  }

  return TRUE;
}

irs_struct_node * irs_list_push(irs_list * list, irs_struct_node * node, void * data)
{
  if (list == NULL_PTR)
    return NULL_PTR;

  if (data == NULL_PTR)
    return NULL_PTR;

  if (node == NULL_PTR)
  {
    list->begin = irs_struct_node_init(data);
    list->size = 1;

    if (list->end == NULL_PTR)
      list->end = irs_mem_new(irs_struct_node, 1);

    list->begin->next = list->end;
    list->end->prev = list->begin;
    list->end->next = NULL_PTR;
    list->end->data = NULL_PTR;

    return list->begin;
  }

  irs_struct_node * temp = node->next;
  node->next = irs_struct_node_init(data);
  node->next->prev = node;
  node->next->next = temp;
  temp->prev = node->next;

  ++list->size;

  return node->next;
}

irs_struct_node * irs_list_push_back(irs_list * list, void * data)
{
  return irs_list_push(list, list->end ? list->end->prev : NULL_PTR, data);
}

irs_struct_node * irs_list_pop_back(irs_list * list)
{
  if (list == NULL_PTR)
    return NULL_PTR;

  if (list->size == 0)
    return NULL_PTR;

  irs_struct_node * temp = list->end->prev;

  if (list->end->prev->prev != NULL_PTR)
    list->end->prev->prev->next = list->end;

  list->end->prev = list->end->prev->prev;

  --list->size;

  return temp;
}

irs_bool irs_list_remove_if(irs_list * list, void * data, irs_bool (*predicate)(void * data, void * other))
{
  if (list == NULL_PTR)
    return FALSE;

  irs_bool is_removed = FALSE;
  irs_struct_node * node = list->begin;
  irs_struct_node * temp = NULL_PTR;
  while (node != NULL_PTR)
  {
    if (node->data != NULL_PTR && predicate(node->data, data))
    {
      if (node->prev != NULL_PTR)
        node->prev->next = node->next;

      if (node->next != NULL_PTR)
        node->next->prev = node->prev;

      temp = node;
      node = node->next;
      irs_mem_free(temp);
      is_removed = TRUE;

      --list->size;
    }
    else
      node = node->next;
  }

  if (list->size == 0)
    list->begin = NULL_PTR;

  return is_removed;
}

irs_struct_node * irs_list_front(irs_list * list)
{
  if (list == NULL_PTR)
    return NULL_PTR;

  return list->begin;
}

irs_struct_node * irs_list_back(irs_list * list)
{
  if (list == NULL_PTR)
    return NULL_PTR;

  if (list->size == 0)
    return NULL_PTR;

  return list->end->prev;
}

irs_iterator * irs_list_iterator(irs_list * list)
{
  if (list == NULL_PTR)
    return NULL_PTR;

  return irs_iterator_init(list->begin, list->end);
}
