/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_pair_h_
#define _irs_pair_h_

typedef struct _irs_pair
{
  void * first;
  void * second;
} irs_pair;

irs_bool irs_pair_initialize(irs_pair ** pair, void * first, void * second)
{
  *pair = irs_mem_new(irs_pair, 1);
  (*pair)->first = first;
  (*pair)->second = second;

  return TRUE;
}

#endif
