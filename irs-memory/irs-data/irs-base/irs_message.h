/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_message_h_
#define _irs_message_h_

#define GLIB

#ifdef GLIB
#  include <glib.h>
#endif

#define irs_error(...) g_error(__VA_ARGS__)

#define irs_critical(...) g_critical(__VA_ARGS__)

#define irs_warning(...) g_warning(__VA_ARGS__)

#define irs_message(...) g_message(__VA_ARGS__)

#undef GLIB

#endif //_irs_message_h_
