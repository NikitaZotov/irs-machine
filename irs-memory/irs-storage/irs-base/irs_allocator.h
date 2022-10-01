/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_allocator_h_
#define _irs_allocator_h_

#define GLIB

#ifdef GLIB
#  include <glib.h>
#  include <memory.h>
#endif

#define irs_mem_new(struct_type, n_structs) g_new0(struct_type, n_structs)

#define irs_mem_set(pointer, constant, n_structs) memset(pointer, constant, n_structs)

#define irs_mem_cpy(source, dest, n_structs) memcpy(source, dest, n_structs)

#define irs_mem_free(pointer) g_free(pointer)

#undef GLIB

#endif //_irs_allocator_h_
