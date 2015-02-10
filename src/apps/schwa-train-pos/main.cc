/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>

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
namespace tg = ::schwa::tagger;


namespace schwa {
namespace tagger {

template <typename TRANSFORMER>
static void
train_pos(io::InputStream &in, cs::Doc::Schema &schema, tg::POSOutputModel &model, TRANSFORMER &transformer, const ln::CRFSuiteTrainerParams &params, const bool retain_docs, const cf::Op<std::string> &extracted_path, const bool extract_only) {
  // Create the feature extractor.
  POSExtractor extractor(model);

  // Create the trainer.
  ln::CRFSuiteTrainer<POSExtractor> trainer(extractor, model, params);

  {
    // Construct a resettable docrep reader over the provided stream.
    ln::ResettableDocrepReader<cs::Doc> doc_reader(in, schema, retain_docs);

    // Extract the features.
    trainer.extract<TRANSFORMER>(doc_reader, transformer);
  }

  // Optionally dump out the extracted features.
  if (extracted_path.was_mentioned()) {
    io::OutputStream out(extracted_path());
    trainer.dump_crfsuite_data(out);
  }

  if (extract_only)
    return;

  // Train the model.
  trainer.train();
}

}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  cs::Doc::Schema schema;

  // Construct an option parser.
  cf::Main cfg("schwa-train-pos", "Schwa Lab POS tag trainer. Linear CRF backed by crfsuite.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> model_path(cfg, "model", 'm', "The model path");
  cf::Op<size_t> feature_hashing(cfg, "feature-hashing", 'H', "Number of bits to use for feature hashing", cf::Flags::OPTIONAL);
  cf::Op<std::string> extracted_path(cfg, "dump-extracted", "The path to dump the extracted features in crfsuite format", cf::Flags::OPTIONAL);
  cf::Op<bool> extract_only(cfg, "extract-only", "Whether to perform feature extraction only and no training", false);
  cf::Op<bool> retain_docs(cfg, "retain-docs", "Read the documents into memory instead of reading multiple times from disk (useful if input is a pipe)", false);
  tg::POSModelParams model_params(cfg, "model-params", "The model path path");
  ln::CRFSuiteTrainerParams trainer_params(cfg);
  dr::DocrepGroup dr(cfg, schema);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the input data and model paths.
    io::InputStream in(input_path());
    tg::POSOutputModel model(model_path(), model_params, cfg);

    // Create the feature extractor.
    if (feature_hashing.was_mentioned()) {
      ln::HasherTransform<> transformer(feature_hashing());
      tg::train_pos(in, schema, model, transformer, trainer_params, retain_docs(), extracted_path, extract_only());
    }
    else {
      ln::NoTransform transformer;
      tg::train_pos(in, schema, model, transformer, trainer_params, retain_docs(), extracted_path, extract_only());
    }
  })
  return 0;
}
