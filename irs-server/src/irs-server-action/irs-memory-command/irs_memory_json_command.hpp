#pragma once

#include "../irs_memory_json_payload.hpp"

#include "../irs-memory/irs_memory.hpp"

class IrsMemoryJsonCommand
{
public:
  virtual IrsMemoryJsonPayload Complete(
      IrsMemory const & memory, IrsMemoryJsonPayload requestPayload, irs_bool & status) = 0;

  virtual ~IrsMemoryJsonCommand() = default;
};
