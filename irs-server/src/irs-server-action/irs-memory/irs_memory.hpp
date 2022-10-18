#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "irs-storage/irs-base/irs_types.h"

typedef struct _irs_storage irs_storage;

class IrsMemory
{
public:
  static IrsMemory * GetInstance(irs_char const * path);

  ~IrsMemory();

  void Add(std::vector<std::string> const & documents) const;

  std::unordered_map<std::string, std::unordered_map<std::string, irs_float>>
    Get(std::vector<std::string> const & terms) const;

private:
  static IrsMemory * m_instance;

  irs_storage * m_storage;

  IrsMemory(irs_char const * path);
};
