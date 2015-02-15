/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/lex/word-embeddings.h>

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/msgpack.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace lex {

const std::string WordEmbeddings::UNKNOWN_WORD_KEY = "*UNKNOWN*";


WordEmbeddings::WordEmbeddings(StringPool &string_pool) :
    _string_pool(string_pool),
    _embeddings(nullptr),
    _unknown_embeddings(nullptr),
    _ndimensions(0)
  { }

WordEmbeddings::~WordEmbeddings(void) {
  delete [] _embeddings;
}


void
WordEmbeddings::load(std::istream &in) {
  if (SCHWA_UNLIKELY(_embeddings != nullptr))
    throw ValueException("WordEmbeddings instance is already initialised");

  // Keep track of the maximum absolute value for normalisation later on.
  float max_abs_embedding = std::numeric_limits<float>::lowest();
  float *embeddings = nullptr;

  // Read how many words are in the map.
  const uint32_t nwords = mp::read_map_size(in);

  // Read in each of the words.
  for (uint32_t i = 0; i != nwords; ++i) {
    // Read in the word.
    const std::string key = mp::read_str(in);

    // Read in the dimensions array size and sure it's valid.
    const uint32_t ndimensions = mp::read_array_size(in);
    if (i == 0) {
      // Allocate space for all of the embedding values in one giant array.
      _ndimensions = ndimensions;
      embeddings = _embeddings = new float[nwords * ndimensions];
    }
    else if (SCHWA_UNLIKELY(ndimensions != _ndimensions)) {
      std::ostringstream ss;
      ss << "Embeddings for '" << key << "' are dimensionality " << ndimensions << " when we expected " << _ndimensions;
      throw ValueException(ss.str());
    }

    // Insert the current canonicalised word into the map.
    const uint8_t *canonical_key = _string_pool.get(key);
    _map.emplace(canonical_key, embeddings);

    // If it's the special unknown key, keep track of it to save looking it up later.
    if (key == UNKNOWN_WORD_KEY)
      _unknown_embeddings = embeddings;

    // Read in each of the embedding values.
    for (uint32_t j = 0; j != ndimensions; ++j) {
      float f = mp::read_float(in);
      *embeddings++ = f;

      // Keep track of the maximum absolute embedding value.
      f = std::fabs(f);
      if (f > max_abs_embedding)
        max_abs_embedding = f;
    }
  }

  // Normalise all of the values.
  for (size_t i = 0; i != nwords * _ndimensions; ++i)
    _embeddings[i] /= max_abs_embedding;

  LOG(INFO) << "WordEmbeddings::load loaded " << nwords << " words with " << _ndimensions << " dimensions." << std::endl;
}


const float *
WordEmbeddings::get_embeddings(const std::string &word) const {
  const auto &it = _map.find(reinterpret_cast<const uint8_t *>(word.c_str()));
  return (it == _map.end()) ? _unknown_embeddings : it->second;
}

}  // namespace lex
}  // namespace schwa