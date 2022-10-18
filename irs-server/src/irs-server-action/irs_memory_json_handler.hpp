#pragma once

#include <string>

#include "irs-storage/irs-base/irs_types.h"

#include "../irs_server_defines.hpp"
#include "irs_memory_json_payload.hpp"
#include "irs_server_command_defines.hpp"

class IrsMemoryJsonHandler
{
public:
  virtual ~IrsMemoryJsonHandler() = default;

  virtual std::string Handle(IrsServerConnectionHandle const & hdl, std::string const & requestMessage);

protected:
  std::vector<IrsMemoryJsonPayload> ParseRequestMessage(std::string const & requestMessage);

  IrsMemoryJsonPayload ResponseRequestMessage(
      IrsServerConnectionHandle const & hdl,
      size_t requestId,
      std::string const & requestType,
      IrsMemoryJsonPayload const & requestPayload);

  IrsMemoryJsonPayload HandleRequestPayload(
      IrsServerConnectionHandle const & hdl,
      std::string const & requestType,
      IrsMemoryJsonPayload const & requestPayload,
      irs_bool & status);

  static IrsMemoryJsonPayload FormResponseMessage(
      size_t requestId,
      irs_bool status,
      IrsMemoryJsonPayload const & responsePayload);


  std::map<std::string, IrsMemoryJsonCommand *> m_commands = {
      { "add_documents", new IrsMemoryAddDocumentsCommand() },
      { "get_documents", new IrsMemoryGetDocumentsCommand() },
  };
};
