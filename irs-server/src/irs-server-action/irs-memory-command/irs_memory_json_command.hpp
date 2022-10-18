#pragma once

#include "irs_memory.h"
#include "../irs_memory_json_payload.hpp"

class IrsMemoryJsonCommand
{
public:
  virtual IrsMemoryJsonPayload Complete(IrsMemoryJsonPayload requestPayload) = 0;

  virtual ~IrsMemoryJsonCommand() = default;
};
