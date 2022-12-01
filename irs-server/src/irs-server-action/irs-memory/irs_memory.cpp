#include "irs_memory.hpp"

#include <sstream>

extern "C"
{
#include "irs_memory.h"
#include "irs-container/irs-list/irs_list.h"
#include "irs-container/irs-pair/irs_pair.h"
}

IrsMemory * IrsMemory::m_instance = nullptr;

IrsMemory::IrsMemory(irs_char const * path, irs_char const * data_path)
{
  irs_memory_initialize(&m_memory, path, data_path);
  irs_memory_load(m_memory);
}

IrsMemory * IrsMemory::GetInstance(irs_char const * path, irs_char const * data_path)
{
  if (m_instance == nullptr)
  {
    return m_instance = new IrsMemory(path, data_path);
  }

  return m_instance;
}

IrsMemory::~IrsMemory()
{
  irs_memory_save(m_memory);
  irs_memory_shutdown(m_memory);

  m_instance = nullptr;
}

void IrsMemory::Add(std::vector<std::string> const & documents) const
{
  irs_list * list;
  irs_list_init(&list);

  for (auto const & doc : documents)
    irs_list_push_back(list, (irs_char *)doc.c_str());

  irs_memory_add_documents(m_memory, list);

  irs_list_destroy(list);
}

std::vector<std::unordered_map<irs_uint64, std::pair<std::string, irs_float>>>
  IrsMemory::Get(std::vector<std::string> const & terms) const
{
  irs_list * list;
  irs_list_init(&list);

  for (auto const & term : terms)
    irs_list_push_back(list, (irs_char *)term.c_str());

  irs_list * documentsWithSignificancies;
  irs_memory_get_documents(m_memory, list, &documentsWithSignificancies);

  std::vector<std::unordered_map<irs_uint64, std::pair<std::string, irs_float>>> result;
  irs_uint32 count = 0;

  irs_iterator * termsDocumentsIt = irs_list_iterator(documentsWithSignificancies);
  while (irs_iterator_next(termsDocumentsIt))
  {
    auto * documentsWithTerm = (irs_list *)irs_iterator_get(termsDocumentsIt);
    if (documentsWithTerm == nullptr)
    {
      result.push_back({});
      continue;
    }

    std::unordered_map<irs_uint64, std::pair<std::string, irs_float>> documents;

    irs_iterator * termDocumentsIt = irs_list_iterator(documentsWithTerm);
    while (irs_iterator_next(termDocumentsIt))
    {
      auto * triple = (irs_list *)irs_iterator_get(termDocumentsIt);

      irs_iterator * triple_it = irs_list_iterator(triple);

      irs_iterator_next(triple_it);
      irs_uint64 const offset = *(irs_uint64 *)irs_iterator_get(triple_it);

      irs_iterator_next(triple_it);
      std::string const documentStr = std::string((irs_char*)irs_iterator_get(triple_it));

      irs_iterator_next(triple_it);
      irs_float const termSignificancy = std::stof(std::string((irs_char*)irs_iterator_get(triple_it)));
      irs_iterator_destroy(triple_it);

      documents.insert({ offset, { documentStr, termSignificancy } });
    }
    irs_iterator_destroy(termDocumentsIt);

    irs_list_clear(documentsWithTerm);
    irs_list_destroy(documentsWithTerm);

    result.push_back({ documents });

    ++count;
  }
  irs_iterator_destroy(termsDocumentsIt);

  irs_list_destroy(list);

  return result;
}

std::vector<std::string> IrsMemory::GetLangs(std::vector<std::string> const & documents) const
{
  irs_list * list;
  irs_list_init(&list);

  for (auto const & document : documents)
    irs_list_push_back(list, (irs_char *)document.c_str());

  irs_list * langs;
  irs_memory_get_documents_langs(m_memory, list, &langs);

  std::vector<std::string> langs_vector;
  irs_iterator * it = irs_list_iterator(langs);
  while (irs_iterator_next(it))
  {
    langs_vector.emplace_back((irs_char *)irs_iterator_get(it));
  }
  irs_iterator_destroy(it);
  irs_list_destroy(langs);

  return langs_vector;
}

std::vector<std::string> IrsMemory::GetSummarizations(std::vector<std::string> const & documents) const
{
  irs_list * list;
  irs_list_init(&list);

  for (auto const & document : documents)
    irs_list_push_back(list, (irs_char *)document.c_str());

  irs_list * summarizations;
  irs_memory_get_documents_summarizations(m_memory, list, &summarizations);

  std::vector<std::string> langs_vector;
  irs_iterator * it = irs_list_iterator(summarizations);
  while (irs_iterator_next(it))
  {
    auto * sentences = (irs_list *)irs_iterator_get(it);
    irs_iterator * sent_it = irs_list_iterator(sentences);
    std::stringstream sstream;
    while (irs_iterator_next(sent_it))
    {
     auto * sent = (irs_char *)irs_iterator_get(sent_it);
     sstream << sent << ".";
    }
    irs_iterator_destroy(sent_it);
    irs_list_destroy(sentences);
    langs_vector.emplace_back(sstream.str());
  }
  irs_iterator_destroy(it);
  irs_list_destroy(summarizations);

  return langs_vector;
}
