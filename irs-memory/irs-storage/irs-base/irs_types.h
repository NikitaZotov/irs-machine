/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_types_h_
#define _irs_types_h_

#include <glib.h>

#define NULL_PTR 0
#define IRS_TRUE 1
#define IRS_FALSE 0

#define MAX_PATH_LENGTH 1024

typedef unsigned short irs_bool;

typedef char irs_int8;
typedef unsigned char irs_uint8;
typedef short irs_int16;
typedef unsigned short irs_uint16;
typedef int irs_int32;
typedef unsigned int irs_uint32;
typedef long irs_int64;
typedef unsigned long irs_uint64;

typedef char irs_char;
typedef unsigned char irs_uchar;

typedef float irs_float;

#endif //_irs_types_h_
