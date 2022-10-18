#pragma once

#include <utility>

#include "irs_server_defines.hpp"
#include "irs_server_log.hpp"

#include "irs-server-action/irs_server_action.hpp"
#include "irs-storage/irs-base/irs_types.h"

class IrsServer
{
public:
  explicit IrsServer(
      std::string hostName,
      size_t port,
      std::string const & logType,
      std::string const & logFile,
      std::string const & logLevel)
    : m_hostName(std::move(hostName))
    , m_port(port)
  {
    m_instance = new IrsServerCore();
    {
      LogMessage(IrsServerLogMessages::app, "IRS-machine socket data");
      LogMessage(IrsServerLogMessages::app, "\tHost name: " + m_hostName);
      LogMessage(IrsServerLogMessages::app, "\tPort: " + std::to_string(m_port));
    }
    {
      LogMessage(IrsServerLogMessages::app, "IRS-machine log");
      LogMessage(IrsServerLogMessages::app, "\tLog type: " + logType);
      LogMessage(
          IrsServerLogMessages::app,
          "\tLog file: " + ((logType != IRS_SERVER_FILE_TYPE || logFile.empty()) ? "No" : logFile));
      LogMessage(IrsServerLogMessages::app, "\tLog level: " + logLevel);
    }
    LogMessage(IrsServerLogMessages::app, "IRS-machine launched");
    m_log = new IrsServerLog(m_instance, logType, logFile, logLevel);
  }

  void Run()
  {
    m_instance->init_asio();
    m_instance->set_reuse_addr(IRS_TRUE);

    Initialize();

    m_instance->listen({boost::asio::ip::address::from_string(m_hostName), irs_uint16(m_port)});
    m_instance->start_accept();

    LogMessage(IrsServerLogMessages::app, "Start actions processing");
    m_actionsThread = std::thread(&IrsServer::EmitActions, &*this);

    LogMessage(IrsServerLogMessages::app, "Start input-output processing");
    m_ioThread = std::thread(&IrsServerCore::run, &*m_instance);
  }

  void Stop()
  {
    m_instance->stop_listening();

    AfterInitialize();

    if (m_actionsThread.joinable())
    {
      LogMessage(IrsServerLogMessages::app, "Close actions processing");
      m_actionsThread.join();
    }

    if (m_ioThread.joinable())
    {
      LogMessage(IrsServerLogMessages::app, "Close input-output processing");

      m_instance->stop();
      m_ioThread.join();
    }
  }

  void Shutdown()
  {
    ClearLogOptions();

    LogMessage(IrsServerLogMessages::app, "Shutdown irs-machine");

    delete m_instance;
    m_instance = nullptr;

    delete m_log;
    m_log = nullptr;
  }

  virtual void EmitActions() = 0;

  void Send(IrsServerConnectionHandle const & hdl, std::string const & message, IrsServerMessageType type)
  {
    m_instance->send(hdl, message, type);
  }

  void LogMessage(IrsServerLogChannel channel, std::string const & message)
  {
    m_instance->get_alog().write(channel, message);
  }

  void LogError(IrsServerLogChannel channel, std::string const & errorMessage)
  {
    m_instance->get_elog().write(channel, errorMessage);
  }

  void ClearLogOptions()
  {
    delete m_log;
    m_log = new IrsServerLog(m_instance, IRS_SERVER_CONSOLE_TYPE, "", IRS_SERVER_INFO_LEVEL);
  }

  void SetMessageChannels(IrsServerLogChannel channels)
  {
    m_instance->set_access_channels(channels);
  }

  void SetErrorChannels(IrsServerLogChannel channels)
  {
    m_instance->set_access_channels(channels);
  }

  virtual ~IrsServer()
  {
    Shutdown();
  }

protected:
  std::string m_hostName;
  IrsServerPort m_port;

  IrsServerCore * m_instance;

  IrsServerLog * m_log;

  virtual void Initialize() = 0;

  virtual void AfterInitialize() = 0;

  virtual void OnMessage(IrsServerConnectionHandle const & hdl, IrsServerMessage const & msg) = 0;

private:
  std::thread m_ioThread;
  std::thread m_actionsThread;
};
