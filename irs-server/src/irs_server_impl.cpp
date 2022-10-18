#include "irs_server_impl.hpp"

#include "irs_server_action_defines.hpp"

IrsServerImpl::IrsServerImpl(
    std::string const & host,
    IrsServerPort port,
    std::string const & logType,
    std::string const & logFile,
    std::string const & logLevel)
  : IrsServer(host, port, logType, logFile, logLevel),
    m_actionsRun(IRS_TRUE),
    m_actions(new IrsServerActions()),
    m_memory(nullptr)
{
}

void IrsServerImpl::Initialize()
{
  m_memory = IrsMemory::GetInstance("db/");

  m_instance->set_message_handler(bind(&IrsServerImpl::OnMessage, this, ::_1, ::_2));
}

void IrsServerImpl::AfterInitialize()
{
  while (m_actions->empty() == IRS_FALSE)
    ;

  m_actionsRun = IRS_FALSE;
  m_actionCond.notify_one();

  delete m_memory;
  m_memory = nullptr;
}

void IrsServerImpl::EmitActions()
{
  while (m_actionsRun == IRS_TRUE)
  {
    IrsServerUniqueLock lock(m_actionLock);

    while (m_actions->empty() && m_actionsRun)
      m_actionCond.wait(lock);

    if (m_actionsRun == IRS_TRUE)
      break;

    IrsServerAction * action = m_actions->front();
    m_actions->pop();

    lock.unlock();

    IrsServerLock guard(m_connectionLock);

    try
    {
      std::string answer = action->Emit(*m_memory);
    }
    catch (std::exception const & e)
    {
      LogError(IrsServerLogErrors::devel, e.what());
    }
    delete action;
  }
}

void IrsServerImpl::OnMessage(IrsServerConnectionHandle const & hdl, IrsServerMessage const & msg)
{
  {
    IrsServerLock guard(m_actionLock);
    m_actions->push(new IrsServerMessageAction(hdl, msg));
  }
  m_actionCond.notify_one();
}

IrsServerImpl::~IrsServerImpl()
{
  delete m_actions;
}
