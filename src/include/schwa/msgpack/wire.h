/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace msgpack {

    enum class WireType : uint8_t {
      FIXNUM_POSITIVE, FIXNUM_NEGATIVE,
      MAP_FIXED, MAP_16, MAP_32,
      ARRAY_FIXED, ARRAY_16, ARRAY_32,
      RAW_FIXED, RAW_16, RAW_32,
      NIL,
      TRUE, FALSE,
      FLOAT, DOUBLE,
      UINT_8, UINT_16, UINT_32, UINT_64,
      INT_8, INT_16, INT_32, INT_64,
      RESERVED
    };

    inline std::ostream &operator <<(std::ostream &out, const WireType &t) { return out << static_cast<uint8_t>(t); }

    extern const WireType TABLE[256];


    namespace header {
      static const unsigned char MAP_FIXED   = 0x80;
      static const unsigned char ARRAY_FIXED = 0x90;
      static const unsigned char RAW_FIXED   = 0xa0;
      static const unsigned char NIL      = 0xc0;
      static const unsigned char FALSE    = 0xc2;
      static const unsigned char TRUE     = 0xc3;
      static const unsigned char FLOAT    = 0xca;
      static const unsigned char DOUBLE   = 0xcb;
      static const unsigned char UINT_8   = 0xcc;
      static const unsigned char UINT_16  = 0xcd;
      static const unsigned char UINT_32  = 0xce;
      static const unsigned char UINT_64  = 0xcf;
      static const unsigned char INT_8    = 0xd0;
      static const unsigned char INT_16   = 0xd1;
      static const unsigned char INT_32   = 0xd2;
      static const unsigned char INT_64   = 0xd3;
      static const unsigned char RAW_16   = 0xda;
      static const unsigned char RAW_32   = 0xdb;
      static const unsigned char ARRAY_16 = 0xdc;
      static const unsigned char ARRAY_32 = 0xdd;
      static const unsigned char MAP_16   = 0xde;
      static const unsigned char MAP_32   = 0xdf;
    }


    // ========================================================================
    // Reading API
    // ========================================================================
    inline WireType header_type(const int header);

    template <typename IN> inline void     read_nil(IN &in);
    template <typename IN> inline bool     read_bool(IN &in);
    template <typename IN> inline float    read_float(IN &in);
    template <typename IN> inline double   read_double(IN &in);
    template <typename IN> inline int64_t  read_int(IN &in);
    template <typename IN> inline uint64_t read_uint(IN &in);

    template <typename IN> inline size_t   read_array_size(IN &in);
    template <typename IN> inline size_t   read_map_size(IN &in);

    template <typename IN> inline std::string read_raw(IN &in);

    inline int8_t   read_val_int_fixed(const int h);
    template <typename IN> inline int8_t   read_val_int8(IN &in);
    template <typename IN> inline int16_t  read_val_int16(IN &in);
    template <typename IN> inline int32_t  read_val_int32(IN &in);
    template <typename IN> inline int64_t  read_val_int64(IN &in);

    inline uint8_t  read_val_uint_fixed(const int h);
    template <typename IN> inline uint8_t  read_val_uint8(IN &in);
    template <typename IN> inline uint16_t read_val_uint16(IN &in);
    template <typename IN> inline uint32_t read_val_uint32(IN &in);
    template <typename IN> inline uint64_t read_val_uint64(IN &in);

    template <typename IN> inline int8_t   read_int_fixed(IN &in);
    template <typename IN> inline int8_t   read_int8(IN &in);
    template <typename IN> inline int16_t  read_int16(IN &in);
    template <typename IN> inline int32_t  read_int32(IN &in);
    template <typename IN> inline int64_t  read_int64(IN &in);

    template <typename IN> inline uint8_t  read_uint_fixed(IN &in);
    template <typename IN> inline uint8_t  read_uint8(IN &in);
    template <typename IN> inline uint16_t read_uint16(IN &in);
    template <typename IN> inline uint32_t read_uint32(IN &in);
    template <typename IN> inline uint64_t read_uint64(IN &in);

    template <typename IN> inline void read(IN &in, int8_t &val) { val = read_int8(in); }
    template <typename IN> inline void read(IN &in, int16_t &val) { val = read_int16(in); }
    template <typename IN> inline void read(IN &in, int32_t &val) { val = read_int32(in); }
    template <typename IN> inline void read(IN &in, int64_t &val) { val = read_int64(in); }
    template <typename IN> inline void read(IN &in, uint8_t &val) { val = read_uint8(in); }
    template <typename IN> inline void read(IN &in, uint16_t &val) { val = read_uint16(in); }
    template <typename IN> inline void read(IN &in, uint32_t &val) { val = read_uint32(in); }
    template <typename IN> inline void read(IN &in, uint64_t &val) { val = read_uint64(in); }
    template <typename IN> inline void read(IN &in, float &val) { val = read_float(in); }
    template <typename IN> inline void read(IN &in, double &val) { val = read_double(in); }
    template <typename IN> inline void read(IN &in, bool &val) { val = read_bool(in); }
    template <typename IN> inline void read(IN &in, std::string &val) { val = read_raw(in); }

    template <typename IN, typename OUT>
    bool read_lazy(IN &in, OUT &out, WireType &type);

    // ========================================================================
    // Writing API
    // ========================================================================
    template <typename OUT> inline void write_nil(OUT &out);
    template <typename OUT> inline void write_bool(OUT &out, const bool x);
    template <typename OUT> inline void write_int(OUT &out, const int64_t x);
    template <typename OUT> inline void write_uint(OUT &out, const uint64_t x);
    template <typename OUT> inline void write_float(OUT &out, const float x);
    template <typename OUT> inline void write_double(OUT &out, const double x);

    template <typename OUT> inline void write_array_size(OUT &out, const size_t size);
    template <typename OUT> inline void write_map_size(OUT &out, const size_t size);

    template <typename OUT> inline void write_raw(OUT &out, const std::string &data);
    template <typename OUT> inline void write_raw(OUT &out, const char *const data, const size_t size);

    template <typename OUT> inline void write_int_fixed(OUT &out, const int8_t x);
    template <typename OUT> inline void write_int8(OUT &out, const int8_t x);
    template <typename OUT> inline void write_int16(OUT &out, const int16_t x);
    template <typename OUT> inline void write_int32(OUT &out, const int32_t x);
    template <typename OUT> inline void write_int64(OUT &out, const int64_t x);

    template <typename OUT> inline void write_uint_fixed(OUT &out, const uint8_t x);
    template <typename OUT> inline void write_uint8(OUT &out, const uint8_t x);
    template <typename OUT> inline void write_uint16(OUT &out, const uint16_t x);
    template <typename OUT> inline void write_uint32(OUT &out, const uint32_t x);
    template <typename OUT> inline void write_uint64(OUT &out, const uint64_t x);

    template <typename OUT> inline void write(OUT &out, const int8_t &val) { return write_int8(out, val); }
    template <typename OUT> inline void write(OUT &out, const int16_t &val) { return write_int16(out, val); }
    template <typename OUT> inline void write(OUT &out, const int32_t &val) { return write_int32(out, val); }
    template <typename OUT> inline void write(OUT &out, const int64_t &val) { return write_int64(out, val); }
    template <typename OUT> inline void write(OUT &out, const uint8_t &val) { return write_uint8(out, val); }
    template <typename OUT> inline void write(OUT &out, const uint16_t &val) { return write_uint16(out, val); }
    template <typename OUT> inline void write(OUT &out, const uint32_t &val) { return write_uint32(out, val); }
    template <typename OUT> inline void write(OUT &out, const uint64_t &val) { return write_uint64(out, val); }
    template <typename OUT> inline void write(OUT &out, const float &val) { return write_float(out, val); }
    template <typename OUT> inline void write(OUT &out, const double &val) { return write_double(out, val); }
    template <typename OUT> inline void write(OUT &out, const bool &val) { return write_bool(out, val); }
    template <typename OUT> inline void write(OUT &out, const std::string &val) { return write_raw(out, val); }


    // ========================================================================
    // Reading API implementations
    // ========================================================================
    template <typename IN, typename T>
    inline void
    read_bytes8(IN &in, T &x) {
      uint8_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 1);
      x = tmp;
    }

    template <typename IN, typename T>
    inline void
    read_bytes16(IN &in, T &x) {
      uint16_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 2);
      x = port::be16_to_h(tmp);
    }

    template <typename IN, typename T>
    inline void
    read_bytes32(IN &in, T &x) {
      uint32_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 4);
      x = port::be32_to_h(tmp);
    }

    template <typename IN, typename T>
    inline void
    read_bytes64(IN &in, T &x) {
      uint64_t tmp;
      in.read(reinterpret_cast<char *>(&tmp), 8);
      x = port::be64_to_h(tmp);
    }

    inline WireType
    header_type(const int header) {
      if (header < 0 || header > 255)
        return WireType::RESERVED;
      return TABLE[header & 0xFF];
    }

    template <typename IN>
    inline void
    read_nil(IN &in) {
      const int h = in.get();
      assert(h == header::NIL);
    }

    template <typename IN>
    inline bool
    read_bool(IN &in) {
      const int h = in.get();
      if (h == header::TRUE)
        return true;
      else if (h == header::FALSE)
        return false;
      assert((h == header::TRUE || h == header::FALSE));
      return false;
    }

    template <typename IN>
    inline float
    read_float(IN &in) {
      float x;
      const int h = in.get();
      assert(h == header::FLOAT);
      read_bytes32(in, x);
      return x;
    }

    template <typename IN>
    inline double
    read_double(IN &in) {
      double x;
      const int h = in.get();
      assert(h == header::DOUBLE);
      read_bytes64(in, x);
      return x;
    }

    inline int8_t
    read_val_int_fixed(const int h) {
      return static_cast<int8_t>(h & 0x1F);
    }

    template <typename IN>
    inline int8_t
    read_val_int8(IN &in) {
      int8_t x;
      read_bytes8(in, x);
      return x;
    }

    template <typename IN>
    inline int16_t
    read_val_int16(IN &in) {
      int16_t x;
      read_bytes16(in, x);
      return x;
    }

    template <typename IN>
    inline int32_t
    read_val_int32(IN &in) {
      int32_t x;
      read_bytes32(in, x);
      return x;
    }

    template <typename IN>
    inline int64_t
    read_val_int64(IN &in) {
      int64_t x;
      read_bytes64(in, x);
      return x;
    }

    template <typename IN>
    inline int8_t
    read_int_fixed(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::FIXNUM_NEGATIVE)
        throw ReadException("Failed to read FIXNUM_NEGATIVE", h);
      return read_val_int_fixed(h);
    }

    template <typename IN>
    inline int8_t
    read_int8(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_8)
        throw ReadException("Failed to read INT_8", h, header::INT_8);
      return read_val_int8(in);
    }

    template <typename IN>
    inline int16_t
    read_int16(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_16)
        throw ReadException("Failed to read INT_16", h, header::INT_16);
      return read_val_int16(in);
    }

    template <typename IN>
    inline int32_t
    read_int32(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_32)
        throw ReadException("Failed to read INT_32", h, header::INT_32);
      return read_val_int32(in);
    }

    template <typename IN>
    inline int64_t
    read_int64(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::INT_64)
        throw ReadException("Failed to read INT_64", h, header::INT_64);
      return read_val_int64(in);
    }

    template <typename IN>
    inline int64_t
    read_int(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      switch (type) {
      case WireType::FIXNUM_NEGATIVE: return read_val_int_fixed(h);
      case WireType::FIXNUM_POSITIVE: return read_val_uint_fixed(h);
      case WireType::INT_8: return read_val_int8(in);
      case WireType::INT_16: return read_val_int16(in);
      case WireType::INT_32: return read_val_int32(in);
      case WireType::INT_64: return read_val_int64(in);
      default:
        throw ReadException("Did not find an integer to read", h);
      }
    }

    inline uint8_t
    read_val_uint_fixed(const int h) {
      return static_cast<uint8_t>(h & 0x7F);
    }

    template <typename IN>
    inline uint8_t
    read_val_uint8(IN &in) {
      uint8_t x;
      read_bytes8(in, x);
      return x;
    }

    template <typename IN>
    inline uint16_t
    read_val_uint16(IN &in) {
      uint16_t x;
      read_bytes16(in, x);
      return x;
    }

    template <typename IN>
    inline uint32_t
    read_val_uint32(IN &in) {
      uint32_t x;
      read_bytes32(in, x);
      return x;
    }

    template <typename IN>
    inline uint64_t
    read_val_uint64(IN &in) {
      uint64_t x;
      read_bytes64(in, x);
      return x;
    }

    template <typename IN>
    inline uint8_t
    read_uint_fixed(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::FIXNUM_POSITIVE)
        throw ReadException("Failed to read FIXNUM_POSITIVE", h);
      return read_val_uint_fixed(h);
    }

    template <typename IN>
    inline uint8_t
    read_uint8(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_8)
        throw ReadException("Failed to read UINT_8", h, header::UINT_8);
      return read_val_uint8(in);
    }

    template <typename IN>
    inline uint16_t
    read_uint16(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_16)
        throw ReadException("Failed to read UINT_16", h, header::UINT_16);
      return read_val_uint16(in);
    }

    template <typename IN>
    inline uint32_t
    read_uint32(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_32)
        throw ReadException("Failed to read UINT_32", h, header::UINT_32);
      return read_val_uint32(in);
    }

    template <typename IN>
    inline uint64_t
    read_uint64(IN &in) {
      const int h = in.get();
      if (header_type(h) != WireType::UINT_64)
        throw ReadException("Failed to read UINT_64", h, header::UINT_64);
      return read_val_uint64(in);
    }

    template <typename IN>
    inline uint64_t
    read_uint(IN &in) {
      const int h = in.get();
      const WireType type = header_type(h);
      switch (type) {
      case WireType::FIXNUM_POSITIVE: return read_val_uint_fixed(h);
      case WireType::UINT_8: return read_val_uint8(in);
      case WireType::UINT_16: return read_val_uint16(in);
      case WireType::UINT_32: return read_val_uint32(in);
      case WireType::UINT_64: return read_val_uint64(in);
      default:
        throw ReadException("Did not find an unsigned integer to read", h);
      }
    }

    template <typename IN>
    inline size_t
    read_array_size(IN &in) {
      const int header = in.get();
      const WireType type = header_type(header);
      std::cout << "[read_array_size] " << std::hex << header << std::dec << std::endl;
      uint16_t s16;
      uint32_t s32;
      switch (type) {
      case WireType::ARRAY_FIXED:
        return header & 0x0F;
      case WireType::ARRAY_16:
        read_bytes16(in, s16);
        return s16;
      case WireType::ARRAY_32:
        read_bytes32(in, s32);
        return s32;
      default:
        assert(!"header is not an array");
        return 0;
      }
    }

    template <typename IN>
    inline size_t
    read_map_size(IN &in) {
      const int header = in.get();
      const WireType type = header_type(header);
      std::cout << "[read_map_size] " << std::hex << header << std::dec << std::endl;
      uint16_t s16;
      uint32_t s32;
      switch (type) {
      case WireType::MAP_FIXED:
        return header & 0x0F;
      case WireType::MAP_16:
        read_bytes16(in, s16);
        return s16;
      case WireType::MAP_32:
        read_bytes32(in, s32);
        return s32;
      default:
        assert(!"header is not a map");
        return 0;
      }
    }

    template <typename IN>
    inline std::string
    read_raw(IN &in) {
      const int header = in.get();
      const WireType type = header_type(header);
      std::cout << "[read_raw_size] " << std::hex << header << std::dec << std::endl;
      uint16_t s16;
      uint32_t s32;

      switch (type) {
      case WireType::RAW_FIXED:
        s32 = header & 0x1F;
        break;
      case WireType::RAW_16:
        read_bytes16(in, s16);
        s32 = s16;
        break;
      case WireType::RAW_32:
        read_bytes32(in, s32);
        break;
      default:
        assert(!"header is not a raw");
        return "";
      }

      std::string s;
      s.resize(s32);
      in.read(&s[0], s32);
      return s;
    }


    template <typename IN, typename OUT>
    bool
    read_lazy(IN &in, OUT &out, WireType &type) {
      char buf[8];
      uint16_t s16;
      uint32_t s32;
      bool recurse = false, raw = false;

      const int h = in.get();
      if (h == EOF)
        return false;
      out.put(h);

      type = header_type(h);
      switch (type) {
      case WireType::FIXNUM_POSITIVE:
      case WireType::FIXNUM_NEGATIVE:
      case WireType::NIL:
      case WireType::TRUE:
      case WireType::FALSE:
        break;
      case WireType::INT_8:
      case WireType::UINT_8:
        in.read(buf, 1);
        out.write(buf, 1);
        break;
      case WireType::INT_16:
      case WireType::UINT_16:
        in.read(buf, 2);
        out.write(buf, 2);
        break;
      case WireType::FLOAT:
      case WireType::INT_32:
      case WireType::UINT_32:
        in.read(buf, 4);
        out.write(buf, 4);
        break;
      case WireType::DOUBLE:
      case WireType::INT_64:
      case WireType::UINT_64:
        in.read(buf, 8);
        out.write(buf, 8);
        break;
      case WireType::ARRAY_FIXED:
        s32 = h & 0x0F;
        recurse = true;
        break;
      case WireType::ARRAY_16:
        read_bytes16(in, s16);
        s32 = s16;
        recurse = true;
        break;
      case WireType::ARRAY_32:
        read_bytes32(in, s32);
        recurse = true;
        break;
      case WireType::MAP_FIXED:
        s32 = h & 0x0F;
        s32 *= 2;
        recurse = true;
        break;
      case WireType::MAP_16:
        read_bytes16(in, s16);
        s32 = 2 * s16;
        recurse = true;
        break;
      case WireType::MAP_32:
        read_bytes32(in, s32);
        s32 *= 2;
        recurse = true;
        break;
      case WireType::RAW_FIXED:
        s32 = h & 0x1F;
        raw = true;
        break;
      case WireType::RAW_16:
        read_bytes16(in, s16);
        s32 = s16;
        raw = true;
        break;
      case WireType::RAW_32:
        read_bytes32(in, s32);
        raw = true;
        break;
      case WireType::RESERVED:
        break;
      }

      if (recurse) {
        WireType t;
        for (uint32_t i = 0; i != s32; ++i)
          if (!read_lazy(in, out, t))
            return false;
      }
      else if (raw) {
        for (uint32_t i = 0; i != s32; ++i) {
          int c = in.get();
          if (c == EOF)
            return false;
          out.put(c);
        }
      }

      return true;
    }


    // ========================================================================
    // Writing API implementations
    // ========================================================================
    template <typename OUT, typename T>
    inline void
    write_bytes_8(OUT &out, const T _x) {
      const unsigned char *x = reinterpret_cast<const unsigned char *>(&_x);
      out.put(x[0]);
    }

    template <typename OUT, typename T>
    inline void
    write_bytes_16(OUT &out, const T _x) {
      const uint16_t x = port::h_to_be16(static_cast<uint16_t>(_x));
      out.write(reinterpret_cast<const char *>(&x), 2);
    }

    template <typename OUT, typename T>
    inline void
    write_bytes_32(OUT &out, const T _x) {
      const uint32_t x = port::h_to_be32(static_cast<uint32_t>(_x));
      out.write(reinterpret_cast<const char *>(&x), 4);
    }

    template <typename OUT, typename T>
    inline void
    write_bytes_64(OUT &out, const T _x) {
      const uint64_t x = port::h_to_be64(static_cast<uint64_t>(_x));
      out.write(reinterpret_cast<const char *>(&x), 8);
    }

    template <typename OUT> inline void write_raw_uint8(OUT &out, const uint8_t x) { write_bytes_8<OUT, uint8_t>(out, x); }
    template <typename OUT> inline void write_raw_uint16(OUT &out, const uint16_t x) { write_bytes_16<OUT, uint16_t>(out, x); }
    template <typename OUT> inline void write_raw_uint32(OUT &out, const uint32_t x) { write_bytes_32<OUT, uint32_t>(out, x); }
    template <typename OUT> inline void write_raw_uint64(OUT &out, const uint64_t x) { write_bytes_64<OUT, uint64_t>(out, x); }

    template <typename OUT> inline void write_raw_int8(OUT &out, const int8_t x) { write_bytes_8<OUT, int8_t>(out, x); }
    template <typename OUT> inline void write_raw_int16(OUT &out, const int16_t x) { write_bytes_16<OUT, int16_t>(out, x); }
    template <typename OUT> inline void write_raw_int32(OUT &out, const int32_t x) { write_bytes_32<OUT, int32_t>(out, x); }
    template <typename OUT> inline void write_raw_int64(OUT &out, const int64_t x) { write_bytes_64<OUT, int64_t>(out, x); }

    template <typename OUT>
    inline void
    write_uint_fixed(OUT &out, const uint8_t x) {
      write_raw_uint8(out, x & 0x7F);
    }

    template <typename OUT>
    inline void
    write_uint8(OUT &out, const uint8_t x) {
      out.put(header::UINT_8);
      write_raw_uint8(out, x);
    }

    template <typename OUT>
    inline void
    write_uint16(OUT &out, const uint16_t x) {
      out.put(header::UINT_16);
      write_raw_uint16(out, x);
    }

    template <typename OUT>
    inline void
    write_uint32(OUT &out, const uint32_t x) {
      out.put(header::UINT_32);
      write_raw_uint32(out, x);
    }

    template <typename OUT>
    inline void
    write_uint64(OUT &out, const uint64_t x) {
      out.put(header::UINT_64);
      write_raw_uint64(out, x);
    }

    template <typename OUT>
    inline void
    write_int_fixed(OUT &out, const int8_t x) {
      write_raw_uint8(out, ~(*reinterpret_cast<const uint8_t *>(&x)) & 0xE0);
    }

    template <typename OUT>
    inline void
    write_int8(OUT &out, const int8_t x) {
      out.put(header::INT_8);
      write_raw_int8(out, x);
    }

    template <typename OUT>
    inline void
    write_int16(OUT &out, const int16_t x) {
      out.put(header::INT_16);
      write_raw_int16(out, x);
    }

    template <typename OUT>
    inline void
    write_int32(OUT &out, const int32_t x) {
      out.put(header::INT_32);
      write_raw_int32(out, x);
    }

    template <typename OUT>
    inline void
    write_int64(OUT &out, const int64_t x) {
      out.put(header::INT_64);
      write_raw_int64(out, x);
    }

    template <typename OUT>
    inline void
    write_array_size(OUT &out, const size_t size) {
      if (size <= 15)
        out.put(static_cast<unsigned char>(header::ARRAY_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::ARRAY_16);
        write_raw_uint16(out, size);
      }
      else {
        out.put(header::ARRAY_32);
        write_raw_uint32(out, size);
      }
    }

    template <typename OUT>
    inline void
    write_map_size(OUT &out, const size_t size) {
      if (size <= 15)
        out.put(static_cast<unsigned char>(header::MAP_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::MAP_16);
        write_raw_uint16(out, size);
      }
      else {
        out.put(header::MAP_32);
        write_raw_uint32(out, size);
      }
    }

    template <typename OUT>
    inline void
    write_nil(OUT &out) {
      out.put(header::NIL);
    }

    template <typename OUT>
    inline void
    write_bool(OUT &out, const bool x) {
      out.put(x ? header::TRUE : header::FALSE);
    }

    template <typename OUT>
    inline void
    write_uint(OUT &out, const uint64_t x) {
      if (x <= 127)
        write_uint_fixed(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint8_t>::max())
        write_uint8(out, static_cast<uint8_t>(x));
      else if (x <= std::numeric_limits<uint16_t>::max())
        write_uint16(out, static_cast<uint16_t>(x));
      else if (x <= std::numeric_limits<uint32_t>::max())
        write_uint32(out, static_cast<uint32_t>(x));
      else
        write_uint64(out, x);
    }

    template <typename OUT>
    inline void
    write_int(OUT &out, const int64_t x) {
      if (x >= -32 && x <= -1)
        write_int_fixed(out, static_cast<int8_t>(x));
      else if (x >= 0 && x <= 127)
        write_uint_fixed(out, static_cast<uint8_t>(x));
      else if (x >= std::numeric_limits<int8_t>::min() && x <= std::numeric_limits<int8_t>::max())
        write_int8(out, static_cast<int8_t>(x));
      else if (x >= std::numeric_limits<int16_t>::min() && x <= std::numeric_limits<int16_t>::max())
        write_int16(out, static_cast<int16_t>(x));
      else if (x >= std::numeric_limits<int32_t>::min() && x <= std::numeric_limits<int32_t>::max())
        write_int32(out, static_cast<int32_t>(x));
      else
        write_int64(out, x);
    }

    template <typename OUT>
    inline void
    write_float(OUT &out, const float x) {
      out.put(header::FLOAT);
      write_bytes_32(out, x);
    }

    template <typename OUT>
    inline void
    write_double(OUT &out, const double x) {
      out.put(header::DOUBLE);
      write_bytes_64(out, x);
    }

    template <typename OUT>
    inline void
    write_raw(OUT &out, const char *const data, const size_t size) {
      if (size <= 31)
        out.put(static_cast<unsigned char>(header::RAW_FIXED | size));
      else if (size <= std::numeric_limits<uint16_t>::max()) {
        out.put(header::RAW_16);
        write_raw_uint16(out, size);
      }
      else {
        out.put(header::RAW_32);
        write_raw_uint32(out, size);
      }
      io::Traits<OUT>::write_zerocopy(out, data, size);
    }

    template <typename OUT>
    inline void
    write_raw(OUT &out, const std::string &data) {
      write_raw(out, data.c_str(), data.size());
    }
  }
}
