/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_NANC_H_
#define SCHWA_CORPORA_NANC_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/corpora/importer.h>


namespace schwa {
  namespace corpora {

    class NANCImporter : public Importer {
    private:
      class Impl;
      Impl *_impl;

    public:
      explicit NANCImporter(const std::string &path);
      virtual ~NANCImporter(void);

      virtual canonical_schema::Doc *import(void) override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(NANCImporter);
    };

  }  // namespace corpora
}  // namespace schwa

#endif  // SCHWA_CORPORA_NANC_H_