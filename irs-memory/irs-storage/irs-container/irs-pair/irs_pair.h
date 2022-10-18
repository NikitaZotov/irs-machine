#ifndef _irs_pair_h_
#define _irs_pair_h_

#include "../../irs-base/irs_allocator.h"

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

  return IRS_TRUE;
}

#endif
