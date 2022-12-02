#include "irs_server_factory.hpp"

#include "src/irs_server_impl.hpp"
#include "irs-config/irs_config.hpp"

std::unique_ptr<IrsServer> IrsServerFactory::ConfigureIrsServer(std::string const & configFilePath)
{
  IrsConfig config{configFilePath, IrsServerFactory::GetKeys()};
  IrsConfigGroup serverGroup = config["irs-server"];

  IrsConfigGroup memoryGroup = config["irs-memory"];
  IrsMemoryData data = {
    memoryGroup["db_path"],
    memoryGroup["lang_key_words_path"],
    memoryGroup["lang_alpha_path"]};

  std::unique_ptr<IrsServer> server = std::unique_ptr<IrsServer>(new IrsServerImpl(
      serverGroup["host"],
      serverGroup["port"].empty() ? 7090 : std::stoi(serverGroup["port"]),
      serverGroup["log_type"],
      serverGroup["log_file"],
      serverGroup["log_level"],
      data));

  return server;
}

std::vector<std::string> IrsServerFactory::GetKeys()
{
  return { "db_path", "lang_key_words_path", "lang_alpha_path" };
}
