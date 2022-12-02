#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "irs-base/irs_types.h"

typedef struct _irs_memory irs_memory;

struct IrsMemoryData
{
  std::string dbPath;
  std::string langKeyWordsPath;
  std::string langAlphaPath;
};

class IrsMemory
{
public:
  static IrsMemory * GetInstance(
    irs_char const * path, irs_char const * lang_key_words_path, irs_char const * lang_alpha_path);

  ~IrsMemory();

  void Add(std::vector<std::string> const & documents) const;

  [[nodiscard]] std::vector<std::unordered_map<irs_uint64, std::pair<std::string, irs_float>>>
    Get(std::vector<std::string> const & terms) const;

  [[nodiscard]] std::vector<std::string> GetLangs(irs_uint8 type, std::vector<std::string> const & documents) const;

  [[nodiscard]] std::vector<std::string> GetSummarizations(std::vector<std::string> const & documents) const;

private:
  static IrsMemory * m_instance;

  irs_memory * m_memory;

  explicit IrsMemory(irs_char const * path, irs_char const * lang_key_words_path, irs_char const * lang_alpha_path);
};
