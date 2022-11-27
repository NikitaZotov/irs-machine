#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "irs-base/irs_types.h"

typedef struct _irs_memory irs_memory;

struct IrsMemoryData
{
  std::string dbPath;
  std::string dataPath;
};

class IrsMemory
{
public:
  static IrsMemory * GetInstance(irs_char const * path, irs_char const * data_path);

  ~IrsMemory();

  void Add(std::vector<std::string> const & documents) const;

  [[nodiscard]] std::vector<std::unordered_map<irs_uint64, std::pair<std::string, irs_float>>>
    Get(std::vector<std::string> const & terms) const;

  std::vector<std::string> GetLangs(std::vector<std::string> const & documents) const;

private:
  static IrsMemory * m_instance;

  irs_memory * m_memory;

  explicit IrsMemory(irs_char const * path, irs_char const * data_path);
};
