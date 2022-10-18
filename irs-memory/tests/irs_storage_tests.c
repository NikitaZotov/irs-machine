#include <stdio.h>

#include "../irs-storage/irs-base/irs_allocator.h"
#include "../irs-storage/irs-base/irs_types.h"
#include "../irs-storage/irs_storage.h"
#include "../irs-storage/irs-container/irs-pair/irs_pair.h"

#define EXPECT_TRUE(expression) if (expression == FALSE) g_error("Expect TRUE, but gotten FALSE")

#define TEST(name) void name()

TEST(irs_storage_init_shut)
{
  irs_storage * storage;
  EXPECT_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  EXPECT_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

TEST(irs_storage_init_and_add_documents)
{
  irs_storage * storage;
  EXPECT_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  irs_list * documents;
  irs_list_init(&documents);
  EXPECT_TRUE(irs_list_push_back(documents, "it is the first document"));
  EXPECT_TRUE(irs_list_push_back(documents, "it is the second document"));

  EXPECT_TRUE(irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK);
  irs_list_destroy(documents);

  irs_list * terms;
  irs_list_init(&terms);
  EXPECT_TRUE(irs_list_push_back(terms, "it"));
  EXPECT_TRUE(irs_list_push_back(terms, "first"));
  EXPECT_TRUE(irs_list_push_back(terms, "the"));

  irs_storage_get_documents(storage, terms, &documents);
  irs_list_destroy(terms);

  irs_iterator * terms_documents_it = irs_list_iterator(documents);
  while (irs_iterator_next(terms_documents_it))
  {
    irs_list * documents_with_term = irs_iterator_get(terms_documents_it);

    irs_iterator * term_documents_it = irs_list_iterator(documents_with_term);
    while (irs_iterator_next(term_documents_it))
    {
      irs_pair * pair = irs_iterator_get(term_documents_it);
      printf("%s - %s\n", (irs_char *)pair->first, (irs_char *)pair->second);

      irs_mem_free(pair->first);
      irs_mem_free(pair->second);
    }
    irs_iterator_destroy(term_documents_it);

    irs_list_clear(documents_with_term);
    irs_list_destroy(documents_with_term);
  }
  irs_iterator_destroy(terms_documents_it);

  EXPECT_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

irs_int32 main()
{
  irs_storage_init_shut();
  irs_storage_init_and_add_documents();
}
