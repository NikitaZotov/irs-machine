#include "stdio.h"

#include "../irs-engine/irs_engine.h"
#include "irs-container/irs-string/irs_string.h"

#define EXPECT_IRS_TRUE(expression) if (expression == IRS_FALSE) g_error("Expect IRS_TRUE, but gotten IRS_FALSE")

#define TEST(name) void name()

void _irs_engine_define_documents_langs(irs_engine * engine, irs_uint32 const coef)
{
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

  double time_spent = 0.0;

  clock_t begin = clock();

  EXPECT_IRS_TRUE(irs_list_push_back(documents,new_string));

  irs_engine_define_documents_languages(engine, documents);
  irs_list_destroy(documents);

  irs_mem_free(new_string);

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time for %d case is %f seconds\n", coef, time_spent);
}

TEST(irs_engine_define_documents_langs_benchmark)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "data/langs", "data/alpha") == IRS_ENGINE_OK);

  _irs_engine_define_documents_langs(engine, 1);
  _irs_engine_define_documents_langs(engine, 2);
  _irs_engine_define_documents_langs(engine, 3);
  _irs_engine_define_documents_langs(engine, 4);
  _irs_engine_define_documents_langs(engine, 5);
  _irs_engine_define_documents_langs(engine, 6);
  _irs_engine_define_documents_langs(engine, 7);
  _irs_engine_define_documents_langs(engine, 8);
  _irs_engine_define_documents_langs(engine, 9);
  _irs_engine_define_documents_langs(engine, 10);

  EXPECT_IRS_TRUE(irs_engine_shutdown(engine) == IRS_ENGINE_OK);
}

void _irs_engine_define_documents_langs_by_alpha(irs_engine * engine, irs_uint32 const coef)
{
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

  double time_spent = 0.0;

  clock_t begin = clock();

  EXPECT_IRS_TRUE(irs_list_push_back(documents,new_string));

  irs_engine_define_documents_languages_by_alphabets(engine, documents);
  irs_list_destroy(documents);

  irs_mem_free(new_string);

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time for %d case is %f seconds\n", coef, time_spent);
}

TEST(irs_engine_define_documents_langs_by_alphabets_benchmark)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "data/langs", "data/alpha") == IRS_ENGINE_OK);

  _irs_engine_define_documents_langs_by_alpha(engine, 1);
  _irs_engine_define_documents_langs_by_alpha(engine, 2);
  _irs_engine_define_documents_langs_by_alpha(engine, 3);
  _irs_engine_define_documents_langs_by_alpha(engine, 4);
  _irs_engine_define_documents_langs_by_alpha(engine, 5);
  _irs_engine_define_documents_langs_by_alpha(engine, 6);
  _irs_engine_define_documents_langs_by_alpha(engine, 7);
  _irs_engine_define_documents_langs_by_alpha(engine, 8);
  _irs_engine_define_documents_langs_by_alpha(engine, 9);
  _irs_engine_define_documents_langs_by_alpha(engine, 10);

  EXPECT_IRS_TRUE(irs_engine_shutdown(engine) == IRS_ENGINE_OK);
}

void _irs_engine_define_documents_summarizations(irs_engine * engine, irs_uint32 const coef)
{
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

  double time_spent = 0.0;

  clock_t begin = clock();

  EXPECT_IRS_TRUE(irs_list_push_back(documents,new_string));

  irs_engine_get_documents_summarizations(engine, documents);
  irs_list_destroy(documents);

  irs_mem_free(new_string);

  clock_t end = clock();

  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

  printf("The elapsed time for %d case is %f seconds\n", coef, time_spent);
}

TEST(irs_engine_define_documents_summarizations_benchmark)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "data/langs", "data/alpha") == IRS_ENGINE_OK);

  _irs_engine_define_documents_summarizations(engine, 1);
  _irs_engine_define_documents_summarizations(engine, 2);
  _irs_engine_define_documents_summarizations(engine, 3);
  _irs_engine_define_documents_summarizations(engine, 4);
  _irs_engine_define_documents_summarizations(engine, 5);
  _irs_engine_define_documents_summarizations(engine, 6);
  _irs_engine_define_documents_summarizations(engine, 7);
  _irs_engine_define_documents_summarizations(engine, 8);
  _irs_engine_define_documents_summarizations(engine, 9);
  _irs_engine_define_documents_summarizations(engine, 10);

  EXPECT_IRS_TRUE(irs_engine_shutdown(engine) == IRS_ENGINE_OK);
}

irs_int32 main()
{
  irs_engine_define_documents_langs_benchmark();
  irs_engine_define_documents_langs_by_alphabets_benchmark();
  irs_engine_define_documents_summarizations_benchmark();
}
