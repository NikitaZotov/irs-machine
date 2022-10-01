/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_dictionary_h_
#define _irs_dictionary_h_

#include "../../irs-base/irs_types.h"
#include "../irs-list/irs_list.h"

#define SC_DC_NODE_ACCESS_LVL_RMASK 0b00000001
#define SC_DC_NODE_ACCESS_LVL_WMASK 0b00000010

#define irs_dc_node_access_lvl_make_read(node) (node->mask |= SC_DC_NODE_ACCESS_LVL_RMASK)
#define irs_dc_node_access_lvl_make_no_read(node) (node->mask &= ~SC_DC_NODE_ACCESS_LVL_RMASK)
#define irs_dc_node_access_lvl_check_read(node) \
  ((node->mask & SC_DC_NODE_ACCESS_LVL_RMASK) == SC_DC_NODE_ACCESS_LVL_RMASK)

#define irs_dc_node_access_lvl_make_write(node) (node->mask |= SC_DC_NODE_ACCESS_LVL_WMASK)
#define irs_dc_node_access_lvl_make_no_write(node) (node->mask &= ~SC_DC_NODE_ACCESS_LVL_WMASK)
#define irs_dc_node_access_lvl_check_write(node) \
  ((node->mask & SC_DC_NODE_ACCESS_LVL_WMASK) == SC_DC_NODE_ACCESS_LVL_WMASK)

//! A irs-dictionary structure node to store prefixes
typedef struct _irs_dictionary_node
{
  struct _irs_dictionary_node ** next;  // a pointer to irs-dictionary node children pointers
  irs_char * offset;                    // a pointer to substring of node string
  irs_uint32 offset_size;               // size to substring of node string
  irs_list * data_list;                 // data list
  irs_uint8 mask;                       // mask for rights checking and memory optimization
} irs_dictionary_node;

//! A irs-dictionary structure node to store pairs of <string, object> type
typedef struct _irs_dictionary
{
  irs_dictionary_node * root;  // irs-dictionary tree root node
  irs_uint8 size;              // default irs-dictionary node children size
  void (*irs_char_to_irs_int)(irs_char, irs_uint8 *, irs_uint8 *);
} irs_dictionary;

/*! Initializes irs-dictionary
 * @param dictionary Pointer to a irs-dictionary pointer to initialize
 * @returns[out] Returns TRUE, if irs-dictionary doesn't exist; otherwise return FALSE.
 */
irs_bool irs_dictionary_initialize(
    irs_dictionary ** dictionary, irs_uint8 size, void (*irs_char_to_irs_int)(irs_char, irs_uint8 *, irs_uint8 *));

/*! Destroys a irs-dictionary
 * @param dictionary A irs-dictionary pointer to destroy
 * @returns Returns TRUE, if a irs-dictionary exists; otherwise return FALSE.
 */
irs_bool irs_dictionary_destroy(irs_dictionary * dictionary);

/*! Appends a string to a irs-dictionary by a common prefix with another string started in irs-dictionary node, if such
 * exists. In end irs-dictionary node stores pointer to data by string.
 * @param node A irs-dictionary node where common prefix may be started
 * @param irs_string An appendable string
 * @param size An appendable string size
 * @param data A pointer to data storing by appended string
 * @returns Returns A irs-dictionary node where appended string ends
 */
irs_dictionary_node * irs_dictionary_append(
    irs_dictionary * dictionary, irs_char * irs_string, irs_uint32 size, void * data);

/*! Checks, if a string is in a irs-dictionary by a common prefix with another string started in irs-dictionary
 * node, if such exists.
 * @param irs_dictionary A irs-dictionary node where common prefix may be started
 * @param irs_string A verifiable string
 * @returns Returns TRUE, if string starts in irs-dictionary node; otherwise return FALSE.
 */
irs_bool irs_dictionary_is_in(irs_dictionary * dictionary, const irs_char * irs_string);

/*! Gets datas from a terminal irs-dictionary node where string ends.
 * @param dictionary A irs-dictionary
 * @param irs_string A string to retrieve datas by it
 * @returns Returns Datas from a irs-dictionary node where string ends
 */
irs_list * irs_dictionary_get(irs_dictionary * dictionary, const irs_char * irs_string);

/*! Visits all irs-dictionary nodes and calls procedure with it and its data. A method completes down iterating visiting.
 * @param node A irs-dictionary
 * @param callable A callable object (procedure)
 * @param[out] dest A pointer to procedure result pointer
 */
void irs_dictionary_visit_down_nodes(
    irs_dictionary * dictionary,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest);

/*! Visits all irs-dictionary nodes and calls procedure with it and its data. A method completes up iterating visiting.
 * @param node A irs-dictionary
 * @param callable A callable object (procedure)
 * @param[out] dest A pointer to procedure result pointer
 */
void irs_dictionary_visit_up_nodes(
    irs_dictionary * dictionary,
    void (*callable)(irs_dictionary_node *, void **),
    void ** dest);

#endif
