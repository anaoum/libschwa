/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    template <typename R, typename T, R T::*field_ptr>
    FieldDef<R T::*, field_ptr>::FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, false, false, FieldTraits<R>::is_slice)
      {
        schema.add(this);
    }

    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDef<R T::*, field_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &, Doc &) const {
      T &ann = static_cast<T &>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val);
    }


    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDef<R T::*, field_ptr>::read_field(io::ArrayReader &in, Doc &_doc) const {
      T &doc = static_cast<T &>(_doc);
      R &val = doc.*field_ptr;
      wire::WireTraits<R>::read(in, val);
    }


    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDef<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &, const Doc &) const {
      const T &ann = static_cast<const T &>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val);
        return true;
      }
      return false;
    }


    // ========================================================================
    // FieldDefWithStore (DR_POINTER)
    // ========================================================================
    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, true, false, FieldTraits<R>::is_slice),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    const TypeInfo &
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    ptrdiff_t
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const D *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &, Doc &_doc) const {
      D &doc = static_cast<D &>(_doc);
      T &ann = static_cast<T &>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, doc.*store_ptr);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Doc &_doc) const {
      D &doc = static_cast<D &>(_doc);
      T &ann = static_cast<T &>(_doc);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, doc.*store_ptr);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &, const Doc &_doc) const {
      const T &ann = static_cast<const T &>(_ann);
      const D &doc = static_cast<const D &>(_doc);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, doc.*store_ptr);
        return true;
      }
      return false;
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, true, false, FieldTraits<R>::is_slice),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    const TypeInfo &
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    ptrdiff_t
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const D *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &, Doc &_doc) const {
      D &doc = static_cast<D &>(_doc);
      T &ann = static_cast<T &>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, doc.*store_ptr);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &, const Doc &_doc) const {
      const T &ann = static_cast<const T &>(_ann);
      const D &doc = static_cast<const D &>(_doc);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, doc.*store_ptr);
        return true;
      }
      return false;
    }

    // ========================================================================
    // FieldDefWithSelfStore (DR_SELF)
    // ========================================================================
    template <typename R, typename T, R T::*field_ptr>
    FieldDefWithSelfStore<R T::*, field_ptr>::FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, false, true, FieldTraits<R>::is_slice),
      _pointer_type(TypeInfo::create<R>())
      {
      schema.add(this);
    }


    template <typename R, typename T, R T::*field_ptr>
    const TypeInfo &
    FieldDefWithSelfStore<R T::*, field_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDefWithSelfStore<R T::*, field_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &store, Doc &) const {
      T &ann = static_cast<T &>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, store);
    }


    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDefWithSelfStore<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &store, const Doc &) const {
      const T &ann = static_cast<const T &>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, store);
        return true;
      }
      return false;
    }


    // ========================================================================
    // StoreDef (DR_STORE)
    // ========================================================================
    template <typename S, typename T, Store<S> T::*store_ptr>
    StoreDef<Store<S> T::*, store_ptr>::StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseStoreDef(name, help, mode, serial),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    const TypeInfo &
    StoreDef<Store<S> T::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    ptrdiff_t
    StoreDef<Store<S> T::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const T *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    IStore &
    StoreDef<Store<S> T::*, store_ptr>::istore(const Doc &_doc) const {
      const T &doc = static_cast<const T &>(_doc);
      const Store<S> &store = doc.*store_ptr;
      return store;
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    inline void
    StoreDef<Store<S> T::*, store_ptr>::resize(Doc &doc, const size_t size) const {
      (static_cast<T &>(doc).*store_ptr).resize(size);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    StoreDef<BlockStore<S> T::*, store_ptr>::StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseStoreDef(name, help, mode, serial),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    const TypeInfo &
    StoreDef<BlockStore<S> T::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    ptrdiff_t
    StoreDef<BlockStore<S> T::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const T *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    IStore &
    StoreDef<BlockStore<S> T::*, store_ptr>::istore(const Doc &_doc) const {
      const BlockStore<S> &store = static_cast<const T &>(_doc).*store_ptr;
      return store;
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    inline void
    StoreDef<BlockStore<S> T::*, store_ptr>::resize(Doc &doc, const size_t size) const {
      BlockStore<S> &store = static_cast<T &>(doc).*store_ptr;
      assert(store.size() == 0);
      store.reserve(size);
    }
  }
}
