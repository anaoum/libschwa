/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_NER_IMPL_H_
#define SCHWA_TAGGER_NER_IMPL_H_

#include <sstream>

#include <schwa/lex/word-form.h>
#include <schwa/unicode.h>


namespace schwa {
namespace tagger {
namespace ner {

// ========================================================================
// Extractor
// ========================================================================
template <typename TRANSFORM, typename VALUE>
inline void
Extractor::phase3_extract(canonical_schema::Sentence &sentence, canonical_schema::Token &token, learn::Features<TRANSFORM, VALUE> &features) {
  using ::schwa::third_party::re2::RE2;

  // Decode the UTF-8 sequence into to a Unicode string once only.
  const size_t i = &token - sentence.span.start;
  const std::string &utf8 = _get_token_ne_normalised(token);
  const UnicodeString u = UnicodeString::from_utf8(utf8);
  std::stringstream ss;

  // Prefix and suffix of length up to 4.
  learn::add_affix_features(features, 4, 4, utf8);

  // Word form.
  features("wf[i]=" + lex::word_form(u));

  // Contains a digit, hyphen, or uppercase code point.
  bool has_digit = false, has_hyphen = false, has_upper = false;
  for (const unicode_t cp : u) {
    if (!has_digit && unicode::is_digit(cp))
      has_digit = true;
    if (!has_hyphen && (unicode::is_hyphen(cp) || unicode::is_dash(cp)))
      has_hyphen = true;
    if (!has_upper && unicode::is_upper(cp))
      has_upper = true;
  }
  if (has_digit)
    features("has_digit");
  if (has_hyphen)
    features("has_hyphen");
  if (has_upper)
    features("has_upper");

  // Person-initial features to help match "Cricket: P. Smith and A. Fitz blah blah"
  //                                                 ^^^^^^^^     ^^^^^^^
  if (sentence.span.start + i != sentence.span.stop && RE2::FullMatch(utf8, RE_PERSON_INITIAL_1) && RE2::FullMatch(_get_token_ne_normalised(*(sentence.span.start + i + 1)), RE_PERSON_INITIAL_2))
    features("person_initial_1");
  if (i != 0 && RE2::FullMatch(utf8, RE_PERSON_INITIAL_2) && RE2::FullMatch(_get_token_ne_normalised(*(sentence.span.start + i - 1)), RE_PERSON_INITIAL_1))
    features("person_initial_2");

  // Does the token look like an acronym?
  if (RE2::FullMatch(utf8, RE_ACRONYM))
    features("acronym");

  // Is the current token a roman numeral looking thing which occurs after a possessive or capitalised word?
  if (i != 0 && RE2::FullMatch(utf8, RE_ROMAN_NUMERAL)) {
    const std::string &prev_ne_normalised = _get_token_ne_normalised(*(sentence.span.start + i - 1));
    if (RE2::FullMatch(prev_ne_normalised, RE_PERSON_INITIAL_2) || prev_ne_normalised == "'s")
      features("roman_numeral");
  }

  // Compute features that are in a +/-2 window of i.
  std::stringstream ss_ctx1, ss_ctx2;
  ss_ctx1 << "ctx[i]_1=";
  ss_ctx2 << "ctx[i]_2=";
  for (int offset = -2; offset != 3; ++offset) {
    const std::string &ne_normalised = _offsets_token_ne_normalised(i, offset);
    const bool is_sentinel = ne_normalised == learn::SENTINEL;

    // Add the NE-normalised token w_{i+j}.
    ss << ((offset < 0) ? "w[i" : "w[i+") << offset << "]=" << ne_normalised;
    features(ss.str());
    ss.str("");

    // Capitalisation patterns in a +/-2  and +/-1 window around w_{i}.
    if (!is_sentinel) {
      const std::string category = unicode::get_category_name(unicode::get_category(*UTF8Decoder(ne_normalised).cbegin()));
      ss_ctx2 << category;
      if (offset != -2 && offset != 2)
        ss_ctx1 << category;
    }
  }
  // Capitalisation patterns in a +/-2  and +/-1 window around w_{i}.
  features(ss_ctx1.str());
  features(ss_ctx2.str());

  // If the word contains a hyphen, add each of the components split by the hyphen.
  if (u.size() > 1) {
    UnicodeString part;
    part.reserve(u.size());
    unsigned int part_count = 0;
    for (size_t j = 0; j != u.size(); ++j) {
      const unicode_t cp = u[j];
      if (unicode::is_hyphen(cp)) {
        if (!part.empty()) {
          ss << "w[i]_part" << part_count << "=" << part.to_utf8();
          features(ss.str());
          ss.str("");
          ++part_count;
          part.clear();
        }
      }
      else
        part.push_back(cp);
    }
    if (part.size() != 0 && part.size() != u.size()) {
      ss << "w[i]_part" << part_count << "=" << part.to_utf8();
      features(ss.str());
      ss.str("");
    }
  }

  for (int offset = 0; offset != 1; ++offset) {
    const std::string &norm = _offsets_token_norm_raw(i, offset);
    if (norm == learn::SENTINEL)
      continue;

    if (_feature_flags.use_brown_cluster_features) {
      // Add Brown cluster paths for w_{i}.
      const size_t npaths = _brown_clusters.get_paths(norm, &_brown_cluster_path, _brown_cluster_path_lengths);
      if (npaths != 0) {
        const int prefix = std::sprintf(_brown_cluster_feature, "bc[i%+d]=", offset);
        std::memcpy(_brown_cluster_feature + prefix, _brown_cluster_path, _brown_cluster_path_lengths[npaths - 1]);
        for (size_t p = 0; p != npaths; ++p) {
          _brown_cluster_feature[prefix + _brown_cluster_path_lengths[npaths - 1 - p]] = '\0';
          features(_brown_cluster_feature);
        }
      }
    }

    if (_feature_flags.use_word_embeddings_features) {
      // Add word embedding values for w_{i}
      const double *embeddings = _word_embeddings.get_embeddings(norm);
      if (SCHWA_LIKELY(embeddings != nullptr)) {
        char buf[14];  // we[i+2]=ddddd
        const int prefix = std::sprintf(buf, "we[i%+d]=", offset);
        for (uint32_t j = 0; j != _word_embeddings.ndimensions(); ++j) {
          std::sprintf(buf + prefix, "%u", j);
          features(buf, embeddings[j]);
        }
      }
    }
  }

  // Extended prediction history (Ratinov & Roth, CoNLL 2009).
  if (_feature_flags.use_extended_prediction_history_features) {
    const auto &it = _token_label_counts.find(utf8);
    if (it != _token_label_counts.end()) {
      third_party::crfsuite::floatval_t total = 0;
      for (const auto &pair : it->second)
        total += std::get<1>(pair);
      for (const auto &pair : it->second) {
        ss << "eph=" << std::get<0>(pair);
        features(ss.str(), std::get<1>(pair)/total);
        ss.str("");
      }
    }
  }

  // Gazetteer match.
  if (_feature_flags.use_gazetteer_features) {
    static constexpr const char GAZ_PREFIXES[4] = {'W', 'B', 'E', 'M'};
    uint8_t gaz = _gazetteer_match[i];
    if (gaz != 0) {
      for (uint8_t i = 0; i != 4; ++i) {
        if ((gaz & 0x01) != 0) {
          ss << "gaz=" << GAZ_PREFIXES[i];
          features(ss.str());
          ss.str("");
        }
        gaz >>= 1;
      }
    }
  }

  // Context aggregation of the surrounding tokens.
  if (_feature_flags.use_context_aggregation_features) {
    const UnicodeString lower = u.to_lower();
    const auto &it = _token_context_aggregations.find(lower);
    if (it != _token_context_aggregations.end()) {
      for (const auto &pair : it->second) {
        if (pair.first == &token)
          continue;
        for (ptrdiff_t i = 0; i != 4; ++i) {
          ss << "ctxagg_w[i";
          if (i < 2)
            ss << (-2 + i);
          else
            ss << "+" << (i - 1);
          ss << "]=" << (pair.second[i] == nullptr ? learn::SENTINEL : _get_token_ne_normalised(*pair.second[i]));
          features.add_unique(ss.str());
          ss.str("");
        }
      }
    }
  }
}

}  // namespace ner
}  // namespace tagger
}  // namespace schwa

#endif  // SCHWA_TAGGER_NER_IMPL_H_
