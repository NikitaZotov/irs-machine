#include <stdio.h>

#include "irs-base/irs_types.h"
#include "../irs-storage/irs_storage.h"
#include "irs-container/irs-string/irs_string.h"

#define EXPECT_IRS_TRUE(expression) if (expression == IRS_FALSE) g_error("Expect IRS_TRUE, but gotten IRS_FALSE")

#define TEST(name) void name()

void _irs_storage_add_documents_N(irs_storage * storage, irs_uint32 const amount)
{
  double time_spent = 0.0;

  clock_t begin = clock();

  irs_list * documents;
  irs_list_init(&documents);

  for (irs_uint32 i = 0; i < amount; ++i)
  {
    EXPECT_IRS_TRUE(irs_list_push_back(
      documents,
      "The cat (Felis catus) is a domestic species of small carnivorous mammal.[1][2] It is the only domesticated "
      "species in the family Felidae and is commonly referred to as the domestic cat or house cat to distinguish it "
      "from the wild members of the family.[4] A cat can either be a house cat, a farm cat, or a feral cat; the latter"
      " ranges freely and avoids human contact.[5] Domestic cats are valued by humans for companionship and their "
      "ability to kill rodents. About 60 cat breeds are recognized by various cat registries."
    ));
  }

  EXPECT_IRS_TRUE(irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK);
  irs_list_destroy(documents);

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time for %d case is %f seconds\n", amount, time_spent);
}

TEST(irs_storage_init_and_add_documents_benchmark)
{
  irs_storage * storage;
  EXPECT_IRS_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  _irs_storage_add_documents_N(storage, 2);
  _irs_storage_add_documents_N(storage, 4);
  _irs_storage_add_documents_N(storage, 8);
  _irs_storage_add_documents_N(storage, 16);
  _irs_storage_add_documents_N(storage, 32);
  _irs_storage_add_documents_N(storage, 64);
  _irs_storage_add_documents_N(storage, 128);
  _irs_storage_add_documents_N(storage, 256);
  _irs_storage_add_documents_N(storage, 512);

  EXPECT_IRS_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

void _irs_storage_add_document_with_coef_length(irs_storage * storage, irs_uint32 const coef)
{
  double time_spent = 0.0;

  clock_t begin = clock();

  irs_list * documents;
  irs_list_init(&documents);

  irs_char * string =
    "The cat (Felis catus) is a domestic species of small carnivorous mammal.[1][2] It is the only domesticated "
    "species in the family Felidae and is commonly referred to as the domestic cat or house cat to distinguish it "
    "from the wild members of the family.[4] A cat can either be a house cat, a farm cat, or a feral cat; the latter"
    " ranges freely and avoids human contact.[5] Domestic cats are valued by humans for companionship and their "
    "ability to kill rodents. About 60 cat breeds are recognized by various cat registries.";

  irs_uint64 const length = irs_str_len(string);
  irs_uint64 const new_length = length * coef;

  irs_char * new_string;
  irs_str_cpy(new_string, string, new_length);

  for (irs_uint32 i = 0; i < coef - 1; ++i)
  {
    strcat(new_string, string);
  }

  EXPECT_IRS_TRUE(irs_list_push_back(documents,new_string));

  EXPECT_IRS_TRUE(irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK);
  irs_list_destroy(documents);

  irs_mem_free(new_string);

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time for %d case is %f seconds\n", coef, time_spent);
}

TEST(irs_storage_init_and_add_diff_documents_benchmark)
{
  irs_storage * storage;
  EXPECT_IRS_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  _irs_storage_add_document_with_coef_length(storage, 1);
  _irs_storage_add_document_with_coef_length(storage, 2);
  _irs_storage_add_document_with_coef_length(storage, 3);
  _irs_storage_add_document_with_coef_length(storage, 4);
  _irs_storage_add_document_with_coef_length(storage, 5);
  _irs_storage_add_document_with_coef_length(storage, 6);
  _irs_storage_add_document_with_coef_length(storage, 7);
  _irs_storage_add_document_with_coef_length(storage, 8);
  _irs_storage_add_document_with_coef_length(storage, 9);
  _irs_storage_add_document_with_coef_length(storage, 10);

  EXPECT_IRS_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

void _irs_storage_add_and_get_documents(irs_storage * storage, irs_uint32 const amount)
{
  irs_list * documents;
  irs_list_init(&documents);

  for (irs_uint32 i = 0; i < amount; ++i)
  {
    EXPECT_IRS_TRUE(irs_list_push_back(
      documents,
      "The cat (Felis catus) is a domestic species of small carnivorous mammal.[1][2] It is the only domesticated "
      "species in the family Felidae and is commonly referred to as the domestic cat or house cat to distinguish it "
      "from the wild members of the family.[4] A cat can either be a house cat, a farm cat, or a feral cat; the latter"
      " ranges freely and avoids human contact.[5] Domestic cats are valued by humans for companionship and their "
      "ability to kill rodents. About 60 cat breeds are recognized by various cat registries."
    ));
  }

  EXPECT_IRS_TRUE(irs_storage_add_documents(storage, documents) == IRS_STORAGE_OK);
  irs_list_destroy(documents);

  double time_spent = 0.0;

  clock_t begin = clock();

  irs_list * terms;
  irs_list_init(&terms);
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "cat"));
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "mammal"));
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "domestication"));
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "Population"));
  EXPECT_IRS_TRUE(irs_list_push_back(terms, "Egypt"));

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

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time for %d case is %f seconds\n", amount, time_spent);
}

TEST(irs_storage_init_add_and_get_documents_benchmark)
{
  irs_storage * storage;
  EXPECT_IRS_TRUE(irs_storage_initialize(&storage, "db/") == IRS_STORAGE_OK);

  _irs_storage_add_and_get_documents(storage, 2);
  _irs_storage_add_and_get_documents(storage, 4);
  _irs_storage_add_and_get_documents(storage, 8);
  _irs_storage_add_and_get_documents(storage, 16);
  _irs_storage_add_and_get_documents(storage, 32);
  _irs_storage_add_and_get_documents(storage, 64);
  _irs_storage_add_and_get_documents(storage, 128);

  _irs_storage_add_and_get_documents(storage, 256);
  _irs_storage_add_and_get_documents(storage, 512);

  EXPECT_IRS_TRUE(irs_storage_shutdown(storage) == IRS_STORAGE_OK);
}

irs_int32 main()
{
  irs_storage_init_and_add_documents_benchmark();
  irs_storage_init_and_add_diff_documents_benchmark();
  irs_storage_init_add_and_get_documents_benchmark();
}
