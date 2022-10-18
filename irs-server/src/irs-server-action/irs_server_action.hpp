#pragma once

#include "../irs_server_defines.hpp"

class IrsServerAction
{
public:
  explicit IrsServerAction(IrsServerConnectionHandle hdl)
    : m_hdl(std::move(hdl))
  {
  }

  virtual std::string Emit() = 0;

  virtual ~IrsServerAction() = default;

protected:
  IrsServerConnectionHandle m_hdl;
};
