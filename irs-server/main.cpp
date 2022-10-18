#include <iostream>
#include <atomic>
#include <thread>

#include "irs_server_factory.hpp"

#include "irs_signal_handler.hpp"

irs_int32 main(irs_int32 argc, irs_char * argv[])
try
{
  irs_bool status = IRS_FALSE;
  std::unique_ptr<IrsServer> server;
  try
  {
    server = IrsServerFactory::ConfigureIrsServer();
    server->Run();
    status = IRS_TRUE;
  }
  catch (std::exception const & e)
  {
    server->ClearLogOptions();
    server->LogError(IrsServerLogErrors::rerror, e.what());
  }

  if (status == IRS_FALSE)
  {
    try
    {
      server->Stop();
    }
    catch (std::exception const & e)
    {
      server->LogError(IrsServerLogErrors::rerror, e.what());
    }

    return EXIT_FAILURE;
  }

  std::atomic_bool isRun = { IRS_TRUE };
  utils::IrsSignalHandler::Initialize();
  utils::IrsSignalHandler::m_onTerminate = [&isRun]() {
    isRun = IRS_FALSE;
  };

  while (isRun)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  server->Stop();

  return EXIT_SUCCESS;
}
catch (std::exception const & e)
{
  std::cout << e.what() << std::endl;
}
