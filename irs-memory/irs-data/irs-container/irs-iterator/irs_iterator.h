/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_container_iterator_h_
#define _irs_container_iterator_h_

#include "../irs_struct_node.h"
#include "../../irs-base/irs_types.h"

//! A irs-container iterator to visit elements in irs-containers
typedef struct _irs_iterator
{
  irs_struct_node * begin;    // a start iterating element
  irs_struct_node * end;      // a finite iterating element
  irs_struct_node * current;  // a current iterating element
} irs_iterator;

/*! Initializes irs-container-iterator.
 * @param begin A start element in irs-container
 * @param end A finite element in irs-container
 * @returns Returns A irs-container-iterator pointer
 */
irs_iterator * irs_iterator_init(irs_struct_node * begin, irs_struct_node * end);

/*! Checks next element existence in a irs-container.
 * @param it A irs-container-iterator pointer
 * @returns Returns IRS_TRUE, if next element exists in irs-container
 */
irs_bool irs_iterator_next(irs_iterator * it);

/*! Checks previous element existence in a irs-container.
 * @param it A irs-container-iterator pointer
 * @returns Returns IRS_TRUE, if a previous element exists in irs-container
 */
irs_bool irs_iterator_prev(irs_iterator * it);

/*! Resets a irs-container-iterator to begin state
 * @param it A irs-container-iterator pointer
 */
void irs_iterator_reset(irs_iterator * it);

/*! Gets data from current element
 * @param it A irs-container-iterator pointer
 */
void * irs_iterator_get(irs_iterator * it);

/*! Destroys a irs-container-iterator
 * @param it A irs-container-iterator pointer
 * @returns Returns IRS_TRUE, if a irs-container-iterator pointer exists
 */
irs_bool irs_iterator_destroy(irs_iterator * it);

#endif
