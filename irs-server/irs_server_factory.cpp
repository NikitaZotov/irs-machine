#include "irs_server_factory.hpp"

#include "src/irs_server_impl.hpp"

std::unique_ptr<IrsServer> IrsServerFactory::ConfigureIrsServer()
{
  std::unique_ptr<IrsServer> server = std::unique_ptr<IrsServer>(new IrsServerImpl(
      "127.0.0.1",
      7090,
      "File",
      "server.log",
      "Debug"));

  return server;
}
