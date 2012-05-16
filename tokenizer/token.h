/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa { namespace token {

typedef int64_t offset_type;

enum Type {
  WORD,
  NUMBER,
  UNIT,
  ABBREVIATION,
  CONTRACTION,
  POSSESSIVE,
  PUNCTUATION,
  QUOTE,
  DASH,
  TERMINATOR,
};

} }
