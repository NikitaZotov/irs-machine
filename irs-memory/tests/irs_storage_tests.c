#include "../irs-storage/irs-base/irs_types.h"
#include "../irs-storage/irs_storage.h"

#define EXPECT_TRUE(expression) if (expression == FALSE) g_error("Expect TRUE, but gotten FALSE")

#define TEST(name) void name()

TEST(irs_storage_init_shut)
{
  irs_storage * storage;
  EXPECT_TRUE(irs_storage_initialize(&storage, "irs-machine/db/") == IRS_STORAGE_OK);

  EXPECT_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

TEST(irs_storage_init_and_add_documents)
{
  irs_storage * storage;
  EXPECT_TRUE(irs_storage_initialize(&storage, "irs-machine/db/") == IRS_STORAGE_OK);

  irs_list * documents;
  irs_list_init(&documents);
  EXPECT_TRUE(irs_list_push_back(documents, "it is a the first document"));
  EXPECT_TRUE(irs_list_push_back(documents, "it is a the second document"));

  EXPECT_TRUE(irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK);
  irs_list_destroy(documents);

  EXPECT_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

irs_int32 main()
{
  irs_storage_init_shut();
  irs_storage_init_and_add_documents();
}
