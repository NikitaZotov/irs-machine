#pragma once

#include "src/irs_server.hpp"

class IrsServerFactory
{
public:
  static std::unique_ptr<IrsServer> ConfigureIrsServer(std::string const & configFilePath);

  static std::vector<std::string> GetKeys();
};
