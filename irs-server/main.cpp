#include <iostream>
#include <atomic>
#include <thread>

#include "irs_server_factory.hpp"

#include "irs_signal_handler.hpp"
#include "irs-config/irs_options.hpp"

irs_int32 main(irs_int32 argc, irs_char * argv[])
try
{
  IrsOptions options{argc, argv};
  if (options.Has("help"))
  {
    std::cout << "IRS-MACHINE USAGE\n\n"
              << "--config|-c -- Path to configuration file\n"
              << "--host|-h -- Irs-server host name, ip-address\n"
              << "--port|-p -- Irs-server port\n"
              << "--help -- Display this message\n";
    return EXIT_SUCCESS;
  }

  std::string configFile;
  if (options.Has("config", "c"))
    configFile = options.Get("config", "c").second;

  irs_bool status = IRS_FALSE;
  std::unique_ptr<IrsServer> server;
  try
  {
    server = IrsServerFactory::ConfigureIrsServer(configFile);
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
