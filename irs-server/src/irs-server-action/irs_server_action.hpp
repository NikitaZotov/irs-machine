#pragma once

#include "../irs_server_defines.hpp"
#include "irs-memory/irs_memory.hpp"

class IrsServerAction
{
public:
  explicit IrsServerAction(IrsServerConnectionHandle hdl)
    : m_hdl(std::move(hdl))
  {
  }

  virtual std::string Emit(IrsMemory const & memory) = 0;

  virtual ~IrsServerAction() = default;

public:
  IrsServerConnectionHandle m_hdl;
};
