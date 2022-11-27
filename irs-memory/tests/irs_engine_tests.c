#include "stdio.h"

#include "../irs-engine/irs_engine.h"

#define EXPECT_IRS_TRUE(expression) if (expression == IRS_FALSE) g_error("Expect IRS_TRUE, but gotten IRS_FALSE")

#define TEST(name) void name()

TEST(irs_engine_init_shut)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "langs/") == IRS_ENGINE_OK);

  EXPECT_IRS_TRUE(irs_engine_shutdown(engine) == IRS_ENGINE_OK);
}

TEST(irs_engine_define_documents_langs)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "langs/") == IRS_ENGINE_OK);

  irs_list * documents;
  irs_list_init(&documents);
  EXPECT_IRS_TRUE(irs_list_push_back(
      documents,
      "La science a pour objet de comprendre et d'expliquer le monde et ses phénomènes au départ de la connaissance, "
      "dans le but d'en tirer des prévisions et des applications fonctionnelles. Elle se veut ouverte à la critique tant "
      "au niveau des connaissances acquises, des méthodes utilisées pour les acquérir et de l'argumentation utilisée lors "
      "de la recherche scientifique ou participative"));
  EXPECT_IRS_TRUE(irs_list_push_back(
      documents,
      "L'insegnamento della scienza e la ricerca scientifica vengono praticati prevalentemente nelle università, negli "
      "istituti di ricerca e nelle imprese. Vengono detti scienziati tutti coloro che si dedicano alla ricerca di nuove "
      "conoscenze utilizzando metodi scientifici. La scienza moderna si sviluppa in modo particolare a partire dalla "
      "rivoluzione scientifica del XVI secolo con l'accumulo di conoscenze nei più svariati ambiti del sapere. La storia "
      "della scienza ne descrive lo sviluppo nel tempo."));

  irs_list * langs = irs_engine_define_documents_languages(engine, documents);
  irs_list_destroy(documents);

  irs_iterator * it = irs_list_iterator(langs);
  while (irs_iterator_next(it))
  {
    irs_char * lang = irs_iterator_get(it);
    printf("%s", lang);
  }
  irs_iterator_destroy(it);
  irs_list_destroy(langs);

  EXPECT_IRS_TRUE(irs_engine_shutdown(engine) == IRS_ENGINE_OK);
}

irs_int32 main()
{
  irs_engine_init_shut();
  irs_engine_define_documents_langs();
}
