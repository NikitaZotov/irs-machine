#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryGetDocumentsLangsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(
      IrsMemory const & memory, IrsMemoryJsonPayload requestPayload, irs_bool & status) override
  {
    auto const documents = requestPayload["documents"].get<std::vector<std::string>>();
    irs_uint8 const methodType = requestPayload["method_type"].get<irs_uint8>();

    std::vector<std::string> const langs = memory.GetLangs(methodType, documents);

    status = langs.size();
    return IrsMemoryJsonPayload(langs);
  }
};
