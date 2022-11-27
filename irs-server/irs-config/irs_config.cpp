#include "irs_config.hpp"

#include <string>
#include <sstream>
#include <utility>
#include <algorithm>
#include <vector>

extern "C"
{
#include "irs-container/irs-string/irs_string.h"
#include "irs_config.h"
}

IrsConfigGroup::IrsConfigGroup(
    irs_config * config,
    std::string configPath,
    std::vector<std::string> pathKeys,
    std::string group)
    : m_config(config)
    , m_configPath(std::move(configPath))
    , m_pathKeys(std::move(pathKeys))
    , m_group(std::move(group))
{
  irs_list * keys = irs_config_get_keys(m_config);

  irs_list * element = irs_list_get_first(keys);
  while (element != NULL_PTR)
  {
    if (irs_str_find((irs_char *)element->data, m_group.c_str()))
    {
      std::string groupKey = std::string((irs_char *)element->data);
      m_keys.push_back(groupKey.erase(0, m_group.size() + 1));
    }

    element = element->next;
  }

  irs_list_free(keys);
}

std::string IrsConfigGroup::operator[](std::string const & key) const
{
  std::string const & value = irs_config_get_value_string(m_config, m_group.c_str(), key.c_str());
  std::stringstream stream;

  auto const & it = std::find(m_pathKeys.cbegin(), m_pathKeys.cend(), key);

  if (it == m_pathKeys.cend())
    return value;

  if (value[0] == '/')
    stream << value;
  else
    stream << m_configPath << value;

  return stream.str();
}

std::vector<std::string> IrsConfigGroup::operator*() const
{
  return m_keys;
}

IrsConfig::IrsConfig(std::string path, std::vector<std::string> paths)
    : m_path(std::move(path))
    , m_pathKeys(std::move(paths))
{
  m_result = irs_config_initialize(&m_instance, m_path.c_str());
}

irs_bool IrsConfig::IsValid() const
{
  return m_result;
}

IrsConfigGroup IrsConfig::operator[](std::string const & group) const
{
  return IrsConfigGroup(this->m_instance, this->GetDirectory(), this->m_pathKeys, group);
}

IrsConfig::~IrsConfig()
{
  irs_config_shutdown(m_instance);
  m_instance = nullptr;
}
