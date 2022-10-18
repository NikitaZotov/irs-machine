#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryGetDocumentsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(
      IrsMemory const & memory, IrsMemoryJsonPayload requestPayload, irs_bool & status) override
  {
    auto const terms = requestPayload.get<std::vector<std::string>>();
    std::unordered_map<std::string, std::unordered_map<std::string, irs_float>> documents = memory.Get(terms);

    status = IRS_TRUE;

    return { documents };
  }
};
