#pragma once

#include <string>
#include <vector>

extern "C"
{
#include "irs_config.h"
#include <typeinfo>
}

using irs_list = GList;

class IrsConfigGroup
{
public:
  explicit IrsConfigGroup(
      irs_config * config,
      std::string configPath,
      std::vector<std::string> pathKeys,
      std::string group);

  std::string operator[](std::string const & key) const;

  std::vector<std::string> operator*() const;

  ~IrsConfigGroup() = default;

private:
  irs_config * m_config;
  std::string m_configPath;
  std::vector<std::string> m_pathKeys;
  std::string m_group;
  std::vector<std::string> m_keys;
};

class IrsConfig
{
public:
  explicit IrsConfig(std::string path, std::vector<std::string> paths = {});

  irs_bool IsValid() const;

  template <class Type>
  Type Get(std::string const & group, std::string const & key) const
  {
    if (typeid(Type) == typeid(std::string) || typeid(Type) == typeid(irs_char *))
      return irs_config_get_value_string(m_instance, group.c_str(), key.c_str());
    else if (typeid(Type) == typeid(irs_uint32))
      return irs_config_get_value_int(m_instance, group.c_str(), key.c_str());
    else if (typeid(Type) == typeid(float))
      return irs_config_get_value_float(m_instance, group.c_str(), key.c_str());
    else if (typeid(Type) == typeid(irs_bool))
      return irs_config_get_value_boolean(m_instance, group.c_str(), key.c_str());
    else
      return 0;
  }

  IrsConfigGroup operator[](std::string const & group) const;

  [[nodiscard]] std::string GetDirectory() const
  {
    return m_path.substr(0, m_path.rfind('/') + 1);
  }

  ~IrsConfig();

private:
  std::string m_path;
  std::vector<std::string> m_pathKeys;
  irs_config * m_instance{};

  irs_bool m_result;
};
