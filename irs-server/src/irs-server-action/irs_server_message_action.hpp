#pragma once

#include "irs_server_action.hpp"

#include <utility>

class IrsServerMessageAction : public IrsServerAction
{
public:
  IrsServerMessageAction(IrsServerConnectionHandle hdl, IrsServerMessage msg)
    : IrsServerAction(std::move(hdl))
    , m_msg(std::move(msg))
  {
  }

  std::string Emit() override
  {

  }

  ~IrsServerMessageAction() override
  {
  };

protected:
  IrsServerMessage m_msg;
};
