#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryGetDocumentsLangsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(
      IrsMemory const & memory, IrsMemoryJsonPayload requestPayload, irs_bool & status) override
  {
    auto const documents = requestPayload.get<std::vector<std::string>>();
    std::vector<std::string> const langs = memory.GetLangs(documents);

    status = langs.size();
    return IrsMemoryJsonPayload(langs);
  }
};
