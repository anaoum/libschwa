/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_WARC_H_
#define SCHWA_FORMATS_WARC_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/formats/html.h>


namespace schwa {
  namespace formats {

    class WARCLexer {
    public:
      static const size_t DEFAULT_BUFFER_SIZE;

    protected:
      uint8_t *_block_buffer;
      size_t _block_buffer_nbytes;
      size_t _block_nbytes_stated;
      size_t _block_nbytes_consumed;
      std::string _warc_type;
      std::string _current_key;
      //std::string _current_val;
      bool _in_key_content_length;
      bool _in_key_warc_type;

      bool _run(std::istream &input, size_t buffer_size=DEFAULT_BUFFER_SIZE);
      bool _run(const uint8_t *input, const size_t nbytes);

      virtual void _block_start(void);
      virtual void _block_consume(uint8_t c);
      virtual bool _block_consume_test(void);
      virtual void _block_end(void);

      virtual void _named_field_key_start(void);
      virtual void _named_field_key_consume(uint8_t);
      virtual void _named_field_key_end(void);
      virtual void _named_field_val_start(void);
      virtual void _named_field_val_consume(uint8_t);
      virtual void _named_field_val_end(void);

      virtual void _record_start(void);
      virtual void _record_end(void);

    public:
      WARCLexer(void);
      virtual ~WARCLexer(void);

      virtual bool run(std::istream &input, size_t buffer_size=DEFAULT_BUFFER_SIZE);
      virtual bool run(const uint8_t *input, const size_t nbytes);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(WARCLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_WARC_H_
