#include "irs_memory.hpp"

#include "irs_memory.h"
#include "irs-storage/irs-container/irs-list/irs_list.h"
#include "irs-storage/irs-container/irs-pair/irs_pair.h"
#include "irs-storage/irs-base/irs_allocator.h"

IrsMemory::IrsMemory(irs_char const * path)
{
  irs_memory_initialize(&m_storage, path);
}

IrsMemory * IrsMemory::GetInstance(irs_char const * path)
{
  if (m_instance == nullptr)
  {
    return m_instance = new IrsMemory(path);
  }

  return m_instance;
}

IrsMemory::~IrsMemory()
{
  irs_memory_shutdown(m_storage);

  m_instance = nullptr;
}

void IrsMemory::Add(std::vector<std::string> const & documents)
{
  irs_list * list;
  irs_list_init(&list);

  for (auto const & doc : documents)
    irs_list_push_back(list, (irs_char *)doc.c_str());

  irs_memory_add_documents(m_storage, list);

  irs_list_destroy(list);
}

std::unordered_map<std::string, std::unordered_map<std::string, irs_float>>
  IrsMemory::Get(std::vector<std::string> const & terms)
{
  irs_list * list;
  irs_list_init(&list);

  for (auto const & term : terms)
    irs_list_push_back(list, (irs_char *)term.c_str());

  irs_list * documentsWithSignificancies;
  irs_memory_get_documents(m_storage, list, &documentsWithSignificancies);

  std::unordered_map<std::string, std::unordered_map<std::string, float>> map;
  irs_uint32 count = 0;

  irs_iterator * termsDocumentsIt = irs_list_iterator(documentsWithSignificancies);
  while (irs_iterator_next(termsDocumentsIt))
  {
    irs_list * documentsWithTerm = (irs_list *)irs_iterator_get(termsDocumentsIt);

    std::unordered_map<std::string, float> documents;

    irs_iterator * termDocumentsIt = irs_list_iterator(documentsWithTerm);
    while (irs_iterator_next(termDocumentsIt))
    {
      irs_pair * pair = (irs_pair *)irs_iterator_get(termDocumentsIt);

      std::string const documentStr = std::string((irs_char*)pair->first);
      irs_float const termSignificancy = std::stof(std::string((irs_char*)pair->second));
      documents.insert({ documentStr, termSignificancy });

      irs_mem_free(pair->first);
      irs_mem_free(pair->second);
    }
    irs_iterator_destroy(termDocumentsIt);

    irs_list_clear(documentsWithTerm);
    irs_list_destroy(documentsWithTerm);

    map.insert({ terms[count], documents });

    ++count;
  }
  irs_iterator_destroy(termsDocumentsIt);

  irs_list_destroy(list);
}
