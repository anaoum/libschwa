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

namespace schwa {
namespace tagger {

template <typename TRANSFORMER>
static void
train_pos(io::InputStream &in, cs::Doc::Schema &schema, io::OutputStream &model, TRANSFORMER &transformer, const bool in_memory_docs) {
  // Create the feature extractor.
  POSExtractor extractor;

  // Create the trainer.
  learn::CRFSuiteTrainer<POSExtractor> trainer(extractor);

  {
    // Construct a resettable docrep reader over the provided stream.
    learn::ResettableDocrepReader<cs::Doc> doc_reader(in, schema, in_memory_docs);

    // Extract the features.
    trainer.extract<TRANSFORMER>(doc_reader, transformer);
  }

  // Train the model.
  trainer.train(model.path());
}

}  // namespace tagger
}  // namespace schwa


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  cs::Doc::Schema schema;

  // Construct an option parser.
  cf::Main cfg("schwa-train-pos", "Schwa Lab POS tag trainer. 1D linear CRF with LBFGS.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> model_path(cfg, "model", 'm', "The model path", io::STDOUT_STRING);
  cf::Op<size_t> feature_hashing(cfg, "feature-hashing", 'H', "Number of bits to use for feature hashing.", cf::Flags::OPTIONAL);
  cf::Op<bool> in_memory_docs(cfg, "in-memory", "Read the documents into memory instead of reading multiple times from disk (useful if input is a pipe).", false);
  dr::DocrepGroup dr(cfg, schema);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the input data and model paths.
    io::InputStream in(input_path());
    io::OutputStream model(model_path());

    // Create the feature extractor.
    if (feature_hashing.was_mentioned()) {
      schwa::learn::HasherTransform<> transformer(feature_hashing());
      schwa::tagger::train_pos(in, schema, model, transformer, in_memory_docs());
    }
    else {
      schwa::learn::NoTransform transformer;
      schwa::tagger::train_pos(in, schema, model, transformer, in_memory_docs());
    }
  })
  return 0;
}