#pragma once

#include <algorithm>
#include <vector>
#include <sstream>

#include "irs-base/irs_types.h"

class IrsOptions
{
public:
  IrsOptions(irs_int32 const & argc, char ** argv)
  {
    for (irs_int32 i = 1; i < argc; ++i)
      m_tokens.emplace_back(argv[i]);
  }

  template <typename... Args>
  std::pair<std::string, std::string> Get(Args const &... options) const
  {
    std::vector<std::string> optionVector{options...};
    for (auto const & item : optionVector)
    {
      std::string const & option = Upstream(item);

      auto it = std::find(m_tokens.begin(), m_tokens.end(), option);
      if (it != m_tokens.end() && ++it != m_tokens.end())
        return {item, *it};
    }

    return {};
  }


  template <typename... Args>
  irs_bool Has(Args const &... options)
  {
    std::vector<std::string> optionVector{options...};
    for (auto const & item : optionVector)
    {
      std::string const & option = Upstream(item);

      if (std::find(m_tokens.begin(), m_tokens.end(), option) != m_tokens.end())
        return IRS_TRUE;
    }

    return IRS_FALSE;
  }

private:
  std::vector<std::string> m_tokens;

  static std::string Upstream(std::string const & option)
  {
    std::stringstream stream;
    stream << "-";

    if (option.length() > 1)
      stream << "-";

    stream << option;

    return stream.str();
  }
};
