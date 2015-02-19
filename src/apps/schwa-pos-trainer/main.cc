/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/io/logging.h>
#include <schwa/learn.h>
#include <schwa/tagger/pos.h>

namespace cf = ::schwa::config;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace io = ::schwa::io;
namespace ln = ::schwa::learn;
namespace pos = ::schwa::tagger::pos;


class Main : public cf::Main {
public:
  cs::Doc::Schema schema;

  cf::Op<std::string> input_path;
  cf::Op<std::string> model_path;
  cf::Op<std::string> extracted_path;
  cf::Op<bool> extract_only;
  cf::Op<bool> retain_docs;
  pos::ModelParams model_params;
  ln::CRFsuiteTrainerParams trainer_params;
  dr::DocrepGroup dr;

  Main(void) :
      cf::Main("schwa-pos-trainer", "Schwa Lab POS tag trainer. Linear CRF backed by crfsuite."),
      input_path(*this, "input", 'i', "The input path", io::STDIN_STRING),
      model_path(*this, "model", 'm', "The model path"),
      extracted_path(*this, "dump-extracted", "The path to dump the extracted features in crfsuite format", cf::Flags::OPTIONAL),
      extract_only(*this, "extract-only", "Whether to perform feature extraction only and no training", false),
      retain_docs(*this, "retain-docs", "Read the documents into memory instead of reading multiple times from disk (useful if input is a pipe)", false),
      model_params(*this, "model-params", "Parameters controlling the contents of the produced model"),
      trainer_params(*this, "train-params", "Parameters to the crfsuite training process"),
      dr(*this, schema)
    { }
  virtual ~Main(void) { }
};


namespace schwa {
namespace tagger {
namespace pos {

template <typename IT, typename TRANSFORMER>
static void
run_trainer(const Main &cfg, const IT docs_begin, const IT docs_end, TRANSFORMER &transformer, OutputModel &model) {
  // Create the feature extractor.
  Extractor extractor(model);

  // Create the trainer.
  ln::CRFsuiteTrainer<Extractor> trainer(extractor, model, cfg.trainer_params);

  // Extract the features.
  trainer.extract<IT, TRANSFORMER>(docs_begin, docs_end, transformer);

  // Optionally dump out the extracted features.
  if (cfg.extracted_path.was_mentioned()) {
    io::OutputStream out(cfg.extracted_path());
    trainer.dump_crfsuite_data(out);
  }

  if (cfg.extract_only())
    return;

  // Train the model.
  trainer.train();
}


template <typename TRANSFORMER>
static void
run_trainer(const Main &cfg, TRANSFORMER &transformer, OutputModel &model) {
  // Read in each of the docrep documents from the input stream.
  std::vector<cs::Doc *> docs;
  {
    io::InputStream in(cfg.input_path());
    dr::Reader reader(in, cfg.schema);
    while (true) {
      cs::Doc *doc = new cs::Doc();
      if (reader >> *doc) {
        docs.push_back(doc);
      }
      else {
        delete doc;
        break;
      }
    }
  }

  // Run the training process over the docs.
  run_trainer(cfg, docs.begin(), docs.end(), transformer, model);

  // Delete the read in docs.
  for (cs::Doc *doc : docs)
    delete doc;
  docs.clear();
}

}  // namespace pos
}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  Main cfg;

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the model. The validation of the model path needs to happen as early as possible.
    pos::OutputModel model(cfg.model_path(), cfg.model_params, cfg);

    // Create the feature transformer.
    if (cfg.model_params.feature_hashing.was_mentioned()) {
      ln::HasherTransform<> transformer(cfg.model_params.feature_hashing());
      pos::run_trainer(cfg, transformer, model);
    }
    else {
      ln::NoTransform transformer;
      pos::run_trainer(cfg, transformer, model);
    }
  })
  return 0;
}
