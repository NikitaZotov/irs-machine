/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_list_h_
#define _irs_list_h_

#include "../irs_struct_node.h"
#include "../irs-iterator/irs_iterator.h"

//! A irs-list container to store data in linear order
typedef struct _irs_list
{
  irs_struct_node * begin;  // root element in irs-list
  irs_struct_node * end;    // last element in irs-list
  irs_uint32 size;          // irs-list elements size
} irs_list;

/*! Initializes irs-list.
 * @param[out] list Pointer to a irs-list pointer to initialize
 * @returns Returns IRS_TRUE, if irs-list didn't exist; otherwise return IRS_FALSE.
 */
irs_bool irs_list_init(irs_list ** list);

/*! Destroys a irs-list.
 * @param list A irs-dictionary pointer to destroy
 * @returns Returns IRS_TRUE, if a irs-dictionary exists; otherwise return IRS_FALSE.
 */
irs_bool irs_list_destroy(irs_list * list);

/*! Free a irs-list values.
 * @param list A irs-dictionary pointer to clear
 * @returns Returns IRS_TRUE, if a irs-dictionary exists; otherwise return IRS_FALSE.
 */
irs_bool irs_list_clear(irs_list * list);

/*! Pushes data in irs-list node after specified, adding new node.
 * @param list A irs-list pointer
 * @param node A node after that new node inserts to store data
 * @param data A pushable data
 * @returns Returns New added node.
 */
irs_struct_node * irs_list_push(irs_list * list, irs_struct_node * node, void * data);

/*! Pushes data in irs-list node at the end of irs-list, adding new node.
 * @param list A irs-list pointer
 * @param data A pushable data
 * @returns Returns New added node.
 */
irs_struct_node * irs_list_push_back(irs_list * list, void * data);

irs_struct_node * irs_list_push_front(irs_list * list, irs_struct_node * node, void * data);

/*! Pops last node from irs-list.
 * @param list A irs-list pointer
 * @returns Returns Popped node.
 */
irs_struct_node * irs_list_pop_back(irs_list * list);

/*! Removes node from irs-list if predicate for values is IRS_TRUE.
 * @param list A irs-list pointer
 * @returns Returns Popped node.
 */
irs_bool irs_list_remove_if(irs_list * list, void * data, irs_bool (*predicate)(void * data, void * other));

/*! Gets irs-list forward-backward iterator.
 * @param list A irs-list pointer
 * @return Returns Iterator.
 */
irs_iterator * irs_list_iterator(irs_list * list);

irs_iterator * irs_list_reverse_iterator(irs_list * list);

#endif
