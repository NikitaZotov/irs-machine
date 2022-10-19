#include <stdio.h>

#include "../irs-storage/irs-base/irs_allocator.h"
#include "../irs-storage/irs-base/irs_types.h"
#include "../irs-storage/irs_storage.h"
#include "../irs-storage/irs-container/irs-pair/irs_pair.h"

#define EXPECT_IRS_TRUE(expression) if (expression == IRS_FALSE) g_error("Expect IRS_TRUE, but gotten IRS_FALSE")

#define TEST(name) void name()

TEST(irs_storage_init_shut)
{
  irs_storage * storage;
  EXPECT_IRS_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  EXPECT_IRS_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

TEST(irs_storage_init_and_add_documents)
{
  irs_storage * storage;
  EXPECT_IRS_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  irs_list * documents;
  irs_list_init(&documents);
  EXPECT_IRS_TRUE(irs_list_push_back(documents, "it is the first document"));
  EXPECT_IRS_TRUE(irs_list_push_back(documents, "it is the second document"));

  EXPECT_IRS_TRUE(irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK);
  irs_list_destroy(documents);

  irs_list * terms;
  irs_list_init(&terms);
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "it"));
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "first"));
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "the"));

  irs_storage_get_documents(storage, terms, &documents);
  irs_list_destroy(terms);

  irs_iterator * terms_documents_it = irs_list_iterator(documents);
  while (irs_iterator_next(terms_documents_it))
  {
    irs_list * documents_with_term = irs_iterator_get(terms_documents_it);

    irs_iterator * term_documents_it = irs_list_iterator(documents_with_term);
    while (irs_iterator_next(term_documents_it))
    {
      irs_list * triple = irs_iterator_get(term_documents_it);

      irs_iterator * triple_it = irs_list_iterator(triple);

      irs_iterator_next(triple_it);
      irs_uint64 const offset = *(irs_uint64 *)irs_iterator_get(triple_it);

      irs_iterator_next(triple_it);
      irs_char * const documentStr = (irs_char *)irs_iterator_get(triple_it);

      irs_iterator_next(triple_it);
      irs_char * const termSignificancy = (irs_char *)irs_iterator_get(triple_it);
      irs_iterator_destroy(triple_it);

      printf("%lu - %s - %s\n", offset, (irs_char *)documentStr, (irs_char *)termSignificancy);
    }
    irs_iterator_destroy(term_documents_it);

    irs_list_clear(documents_with_term);
    irs_list_destroy(documents_with_term);
  }
  irs_iterator_destroy(terms_documents_it);

  EXPECT_IRS_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

irs_int32 main()
{
  irs_storage_init_shut();
  irs_storage_init_and_add_documents();
}
