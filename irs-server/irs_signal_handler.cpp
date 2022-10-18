#include "irs_signal_handler.hpp"

#include <csignal>

namespace handlers
{

void sc_signal_handler(int s)
{
  if (utils::IrsSignalHandler::m_onTerminate)
    utils::IrsSignalHandler::m_onTerminate();
}

}  // namespace handlers

namespace utils
{
IrsSignalHandler::HandlerDelegate IrsSignalHandler::m_onTerminate;

void IrsSignalHandler::Initialize()
{
  struct sigaction sigIntHandler
  {
  };

  sigIntHandler.sa_handler = handlers::sc_signal_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, nullptr);
}

}  // namespace utils
