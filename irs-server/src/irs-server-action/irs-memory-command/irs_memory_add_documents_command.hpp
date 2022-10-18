#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryAddDocumentsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(IrsMemoryJsonPayload requestPayload) override
  {
  }
};
