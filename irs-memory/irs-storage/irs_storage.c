#include "irs_storage.h"

#include "math.h"

#include "irs-base/irs_allocator.h"
#include "irs-container/irs-string/irs_string.h"
#include "irs-container/irs-pair/irs_pair.h"

#include "irs-file-system/irs_file_system.h"

void _irs_init_db_path(irs_char const * path, irs_char const * postfix, irs_char ** out_path)
{
  irs_uint32 size = irs_str_len(path) + irs_str_len(postfix);
  *out_path = irs_mem_new(irs_char, size + 1);
  irs_str_printf(*out_path, size, "%s/%s", path, postfix);
}

irs_storage_status irs_storage_initialize(irs_storage ** storage, irs_char const * path)
{
  if (path == NULL_PTR)
    return IRS_STORAGE_WRONG_PATH;

  if (irs_fs_isdir(path) == FALSE)
  {
    if (irs_fs_mkdirs(path) == FALSE)
      return IRS_STORAGE_WRONG_PATH;
  }

  *storage = irs_mem_new(irs_storage, 1);
  {
    irs_str_cpy((*storage)->path, path, irs_str_len(path));

    static irs_char const * terms_postfix = "terms.irsdb";
    _irs_init_db_path(path, terms_postfix, &(*storage)->terms_path);
    (*storage)->terms_size = 0;

    static irs_char const * documents_postfix = "strings.irsdb";
    _irs_init_db_path(path, documents_postfix, &(*storage)->documents_path);
    (*storage)->documents_size = 0;
  }

  return IRS_STORAGE_OK;
}

irs_storage_status irs_storage_shutdown(irs_storage * storage)
{
  if (storage == NULL_PTR)
    return IRS_STORAGE_NO;

  {
    irs_mem_free(storage->path);
    irs_mem_free(storage->terms_path);
    irs_mem_free(storage->documents_path);
  }
  irs_mem_free(storage);

  return IRS_STORAGE_OK;
}

irs_storage_status _irs_storage_write_documents(
    irs_storage * storage, irs_list const * documents, irs_list ** document_offset_map)
{
  if (irs_list_init(document_offset_map) == FALSE)
    return IRS_STORAGE_WRITE_ERROR;

  irs_io_channel * channel = irs_io_new_channel(storage->documents_path, "w", NULL_PTR);
  irs_io_channel_seek(channel, storage->documents_size, IRS_IO_SEEK_SET, NULL_PTR);

  irs_iterator * it = irs_list_iterator((irs_list *)documents);
  while (irs_iterator_next(it))
  {
    irs_char * document = irs_iterator_get(it);
    irs_uint64 size = irs_str_len(document);
    irs_mem_cpy(document, document, size);

    // save document offset
    {
      irs_pair * pair;
      irs_pair_initialize(&pair, document, &size);
      irs_list_push_back(*document_offset_map, pair);
    }

    // save document in db
    {
      irs_uint64 written_bytes = 0;
      if (irs_io_channel_write_chars(channel, (irs_char *)size, sizeof(size), &written_bytes, NULL_PTR)
        != IRS_IO_STATUS_NORMAL || sizeof(size) != written_bytes)
      {
        irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
        return IRS_STORAGE_WRITE_ERROR;
      }

      if (irs_io_channel_write_chars(channel, document, size, &written_bytes, NULL_PTR) != IRS_IO_STATUS_NORMAL
        || size != written_bytes)
      {
        irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
        return IRS_STORAGE_WRITE_ERROR;
      }
    }

    storage->documents_size += size;
  }
  irs_iterator_destroy(it);
  irs_io_channel_shutdown(channel, TRUE, NULL_PTR);

  return IRS_STORAGE_OK;
}

irs_list * _irs_storage_get_document_terms(irs_char const * document)
{
  irs_char delim[] = " ";
  irs_char * term = strtok((irs_char *)document, delim);

  irs_list * terms;
  irs_list_init(&terms);
  while (term != NULL_PTR)
  {
    irs_list_push_back(terms, term);
    term = strtok(NULL, delim);
  }

  return terms;
}

inline irs_uint8 _irs_dictionary_children_size()
{
  irs_uint8 const max_irs_char = 255;
  irs_uint8 const min_irs_char = 1;

  return max_irs_char - min_irs_char + 1;
}

void _irs_char_to_irs_int(irs_char ch, irs_uint8 * ch_num, irs_uint8 * mask)
{
  *ch_num = 128 + (irs_uint8)ch;
}

void _irs_storage_get_documents_with_term(
    irs_char * term, irs_list * document_offset_map, irs_list ** doc_with_term_offsets)
{
  irs_list_init(doc_with_term_offsets);

  irs_iterator * doc_it = irs_list_iterator(document_offset_map);
  irs_uint64 documents_with_term = 0;
  while (irs_iterator_next(doc_it))
  {
    irs_pair * pair = irs_iterator_get(doc_it);
    irs_char * document_to_find = pair->first;

    // if document doesn't contain term then not save it offset
    if (irs_str_find(document_to_find, term) == FALSE)
      continue;

    irs_uint64 * doc_offset = pair->second;
    irs_list_push_back(*doc_with_term_offsets, doc_offset);

    ++documents_with_term;
  }
}

irs_float _irs_storage_calculate_term_reverse_frequency(
    irs_uint64 const documents_size, irs_uint64 doc_with_term_offsets_size)
{
  irs_float const reverse_frequency = logf(documents_size / doc_with_term_offsets_size);
  return reverse_frequency;
}

irs_float _irs_storage_calculate_term_frequency(irs_char * term, irs_list * document_terms)
{
  irs_uint64 count_in_document = 0;

  // calculate term repetitions
  irs_iterator * it = irs_list_iterator(document_terms);
  while (irs_iterator_next(it))
  {
    irs_char * other_term = irs_iterator_get(it);
    if (irs_str_cmp(term, other_term) == TRUE)
      ++count_in_document;
  }

  irs_float const frequency = count_in_document / document_terms->size;
  return frequency;
}

irs_float _irs_storage_calculate_term_significancy(irs_float const frequency, irs_float const reverse_frequency)
{
  irs_float const significancy = frequency * reverse_frequency;
  return significancy;
}

irs_dictionary * _irs_storage_assign_terms_in_documents(irs_storage * storage, irs_list * document_offset_map)
{
  // dictionary of terms to dictionaries of documents offset to term significancies in this documents
  irs_dictionary * terms_reverse_frequency_dictionary;
  irs_dictionary_initialize(&terms_reverse_frequency_dictionary, _irs_dictionary_children_size(), _irs_char_to_irs_int);

  // dictionary of terms to significancy in documents
  irs_dictionary * terms_doc_significancy_dictionary;
  irs_dictionary_initialize(&terms_doc_significancy_dictionary, _irs_dictionary_children_size(), _irs_char_to_irs_int);

  irs_iterator * doc_it = irs_list_iterator((irs_list *)document_offset_map);
  while (irs_iterator_next(doc_it))
  {
    irs_pair * pair = irs_iterator_get(doc_it);
    irs_char * document = pair->first;
    irs_uint64 doc_offset = *(irs_uint64 *)pair->second;
    irs_char * doc_offset_str;
    irs_int_to_str_int(doc_offset, doc_offset_str);

    irs_list * document_terms = _irs_storage_get_document_terms(document);

    irs_iterator * term_it = irs_list_iterator(document_terms);
    while (irs_iterator_next(term_it))
    {
      irs_char * term = irs_iterator_get(term_it);

      // check if current term significancy in current document was calculated;
      {
        irs_list * list_with_doc_dict = irs_dictionary_get(terms_doc_significancy_dictionary, term);
        if (list_with_doc_dict != NULL_PTR)
        {
          irs_iterator * list_with_doc_dict_it = irs_list_iterator(list_with_doc_dict);
          irs_iterator_next(list_with_doc_dict_it);
          irs_dictionary * doc_offset_dict = irs_iterator_get(list_with_doc_dict_it);
          irs_iterator_destroy(list_with_doc_dict_it);

          // current term in this document is assigned?
          if (irs_dictionary_get(doc_offset_dict, doc_offset_str) != NULL_PTR)
            continue;
        }
      }

      // calculate or get cached current term reverse frequency for all document
      irs_float reverse_frequency;
      irs_float doc_with_term_offsets_size;
      {
        irs_list * list_with_r_frequency = irs_dictionary_get(terms_reverse_frequency_dictionary, term);
        if (list_with_r_frequency == NULL_PTR)
        {
          irs_list * doc_with_term_offsets;
          _irs_storage_get_documents_with_term(term, document_offset_map, &doc_with_term_offsets);
          doc_with_term_offsets_size = doc_with_term_offsets->size;

          reverse_frequency
            = _irs_storage_calculate_term_reverse_frequency(document_offset_map->size, doc_with_term_offsets_size);
          irs_list_destroy(doc_with_term_offsets);

          irs_dictionary_append(terms_reverse_frequency_dictionary, term, strlen(term), &reverse_frequency);
        }
        else
        {
          irs_iterator * list_with_r_frequency_it = irs_list_iterator(list_with_r_frequency);
          irs_iterator_next(list_with_r_frequency_it);
          reverse_frequency = *(irs_float *)irs_iterator_get(list_with_r_frequency_it);
          irs_iterator_destroy(list_with_r_frequency_it);
        }
      }

      // get dictionary of documents offset to term significancies in this documents
      irs_dictionary * doc_offset_dictionary;
      {
        irs_list * list_with_doc_dict = irs_dictionary_get(terms_doc_significancy_dictionary, term);
        if (list_with_doc_dict == NULL_PTR)
        {
          irs_dictionary_initialize(&doc_offset_dictionary, _irs_dictionary_children_size(), _irs_char_to_irs_int);
          irs_uint64 const size = strlen(term);
          irs_dictionary_append(terms_doc_significancy_dictionary, term, size, doc_offset_dictionary);
          irs_dictionary_append(terms_doc_significancy_dictionary, term, size, term);
          irs_dictionary_append(terms_doc_significancy_dictionary, term, size, &doc_with_term_offsets_size);
        }
        else
        {
          irs_iterator * list_with_doc_dict_it = irs_list_iterator(list_with_doc_dict);
          irs_iterator_next(list_with_doc_dict_it);
          doc_offset_dictionary = irs_iterator_get(list_with_doc_dict_it);
          irs_iterator_destroy(list_with_doc_dict_it);
        }
      }

      // calculate term frequency in document and save it in dictionary
      {
        irs_float const frequency = _irs_storage_calculate_term_frequency(term, document_terms);
        irs_float const significancy = _irs_storage_calculate_term_significancy(frequency, reverse_frequency);
        irs_uint64 const size = strlen(doc_offset_str);
        irs_dictionary_append(doc_offset_dictionary, doc_offset_str, size, (void *)&doc_offset);
        irs_dictionary_append(doc_offset_dictionary, doc_offset_str, size, (void *)&significancy);
      }
    }
    irs_iterator_destroy(term_it);

    irs_list_destroy(document_terms);
    irs_mem_free(doc_offset_str);
  }
  irs_iterator_destroy(doc_it);

  irs_dictionary_destroy(terms_reverse_frequency_dictionary);

  return terms_doc_significancy_dictionary;
}

void _irs_storage_pass_terms_doc_offset_dictionary(irs_dictionary_node * node, void ** arguments)
{
  if (node->data_list == NULL_PTR)
    return;

  irs_iterator * list_it = irs_list_iterator(node->data_list);
  irs_iterator_next(list_it);
  irs_uint64 const doc_offset = *(irs_uint64 *)irs_iterator_get(list_it);

  irs_iterator_next(list_it);
  irs_float const significancy = *(irs_float *)irs_iterator_get(list_it);
  irs_iterator_destroy(list_it);

  irs_storage * storage = arguments[0];
  irs_io_channel * channel = arguments[1];

  irs_uint64 written_bytes = 0;
  if (irs_io_channel_write_chars(channel, (irs_char *)doc_offset, sizeof(doc_offset), &written_bytes, NULL_PTR)
    != IRS_IO_STATUS_NORMAL || sizeof(doc_offset) != written_bytes)
  {
    irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
    return;
  }

  storage->terms_size += written_bytes;

  if (irs_io_channel_write_chars(channel, (irs_char *)0, significancy, &written_bytes, NULL_PTR)
    != IRS_IO_STATUS_NORMAL || sizeof(significancy) != written_bytes)
  {
    irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
    return;
  }

  storage->terms_size += written_bytes;
}

void _irs_storage_pass_terms_doc_significancy_dictionary(irs_dictionary_node * node, void ** arguments)
{
  if (node->data_list == NULL_PTR)
    return;

  irs_iterator * list_it = irs_list_iterator(node->data_list);
  irs_iterator_next(list_it);
  irs_dictionary * doc_offset_dictionary = irs_iterator_get(list_it);

  irs_iterator_next(list_it);
  irs_char * const term = irs_iterator_get(list_it);

  irs_iterator_next(list_it);
  irs_uint64 const docs_with_term = *(irs_uint64 *)irs_iterator_get(list_it);
  irs_iterator_destroy(list_it);

  irs_storage * storage = arguments[0];
  irs_io_channel * channel = arguments[1];

  irs_uint64 const term_size = irs_str_len(term);
  irs_uint64 written_bytes = 0;
  if (irs_io_channel_write_chars(channel, (irs_char *)term_size, sizeof(term_size), &written_bytes, NULL_PTR)
      != IRS_IO_STATUS_NORMAL || sizeof(term_size) != written_bytes)
  {
    irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
    return;
  }

  storage->terms_size += written_bytes;

  if (irs_io_channel_write_chars(channel, (irs_char *)term, term_size, &written_bytes, NULL_PTR)
      != IRS_IO_STATUS_NORMAL || term_size != written_bytes)
  {
    irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
    return;
  }

  storage->terms_size += written_bytes;

  if (irs_io_channel_write_chars(channel, (irs_char *)docs_with_term, sizeof(docs_with_term), &written_bytes, NULL_PTR)
      != IRS_IO_STATUS_NORMAL || sizeof(docs_with_term) != written_bytes)
  {
    irs_io_channel_shutdown(channel, TRUE, NULL_PTR);
    return;
  }

  storage->terms_size += written_bytes;

  irs_dictionary_visit_down_nodes(doc_offset_dictionary, _irs_storage_pass_terms_doc_offset_dictionary, arguments);
}

irs_storage_status _irs_storage_write_infix_terms_significancies_in_documents(
    irs_storage * storage, irs_dictionary * terms_doc_significancy_dictionary)
{
  if (terms_doc_significancy_dictionary == NULL_PTR)
    return IRS_STORAGE_WRITE_ERROR;

  irs_io_channel * channel = irs_io_new_channel(storage->terms_path, "w", NULL_PTR);
  irs_io_channel_seek(channel, storage->terms_size, IRS_IO_SEEK_SET, NULL_PTR);

  void * arguments[2];
  arguments[0] = storage;
  arguments[1] = channel;
  irs_dictionary_visit_down_nodes(
      terms_doc_significancy_dictionary, _irs_storage_pass_terms_doc_significancy_dictionary, arguments);

  irs_io_channel_shutdown(channel, TRUE, NULL_PTR);

  return IRS_STORAGE_OK;
}

irs_storage_status irs_storage_add_documents(irs_storage * storage, irs_list const * documents)
{
  if (storage == NULL_PTR)
    return IRS_STORAGE_NO;

  irs_list * document_offset_map;
  if (_irs_storage_write_documents(storage, documents, &document_offset_map) != IRS_STORAGE_OK)
    return IRS_STORAGE_WRITE_ERROR;

  irs_dictionary * terms_doc_significancy_dictionary
    = _irs_storage_assign_terms_in_documents(storage, document_offset_map);

  irs_list_clear(document_offset_map);
  irs_list_destroy(document_offset_map);


}

irs_storage_status irs_storage_get(irs_storage * storage, irs_list const * terms, irs_list ** documents)
{

}