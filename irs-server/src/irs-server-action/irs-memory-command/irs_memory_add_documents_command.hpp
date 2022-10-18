#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryAddDocumentsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(
      IrsMemory const & memory, IrsMemoryJsonPayload requestPayload, irs_bool & status) override
  {
    auto const documents = requestPayload.get<std::vector<std::string>>();
    memory.Add(documents);

    status = IRS_TRUE;

    return { {} };
  }
};
