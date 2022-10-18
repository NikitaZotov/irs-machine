/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "irs_server_impl.hpp"

#include "irs_server_action_defines.hpp"

IrsServerImpl::IrsServerImpl(
    std::string const & host,
    IrsServerPort port,
    std::string const & logType,
    std::string const & logFile,
    std::string const & logLevel)
  : IrsServer(host, port, logType, logFile, logLevel),
    m_actionsRun(TRUE),
    m_actions(new IrsServerActions())
{
}

void IrsServerImpl::Initialize()
{
  m_instance->set_message_handler(bind(&IrsServerImpl::OnMessage, this, ::_1, ::_2));
}

void IrsServerImpl::AfterInitialize()
{
  while (m_actions->empty() == FALSE)
    ;

  m_actionsRun = FALSE;
  m_actionCond.notify_one();
}

void IrsServerImpl::EmitActions()
{
  while (m_actionsRun == TRUE)
  {
    IrsServerUniqueLock lock(m_actionLock);

    while (m_actions->empty() && m_actionsRun)
      m_actionCond.wait(lock);

    if (m_actionsRun == TRUE)
      break;

    IrsServerAction * action = m_actions->front();
    m_actions->pop();

    lock.unlock();

    IrsServerLock guard(m_connectionLock);

    try
    {
      std::string answer = action->Emit();
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
