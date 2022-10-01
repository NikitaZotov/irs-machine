/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <ctype.h>

#include "irs_dictionary.h"
#include "irs_dictionary_private.h"

#include "../../irs-base/irs_allocator.h"
#include "../../irs-container/irs-string/irs_string.h"

#define SC_DICTIONARY_NODE_IS_VALID(__node) ((__node) != NULL_PTR)
#define SC_DICTIONARY_NODE_IS_NOT_VALID(__node) ((__node) == NULL_PTR)

irs_bool irs_dictionary_initialize(
    irs_dictionary ** dictionary, irs_uint8 size, void (*irs_char_to_irs_int)(irs_char, irs_uint8 *, irs_uint8 *))
{
  if (*dictionary != NULL_PTR)
    return FALSE;

  *dictionary = irs_mem_new(irs_dictionary, 1);
  (*dictionary)->size = size;
  (*dictionary)->irs_char_to_irs_int = irs_char_to_irs_int;
  (*dictionary)->root = _irs_dictionary_node_initialize(size);

  return TRUE;
}

irs_bool irs_dictionary_destroy(irs_dictionary * dictionary)
{
  if (dictionary == NULL_PTR)
    return FALSE;

  irs_dictionary_visit_up_nodes(dictionary, irs_dictionary_node_destroy, NULL_PTR);
  irs_dictionary_node_destroy(dictionary->root, NULL_PTR);
  irs_mem_free(dictionary);

  return TRUE;
}

inline irs_dictionary_node * _irs_dictionary_node_initialize(irs_uint8 size)
{
  irs_dictionary_node * node = irs_mem_new(irs_dictionary_node, 1);
  node->next = irs_mem_new(irs_dictionary_node *, size);

  node->data_list = NULL_PTR;
  node->offset = NULL_PTR;
  node->offset_size = 0;

  irs_dc_node_access_lvl_make_read(node);

  return node;
}

void irs_dictionary_node_destroy(irs_dictionary_node * node, void ** args)
{
  (void)args;

  irs_list_clear(node->data_list);
  irs_list_destroy(node->data_list);
  node->data_list = NULL_PTR;

  irs_mem_free(node->next);
  node->next = NULL_PTR;

  irs_mem_free(node->offset);
  node->offset = NULL_PTR;
  node->offset_size = 0;

  irs_mem_free(node);
}

inline irs_dictionary_node * _irs_dictionary_get_next_node(
    irs_dictionary * dictionary, irs_dictionary_node * node, irs_char ch)
{
  irs_uint8 num;
  dictionary->irs_char_to_irs_int(ch, &num, &node->mask);

  return node->next == NULL_PTR ? NULL_PTR : node->next[num];
}

irs_dictionary_node * irs_dictionary_append_to_node(irs_dictionary * dictionary, irs_char * irs_string, irs_uint32 size)
{
  irs_dictionary_node * node = dictionary->root;
  irs_char ** irs_string_ptr = irs_mem_new(irs_char *, 1);
  *irs_string_ptr = irs_string;

  irs_uint32 i = 0;
  irs_uint32 saved_offset_size = size;
  while (i < size)
  {
    irs_uint8 num;
    dictionary->irs_char_to_irs_int(**irs_string_ptr, &num, &node->mask);
    // define prefix
    if (SC_DICTIONARY_NODE_IS_NOT_VALID(node->next[num]))
    {
      node->next[num] = _irs_dictionary_node_initialize(dictionary->size);

      irs_dictionary_node * temp = node->next[num];

      temp->offset_size = size - i;
      irs_str_cpy(temp->offset, *irs_string_ptr, temp->offset_size);

      node = temp;

      break;
    }
    // visit next substring
    else if (node->next[num]->offset != NULL_PTR)
    {
      irs_dictionary_node * moving = node->next[num];

      irs_uint32 j = 0;
      for (; i < size && j < moving->offset_size && tolower(moving->offset[j]) == tolower(**irs_string_ptr);
           ++i, ++j, ++*irs_string_ptr)
        ;

      if (j != moving->offset_size)
      {
        saved_offset_size = moving->offset_size;

        node->next[num] = _irs_dictionary_node_initialize(dictionary->size);

        irs_dictionary_node * temp = node->next[num];

        temp->offset_size = j;
        temp->offset = moving->offset;
      }

      node = node->next[num];

      // insert intermediate node for prefix end branching
      if (j < moving->offset_size)
      {
        irs_char * offset_ptr = &*(moving->offset + j);

        dictionary->irs_char_to_irs_int(*offset_ptr, &num, &node->mask);
        node->next[num] = &*moving;

        irs_dictionary_node * temp = node->next[num];

        temp->offset_size = saved_offset_size - j;
        irs_str_cpy(temp->offset, offset_ptr, temp->offset_size);
      }
    }
    else
    {
      node = node->next[num];
      ++*irs_string_ptr;
      ++i;
    }
  }

  irs_mem_free(irs_string_ptr);
  return node;
}

irs_dictionary_node * irs_dictionary_append(
    irs_dictionary * dictionary, irs_char * irs_string, irs_uint32 size, void * value)
{
  irs_dictionary_node * node = irs_dictionary_append_to_node(dictionary, irs_string, size);

  if (node->data_list == NULL_PTR)
    irs_list_init(&node->data_list);

  void * copy = irs_mem_new(void, sizeof(value));
  *(void **)copy = *(void **)value;
  irs_list_push_back(node->data_list, copy);
  return node;
}

irs_dictionary_node * irs_dictionary_get_last_node_from_node(irs_dictionary * dictionary, const irs_char * irs_string)
{
  irs_dictionary_node * node = dictionary->root;

  if (irs_string == NULL_PTR)
    return NULL_PTR;

  // check prefixes matching
  irs_uint32 i = 0;
  irs_uint32 string_size = strlen(irs_string);
  while (i < string_size)
  {
    irs_dictionary_node * next = _irs_dictionary_get_next_node(dictionary, node, irs_string[i]);
    if (SC_DICTIONARY_NODE_IS_VALID(next))
    {
      node = next;
      i += node->offset_size;
    }
    else
      break;
  }

  // check suffixes matching
  if (i == string_size)
  {
    irs_char * str = irs_mem_new(irs_char, node->offset_size + 1);
    irs_mem_cpy(str, node->offset, node->offset_size);

    if (strcmp(str, irs_string + (string_size - node->offset_size)) == 0)
    {
      irs_mem_free(str);
      return node;
    }
    irs_mem_free(str);
  }

  return NULL_PTR;
}

irs_bool irs_dictionary_is_in(irs_dictionary * dictionary, const irs_char * irs_string)
{
  irs_dictionary_node * last = irs_dictionary_get_last_node_from_node(dictionary, irs_string);

  return SC_DICTIONARY_NODE_IS_VALID(last) && last->data_list != NULL_PTR;
}

irs_list * irs_dictionary_get(irs_dictionary * dictionary, const irs_char * irs_string)
{
  irs_dictionary_node * last = irs_dictionary_get_last_node_from_node(dictionary, irs_string);

  if (SC_DICTIONARY_NODE_IS_VALID(last))
    return last->data_list;

  return NULL_PTR;
}

void _irs_dictionary_visit_down_node_from_node(
    irs_dictionary * dictionary,
    irs_dictionary_node * node,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest)
{
  irs_uint8 i;
  for (i = 0; i < dictionary->size; ++i)
  {
    irs_dictionary_node * next = node->next[i];
    if (SC_DICTIONARY_NODE_IS_VALID(next))
    {
      callable(next, dest);

      irs_dictionary_visit_down_node_from_node(dictionary, next, callable, dest);
    }
  }
}

void irs_dictionary_visit_down_nodes(
    irs_dictionary * dictionary,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest)
{
  irs_dictionary_visit_down_node_from_node(dictionary, dictionary->root, callable, dest);
}

void _irs_dictionary_visit_up_node_from_node(
    irs_dictionary * dictionary,
    irs_dictionary_node * node,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest)
{
  irs_uint8 i;
  for (i = 0; i < dictionary->size; ++i)
  {
    irs_dictionary_node * next = node->next[i];
    if (SC_DICTIONARY_NODE_IS_VALID(next))
    {
      irs_dictionary_visit_up_node_from_node(dictionary, next, callable, dest);

      callable(next, dest);
    }
  }
}

void irs_dictionary_visit_up_nodes(
    irs_dictionary * dictionary,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest)
{
  irs_dictionary_visit_up_node_from_node(dictionary, dictionary->root, callable, dest);
}
