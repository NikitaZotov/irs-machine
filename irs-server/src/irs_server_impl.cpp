#include "irs_server_impl.hpp"

#include "irs_server_action_defines.hpp"

IrsServerImpl::IrsServerImpl(
    std::string const & host,
    IrsServerPort port,
    std::string const & logType,
    std::string const & logFile,
    std::string const & logLevel,
    IrsMemoryData const & data)
  : IrsServer(host, port, logType, logFile, logLevel)
{
  m_memory = IrsMemory::GetInstance(data.dbPath.c_str(), data.dataPath.c_str());

  LogMessage(IrsServerLogMessages::app, "Database path: " + data.dbPath);
  LogMessage(IrsServerLogMessages::app, "Data path: " + data.dbPath);
}

void IrsServerImpl::Initialize()
{
  m_instance->set_message_handler(bind(&IrsServerImpl::OnMessage, this, ::_1, ::_2));
}

void IrsServerImpl::AfterInitialize()
{
  delete m_memory;
  m_memory = nullptr;
}

void IrsServerImpl::EmitActions()
{
}

void IrsServerImpl::OnMessage(IrsServerConnectionHandle const & hdl, IrsServerMessage const & msg)
{
  LogMessage(IrsServerLogMessages::message_payload, msg->get_payload());
  {
    auto action = IrsServerMessageAction(hdl, msg);

    std::string const answer = action.Emit(*m_memory);
    Send(action.m_hdl, answer, IrsServerMessageType::text);
  }
}

IrsServerImpl::~IrsServerImpl()
{
}
