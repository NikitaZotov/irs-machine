/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_dictionary_private_h_
#define _irs_dictionary_private_h_

#include "../../irs-base/irs_types.h"
#include "../irs-list/irs_list.h"

irs_dictionary_node * _irs_dictionary_node_initialize(irs_uint8 size);

irs_dictionary_node * _irs_dictionary_get_next_node(
    irs_dictionary * dictionary, irs_dictionary_node * node, irs_char ch);

/*! Destroys a irs-dictionary node
 * @param node A irs-dictionary node pointer to destroy
 * @param args An additional args used with node destroying
 */
void irs_dictionary_node_destroy(irs_dictionary_node * node, void ** args);

/*! Appends a string to a irs-dictionary by a common prefix with another string started in irs-dictionary node, if such
 * exists.
 * @param dictionary A irs-dictionary where common prefix may be started
 * @param irs_string An appendable string
 * @param size An appendable string size
 * @returns Returns A irs-dictionary node where appended string is ended
 */
irs_dictionary_node * irs_dictionary_append_to_node(
    irs_dictionary * dictionary, irs_char * irs_string, irs_uint32 size);

/*! Gets a terminal irs-dictionary node where string ends.
 * @param node A irs-dictionary node where common prefix may be started
 * @param irs_string A string to retrieve data by it
 * @returns Returns A irs-dictionary node where string ends
 */
irs_dictionary_node * irs_dictionary_get_last_node_from_node(irs_dictionary * dictionary, const irs_char * irs_string);

/*! Visits all irs-dictionary nodes starting with specified node and calls procedure with it and its data. A method
 * completes down iterating visiting.
 * @param node A irs-dictionary
 * @param node A irs-dictionary node to start visiting
 * @param callable A callable object (procedure)
 * @param[out] dest A pointer to procedure result pointer
 */
void _irs_dictionary_visit_down_node_from_node(
    irs_dictionary * dictionary,
    irs_dictionary_node * node,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest);

/*! Visits all irs-dictionary nodes starting with specified node and calls procedure with it and its data. A method
 * completes up iterating visiting.
 * @param node A irs-dictionary
 * @param node A irs-dictionary node to start visiting
 * @param callable A callable object (procedure)
 * @param[out] dest A pointer to procedure result pointer
 */
void _irs_dictionary_visit_up_node_from_node(
    irs_dictionary * dictionary,
    irs_dictionary_node * node,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest);

#endif
