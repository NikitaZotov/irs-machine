#pragma once

#include "irs_server.hpp"

using IrsServerMutex = std::mutex;
using IrsServerUniqueLock = std::unique_lock<IrsServerMutex>;
using IrsServerLock = std::lock_guard<IrsServerMutex>;
using IrsServerCondVar = std::condition_variable;

using IrsServerActions = std::queue<IrsServerAction *>;

class IrsServerImpl : public IrsServer
{
public:
  explicit IrsServerImpl(
      std::string const & host,
      IrsServerPort port,
      std::string const & logType,
      std::string const & logFile,
      std::string const & logLevel);

  void EmitActions() override;

  ~IrsServerImpl() override;

protected:
  IrsServerMutex m_actionLock;
  IrsServerMutex m_connectionLock;
  IrsServerCondVar m_actionCond;

  std::atomic<irs_bool> m_actionsRun;

  IrsServerActions * m_actions;

  IrsMemory * m_memory;

  void Initialize() override;

  void AfterInitialize() override;

  void OnMessage(IrsServerConnectionHandle const & hdl, IrsServerMessage const & msg) override;
};
