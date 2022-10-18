#pragma once

#include "irs_server_action.hpp"
#include "irs_memory_json_handler.hpp"

#include <utility>

class IrsServerMessageAction : public IrsServerAction
{
public:
  IrsServerMessageAction(IrsServerConnectionHandle hdl, IrsServerMessage msg)
    : IrsServerAction(std::move(hdl))
    , m_msg(std::move(msg))
  {

  }

  std::string Emit(IrsMemory const & memory) override
  {
    return m_handler.Handle(memory, m_hdl, m_msg->get_payload());
  }

  ~IrsServerMessageAction() override = default;;

protected:
  IrsMemoryJsonHandler m_handler;

  IrsServerMessage m_msg;
};
