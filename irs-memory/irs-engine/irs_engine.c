#include "irs_engine.h"

#include "irs-file-system/irs_file_system.h"
#include "irs-base/irs_allocator.h"
#include "irs-container/irs-string/irs_string.h"
#include "irs-container/irs-pair/irs_pair.h"

irs_uint8 _irs_dictionary_children_size()
{
  irs_uint8 const max_irs_char = 255;
  irs_uint8 const min_irs_char = 1;

  return max_irs_char - min_irs_char + 1;
}

void _irs_char_to_irs_int(irs_char ch, irs_uint8 * ch_num, irs_uint8 * mask)
{
  *ch_num = 128 + (irs_uint8)ch;
}

irs_pair * _irs_engine_read_lang_file(irs_dictionary * dictionary, irs_char const * path)
{
  irs_io_channel * channel = irs_io_new_channel(path, "r", NULL_PTR);
  irs_io_channel_set_encoding(channel, NULL_PTR, NULL_PTR);

  irs_char * word;
  irs_uint64 length;

  irs_pair * dictionary_lang;
  GIOStatus status = g_io_channel_read_line(channel, &word, &length, NULL_PTR, NULL_PTR);
  irs_char * copy;
  irs_str_cpy(copy, word, length - 1);
  irs_pair_initialize(&dictionary_lang, dictionary, copy);

  while (status != G_IO_STATUS_EOF)
  {
    irs_str_cpy(copy, word, length - 1);
    irs_dictionary_append(dictionary, copy, length - 1, NULL_PTR);
    irs_mem_free(word);

    status = g_io_channel_read_line(channel, &word, &length, NULL_PTR, NULL_PTR);
  }
  irs_io_channel_shutdown(channel, IRS_TRUE, NULL_PTR);

  return dictionary_lang;
}

irs_char * _irs_init_lang_file_path(irs_char const * path, irs_char const * postfix)
{
  irs_uint32 size = irs_str_len(path) + irs_str_len(postfix) + 2;
  irs_char * out_path = irs_mem_new(irs_char, size + 1);
  irs_str_printf(out_path, size, "%s/%s", path, postfix);
  return out_path;
}

void _irs_engine_read_lang_dictionaries(irs_engine * engine, irs_char const * path)
{
  if (engine->lang_key_words == NULL_PTR)
  {
    irs_list_init(&engine->lang_key_words);
  }

  GDir * dir;
  GError * error;
  irs_char const * filename;

  dir = g_dir_open(path, 0, &error);
  while ((filename = g_dir_read_name(dir)))
  {
    irs_dictionary * dict;
    irs_dictionary_initialize(&dict, _irs_dictionary_children_size(), _irs_char_to_irs_int);

    irs_char * file_path = _irs_init_lang_file_path(path, filename);
    irs_pair * pair = _irs_engine_read_lang_file(dict, file_path);
    irs_list_push_back(engine->lang_key_words, pair);
  }
}

irs_engine_status irs_engine_initialize(irs_engine ** engine, irs_char const * path)
{
  if (path == NULL_PTR)
    return IRS_ENGINE_WRONG_PATH;

  if (irs_fs_isdir(path) == IRS_FALSE)
  {
    if (irs_fs_mkdirs(path) == IRS_FALSE)
      return IRS_ENGINE_WRONG_PATH;
  }

  *engine = irs_mem_new(irs_engine, 1);
  {
    irs_str_cpy((*engine)->path, path, irs_str_len(path));
    _irs_engine_read_lang_dictionaries(*engine, path);
  }

  return IRS_ENGINE_OK;
}

void _irs_engine_clear_lang_dictionaries(irs_engine * engine)
{
  irs_iterator * it = irs_list_iterator(engine->lang_key_words);
  while (irs_iterator_next(it))
  {
    irs_pair * pair = irs_iterator_get(it);
    irs_dictionary_destroy(pair->first);
    irs_mem_free(pair->second);
    irs_mem_free(pair);
  }
  irs_iterator_destroy(it);
  irs_list_destroy(engine->lang_key_words);
  engine->lang_key_words = NULL_PTR;
}

irs_engine_status irs_engine_shutdown(irs_engine * engine)
{
  if (engine == NULL_PTR)
    return IRS_ENGINE_NO;

  irs_mem_free(engine->path);
  _irs_engine_clear_lang_dictionaries(engine);
  irs_mem_free(engine);

  return IRS_ENGINE_OK;
}

irs_list * _irs_storage_get_document_short_terms(irs_char * document)
{
  static const irs_char delim[] = " ,.-\0()[]_";
  irs_uint64 size = irs_str_len(document);
  irs_char string[size + 1];
  irs_mem_cpy(string, document, size);
  string[size] = '\0';

  irs_char * term = strtok(string, delim);
  irs_list * terms;
  irs_list_init(&terms);
  while (term != NULL_PTR)
  {
    irs_uint64 const term_length = irs_str_len(term);

    static const irs_uint64 MAX_LENGTH = 7;
    if (term_length <= MAX_LENGTH)
    {
      irs_char * term_copy;
      irs_str_cpy(term_copy, term, term_length);
      irs_list_push_back(terms, term_copy);
    }
    term = strtok(NULL_PTR, delim);
  }

  return terms;
}

irs_list * irs_engine_define_documents_languages(irs_engine * engine, irs_list * documents)
{
  irs_list * langs;
  irs_list_init(&langs);

  irs_iterator * it = irs_list_iterator(documents);
  while (irs_iterator_next(it))
  {
    irs_char * document = irs_iterator_get(it);

    irs_list * document_lang_common_coincidences;
    irs_list_init(&document_lang_common_coincidences);

    irs_list * terms = _irs_storage_get_document_short_terms(document);

    irs_iterator * lang_dict_it = irs_list_iterator(engine->lang_key_words);
    while (irs_iterator_next(lang_dict_it))
    {
      irs_uint32 common_coincidences = 0;

      irs_iterator * term_it = irs_list_iterator(terms);
      while (irs_iterator_next(term_it))
      {
        irs_char * term = irs_iterator_get(term_it);

        {
          irs_pair * dictionary_lang = irs_iterator_get(lang_dict_it);
          irs_dictionary * dictionary = dictionary_lang->first;
          if (irs_dictionary_is_in(dictionary, term))
            ++common_coincidences;
        }
      }
      irs_iterator_destroy(term_it);

      irs_list_push_back(document_lang_common_coincidences, (void *)common_coincidences);
    }
    irs_iterator_destroy(lang_dict_it);

    irs_list_clear(terms);
    irs_list_destroy(terms);

    irs_uint32 position = 0;
    irs_uint32 max_common_coincidences = 0;
    irs_uint32 max_common_coincidences_pos = 0;
    irs_iterator * coincidence_it = irs_list_iterator(document_lang_common_coincidences);
    while (irs_iterator_next(coincidence_it))
    {
      irs_uint32 common_coincidences = (irs_uint32)irs_iterator_get(coincidence_it);
      if (max_common_coincidences < common_coincidences)
      {
        max_common_coincidences = common_coincidences;
        max_common_coincidences_pos = position;
      }

      ++position;
    }
    irs_iterator_destroy(coincidence_it);
    irs_list_destroy(document_lang_common_coincidences);

    position = 0;
    lang_dict_it = irs_list_iterator(engine->lang_key_words);
    while (irs_iterator_next(lang_dict_it))
    {
      if (position == max_common_coincidences_pos)
      {
        irs_pair * dictionary_lang = irs_iterator_get(lang_dict_it);
        irs_char * defined_lang = dictionary_lang->second;
        irs_list_push_back(langs, defined_lang);
      }

      ++position;
    }
    irs_iterator_destroy(lang_dict_it);
  }

  return langs;
}

irs_list * _irs_storage_get_document_sentences(irs_char * document)
{
  static const irs_char delim[] = ".!?";
  irs_uint64 size = irs_str_len(document);
  irs_char string[size + 1];
  irs_mem_cpy(string, document, size);
  string[size] = '\0';

  irs_char * sentence = strtok(string, delim);
  irs_list * sentences;
  irs_list_init(&sentences);
  while (sentence != NULL_PTR)
  {
    irs_uint64 const term_length = irs_str_len(sentence);

    irs_char * term_copy;
    irs_str_cpy(term_copy, sentence, term_length);
    irs_list_push_back(sentences, term_copy);

    sentence = strtok(NULL_PTR, delim);
  }

  return sentences;
}

irs_float _irs_storage_define_document_sentence_posd(irs_char * document, irs_char * sentence)
{
  irs_float const doc_size = irs_str_len(document);
  irs_float const sen_pos = irs_str_find_position(document, sentence);
  irs_float const posd = 1.0f - sen_pos / doc_size;
  return posd;
}

irs_float _irs_storage_define_document_sentence_posp(irs_char * doc_part_with_sent, irs_char * sentence)
{
  irs_float const doc_part_with_sent_size = irs_str_len(doc_part_with_sent);
  irs_float const sen_pos = irs_str_find_position(doc_part_with_sent, sentence);
  irs_float const posd = 1.0f - sen_pos / doc_part_with_sent_size;
  return posd;
}

irs_char * _irs_storage_define_document_part_with_sentence(irs_char * document, irs_char * sentence)
{
  irs_char * isent = irs_str_find_get(document, sentence);

  irs_char const PART_SIGN = '\n';

  irs_char * doc_part_with_sent_start = isent;
  for (; doc_part_with_sent_start != document && *doc_part_with_sent_start != PART_SIGN; --doc_part_with_sent_start);

  irs_char * doc_end = document + irs_str_len(document);
  irs_char * doc_part_with_sent_end = isent + 1;
  for (; doc_part_with_sent_end != doc_end && *doc_part_with_sent_end != PART_SIGN; ++doc_part_with_sent_end);

  irs_char * sent_part;
  irs_uint64 const sent_part_length = doc_part_with_sent_end - doc_part_with_sent_start + 1;
  irs_str_cpy(sent_part, doc_part_with_sent_start, sent_part_length);

  return sent_part;
}

irs_uint64 _irs_define_sentence_score(irs_char * document, irs_char * sentence)
{
  irs_float const posd = _irs_storage_define_document_sentence_posd(document, sentence);

  irs_char * doc_part_with_sent = _irs_storage_define_document_part_with_sentence(document, sentence);
  irs_float const posp = _irs_storage_define_document_sentence_posp(doc_part_with_sent, sentence);

  static irs_float const SCALE = 10000;
  irs_uint64 const score = (irs_uint64)(posd * posp * SCALE);
  return score;
}

irs_list * _irs_sort_doc_sentences_by_score(irs_list * sents_scores)
{
  irs_list * sorted_sents_scores;
  irs_list_init(&sorted_sents_scores);

  irs_iterator * sent_score_it = irs_list_iterator(sents_scores);
  if (irs_iterator_next(sent_score_it))
  {
    irs_pair * pair = (irs_pair *)irs_iterator_get(sent_score_it);
    irs_list_push_back(sorted_sents_scores, pair);
  }

  while (irs_iterator_next(sent_score_it) && sent_score_it->current)
  {
    irs_pair * sent_score_index = (irs_pair *)irs_iterator_get(sent_score_it);
    irs_uint64 const sent_score = (irs_uint64)sent_score_index->first;

    irs_iterator * other_score_sent_it = irs_list_iterator(sorted_sents_scores);
    while (irs_iterator_next(other_score_sent_it))
    {
      irs_pair * other_sent_score_index = (irs_pair *)irs_iterator_get(other_score_sent_it);
      irs_uint64 const other_sent_score = (irs_uint64)other_sent_score_index->first;

      if (sent_score <= other_sent_score)
      {
        irs_list_push_front(sorted_sents_scores, other_score_sent_it->current, sent_score_index);
        break;
      }
      else
      {
        if (irs_iterator_next(sent_score_it) == IRS_FALSE)
        {
          irs_list_push_back(sorted_sents_scores, sent_score_index);
          break;
        }
        irs_iterator_prev(sent_score_it);
      }
    }
    irs_iterator_destroy(other_score_sent_it);
  }
  irs_iterator_destroy(sent_score_it);

  return sorted_sents_scores;
}

irs_list * _irs_get_sentences_scores(irs_char * document, irs_list * sentences)
{
  irs_list * sents_scores;
  irs_list_init(&sents_scores);

  irs_iterator * sent_it = irs_list_iterator(sentences);
  irs_uint64 i = 0;
  while (irs_iterator_next(sent_it))
  {
    irs_char * sentence = irs_iterator_get(sent_it);
    irs_uint64 const sent_score = _irs_define_sentence_score(document, sentence);

    irs_pair * sent_score_index;
    irs_pair_initialize(&sent_score_index, (void *)sent_score, (void *)i);

    irs_list_push_back(sents_scores, sent_score_index);
    ++i;
  }
  irs_iterator_destroy(sent_it);

  return sents_scores;
}

irs_list * _irs_get_document_summarization(irs_list * sentences, irs_list * sorted_sents_scores)
{
  irs_uint64 i = 0;
  static irs_uint64 const MAX_SENT_NUMBER = 10;

  irs_char ** summar = irs_mem_new(irs_char *, sorted_sents_scores->size);

  irs_iterator * sorted_score_sent_it = irs_list_reverse_iterator(sorted_sents_scores);
  while (irs_iterator_prev(sorted_score_sent_it) && i < MAX_SENT_NUMBER)
  {
    irs_pair * sent_score_index = (irs_pair *)irs_iterator_get(sorted_score_sent_it);
    irs_uint64 const index = (irs_uint64)sent_score_index->second;

    irs_uint64 j = 0;
    irs_iterator * sent_it = irs_list_iterator(sentences);
    while (irs_iterator_next(sent_it))
    {
      if (j == index - 1)
      {
        irs_char * sentence = irs_iterator_get(sent_it);
        irs_str_cpy(summar[j], sentence, irs_str_len(sentence));
        break;
      }
      ++j;
    }
    irs_iterator_destroy(sent_it);

    ++i;
  }
  irs_iterator_destroy(sorted_score_sent_it);

  irs_list * summar_sents;
  irs_list_init(&summar_sents);

  for (i = 0; i < sorted_sents_scores->size; ++i)
  {
    if (summar[i] != NULL_PTR)
    {
      irs_char * sentence = summar[i];
      irs_list_push_back(summar_sents, sentence);
    }
  }

  return summar_sents;
}

irs_list * irs_engine_get_documents_summarizations(irs_engine * engine, irs_list * documents)
{
  irs_list * summarizations;
  irs_list_init(&summarizations);

  irs_iterator * doc_it = irs_list_iterator(documents);
  while (irs_iterator_next(doc_it))
  {
    irs_char * document = irs_iterator_get(doc_it);

    irs_list * sentences = _irs_storage_get_document_sentences(document);
    irs_list * sents_scores = _irs_get_sentences_scores(document, sentences);

    irs_list * sorted_sents_scores = _irs_sort_doc_sentences_by_score(sents_scores);
    irs_list_destroy(sents_scores);

    irs_list * summar = _irs_get_document_summarization(sentences, sorted_sents_scores);
    irs_list_push_back(summarizations, summar);

    irs_list_clear(sorted_sents_scores);
    irs_list_destroy(sorted_sents_scores);

    irs_list_clear(sentences);
    irs_list_destroy(sentences);
  }
  irs_iterator_destroy(doc_it);

  return summarizations;
}
