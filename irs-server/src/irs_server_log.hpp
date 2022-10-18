#pragma once

#include <ostream>
#include <string>
#include <unordered_set>

#include "irs_server_defines.hpp"
#include "irs-storage/irs-base/irs_types.h"

class IrsServerLog
{
public:
  IrsServerLog(
      IrsServerCore * server, std::string const & logType, std::string const & logFile, std::string const & logLevel)
    : m_server(server)
  {
    Initialize(logType, logFile, logLevel);
  }

  void Initialize(std::string logType, std::string logFile, std::string logLevel)
  {
    logType = DefineType(logType);
    logFile = DefineFile(logType, logFile);
    logLevel = DefineLevel(logLevel);

    if (logFile.empty() == FALSE)
    {
      m_instance = new std::ofstream();
      m_instance->open(logFile);

      m_server->get_alog().set_ostream(m_instance);
      m_server->get_elog().set_ostream(m_instance);
    }
    else
    {
      m_instance = nullptr;
      m_server->get_alog().set_ostream();
      m_server->get_elog().set_ostream();
    }

    m_server->clear_access_channels(IrsServerLogMessages::all);
    m_server->clear_error_channels(IrsServerLogErrors::all);

    if (logLevel == IRS_SERVER_DEBUG_LEVEL)
      m_server->set_access_channels(IrsServerLogMessages::all);
    else if (logLevel == IRS_SERVER_INFO_LEVEL)
      m_server->set_access_channels(IrsServerLogMessages::app);
    else if (logLevel == IRS_SERVER_ERROR_LEVEL)
      m_server->set_access_channels(IrsServerLogMessages::fail);
    m_server->set_error_channels(IrsServerLogErrors::all);
  }

  std::string DefineType(std::string const & logType)
  {
    std::string defined;

    auto const & it = m_types.find(logType);
    if (it != m_types.end())
      return logType;

    return IRS_SERVER_CONSOLE_TYPE;
  }

  std::string DefineFile(std::string const & logType, std::string const & logFile)
  {
    std::string defined;

    if (logType == IRS_SERVER_FILE_TYPE)
      return logFile;

    static std::string empty;
    return empty;
  }

  std::string DefineLevel(std::string const & logLevel)
  {
    std::string defined;

    auto const & it = m_levels.find(logLevel);
    if (it != m_types.end())
      return logLevel;

    return IRS_SERVER_INFO_LEVEL;
  }

  ~IrsServerLog()
  {
    delete m_instance;
    m_instance = nullptr;
  }

private:
  std::ofstream * m_instance{};
  IrsServerCore * m_server;

  std::unordered_set<std::string> m_types = {IRS_SERVER_CONSOLE_TYPE, IRS_SERVER_FILE_TYPE };
  std::unordered_set<std::string> m_levels = { IRS_SERVER_DEBUG_LEVEL, IRS_SERVER_INFO_LEVEL, IRS_SERVER_ERROR_LEVEL };
};
