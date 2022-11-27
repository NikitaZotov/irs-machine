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
      std::string const & logLevel,
      IrsMemoryData const & memoryData);

  void EmitActions() override;

  ~IrsServerImpl() override;

protected:
  IrsMemory * m_memory;

  void Initialize() override;

  void AfterInitialize() override;

  void OnMessage(IrsServerConnectionHandle const & hdl, IrsServerMessage const & msg) override;
};
