#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryGetDocumentsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(IrsMemoryJsonPayload requestPayload) override
  {
  }
};
