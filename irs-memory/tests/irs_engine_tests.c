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

TEST(irs_engine_define_documents_summars)
{
  irs_engine * engine;
  EXPECT_IRS_TRUE(irs_engine_initialize(&engine, "langs/") == IRS_ENGINE_OK);

  irs_list * documents;
  irs_list_init(&documents);
  EXPECT_IRS_TRUE(irs_list_push_back(
    documents,
    "Ludwig van Beethoven (baptised 17 December 1770 – 26 March 1827) was a German composer and pianist. Beethoven "
    "remains one of the most admired composers in the history of Western music; his works rank amongst the most "
    "performed of the classical music repertoire and span the transition from the Classical period to the Romantic "
    "era in classical music. His career has conventionally been divided into early, middle, and late periods. His early "
    "period, during which he forged his craft, is typically considered to have lasted until 1802. From 1802 to around "
    "1812, his middle period showed an individual development from the styles of Joseph Haydn and Wolfgang Amadeus "
    "Mozart, and is sometimes characterized as heroic. During this time, he began to grow increasingly deaf. In his "
    "late period, from 1812 to 1827, he extended his innovations in musical form and expression.\n"
    "Beethoven was born in Bonn. His musical talent was obvious at an early age. He was initially harshly and "
    "intensively taught by his father Johann van Beethoven. Beethoven was later taught by the composer and conductor "
    "Christian Gottlob Neefe, under whose tutelage he published his first work, a set of keyboard variations, in 1783. "
    "He found relief from a dysfunctional home life with the family of Helene von Breuning, whose children he loved, "
    "befriended, and taught piano. At age 21, he moved to Vienna, which subsequently became his base, and studied "
    "composition with Haydn. Beethoven then gained a reputation as a virtuoso pianist, and he was soon patronized by "
    "Karl Alois, Prince Lichnowsky for compositions, which resulted in his three Opus 1 piano trios (the earliest "
    "works to which he accorded an opus number) in 1795.\n"
    "His first major orchestral work, the First Symphony, premiered in 1800, and his first set of string quartets was "
    "published in 1801. Despite his hearing deteriorating during this period, he continued to conduct, premiering his "
    "Third and Fifth Symphonies in 1804 and 1808, respectively. His Violin Concerto appeared in 1806. His last piano "
    "concerto (No. 5, Op. 73, known as the Emperor), dedicated to his frequent patron Archduke Rudolf of Austria, was "
    "premiered in 1811, without Beethoven as soloist. He was almost completely deaf by 1814, and he then gave up "
    "performing and appearing in public. He described his problems with health and his unfulfilled personal life in "
    "two letters, his Heiligenstadt Testament (1802) to his brothers and his unsent love letter to an unknown "
    "\"Immortal Beloved\".\n"
    "After 1810, increasingly less socially involved, Beethoven composed many of his most admired works, including "
    "later symphonies, mature chamber music and the late piano sonatas. His only opera, Fidelio, first performed in "
    "1805, was revised to its final version in 1814. He composed Missa solemnis between 1819 and 1823 and his final "
    "Symphony, No. 9, one of the first examples of a choral symphony, between 1822 and 1824. Written in his last "
    "years, his late string quartets, including the Grosse Fuge, of 1825–1826 are among his final achievements. After "
    "some months of bedridden illness, he died in 1827. Beethoven's works remain mainstays of the classical music "
    "repertoire."));

  irs_list * summars = irs_engine_get_documents_summarizations(engine, documents);
  irs_list_destroy(documents);

  irs_iterator * it = irs_list_iterator(summars);
  while (irs_iterator_next(it))
  {
    irs_list * summar = irs_iterator_get(it);
    irs_iterator * sent_it = irs_list_iterator(summar);
    while (irs_iterator_next(sent_it))
    {
      irs_char * sent = irs_iterator_get(sent_it);
      printf("%s.\n", sent);
    }
    irs_iterator_destroy(sent_it);
  }
  irs_iterator_destroy(it);
  irs_list_destroy(summars);

  EXPECT_IRS_TRUE(irs_engine_shutdown(engine) == IRS_ENGINE_OK);
}

irs_int32 main()
{
  irs_engine_init_shut();
  irs_engine_define_documents_langs();
  irs_engine_define_documents_summars();
}
