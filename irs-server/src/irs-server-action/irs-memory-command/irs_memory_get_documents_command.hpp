#pragma once

#include "irs_memory_json_command.hpp"

class IrsMemoryGetDocumentsCommand : public IrsMemoryJsonCommand
{
public:
  IrsMemoryJsonPayload Complete(
      IrsMemory const & memory, IrsMemoryJsonPayload requestPayload, irs_bool & status) override
  {
    auto const terms = requestPayload.get<std::vector<std::string>>();
    std::vector<std::unordered_map<irs_uint64, std::pair<std::string, irs_float>>> const termsDocuments
      = memory.Get(terms);

    std::unordered_map<std::string, std::string> documents;
    irs_uint64 count = 0;
    IrsMemoryJsonPayload termAttributes;
    for (auto const & termDocuments : termsDocuments)
    {
      std::unordered_map<std::string, irs_float> attributes;
      for (auto const & document : termDocuments)
      {
        std::string const offset = std::to_string(document.first);
        std::string const & text = document.second.first;
        irs_float const significancy = document.second.second;

        attributes.insert({ offset, significancy });
        documents.insert({ offset, text });
      }

      termAttributes.push_back({ { "term", terms.at(count) }, { "documents", attributes } });
      ++count;
    }
    IrsMemoryJsonPayload payload;
    payload["attributes"] = termAttributes;
    payload["documents"] = documents;

    status = IRS_TRUE;

    return payload;
  }
};
