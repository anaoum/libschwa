/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class Document;
    class TypeRegistry;


    class Writer {
    protected:
      std::ostream &_out;
      const TypeRegistry &_reg;

    public:
      Writer(std::ostream &out, const TypeRegistry &reg) : _out(out), _reg(reg) { }
      ~Writer(void) { }

      void write(const Document &doc);

      inline Writer &
      operator <<(const Document &doc) {
        write(doc);
        return *this;
      }
    };

  }
}