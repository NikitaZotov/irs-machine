#include "irs_memory_json_handler.hpp"

std::string IrsMemoryJsonHandler::Handle(
    IrsMemory const & memory, IrsServerConnectionHandle const & hdl, std::string const & requestMessage)
{
  std::vector<IrsMemoryJsonPayload> requestData = ParseRequestMessage(requestMessage);
  if (requestData.empty())
    return "Invalid request message";

  std::string const & requestType = requestData.at(0).get<std::string>();
  IrsMemoryJsonPayload const & requestPayload = requestData.at(1);
  size_t const & requestId = requestData.at(2).get<size_t>();

  return ResponseRequestMessage(memory, hdl, requestId, requestType, requestPayload).dump();
}

std::vector<IrsMemoryJsonPayload> IrsMemoryJsonHandler::ParseRequestMessage(std::string const & requestMessage)
{
  std::vector<IrsMemoryJsonPayload> requestData;

  IrsMemoryJsonPayload const & messageJson = IrsMemoryJsonPayload::parse(requestMessage);
  if (messageJson.is_null())
    return requestData;

  requestData = { messageJson["type"], messageJson["payload"], messageJson["id"] };

  for (auto const & item : requestData)
  {
    if (item.is_null())
      return {};
  }

  return requestData;
}

IrsMemoryJsonPayload IrsMemoryJsonHandler::ResponseRequestMessage(
    IrsMemory const & memory,
    IrsServerConnectionHandle const & hdl,
    size_t const requestId,
    std::string const & requestType,
    IrsMemoryJsonPayload const & requestPayload)
{
  irs_bool status = IRS_FALSE;

  IrsMemoryJsonPayload responsePayload;
  try
  {
    responsePayload = HandleRequestPayload(memory, hdl, requestType, requestPayload, status);
  }
  catch (...)
  {
    status = IRS_FALSE;
  }

  return FormResponseMessage(requestId, status, responsePayload);
}

IrsMemoryJsonPayload IrsMemoryJsonHandler::HandleRequestPayload(
    IrsMemory const & memory,
    IrsServerConnectionHandle const & hdl,
    std::string const & requestType,
    IrsMemoryJsonPayload const & requestPayload,
    irs_bool & status)
{
  status = IRS_FALSE;

  IrsMemoryJsonPayload responsePayload;
  auto const & it = m_commands.find(requestType);
  if (it == m_commands.end())
  {
    status = IRS_FALSE;
  }

  auto * command = it->second;
  responsePayload = command->Complete(memory, requestPayload, status);

  return responsePayload;
}

IrsMemoryJsonPayload IrsMemoryJsonHandler::FormResponseMessage(
    size_t requestId,
    irs_bool status,
    IrsMemoryJsonPayload const & responsePayload)
{
  return IrsMemoryJsonPayload({ {"id", requestId}, {"status", status}, {"payload", responsePayload} });
}
