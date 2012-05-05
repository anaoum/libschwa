# vim: set ts=2 et:
import cStringIO

import msgpack

from .constants import FIELD_TYPE_POINTER_TO
from .fields import Pointer, Range, Singleton
from .io import swizzle_ptr, types_from_doc
from .meta import Document

__all__ = ['Writer']


class Writer(object):
  __slots__ = ('_ostream', '_packer')

  def __init__(self, ostream):
    if not hasattr(ostream, 'write'):
        raise TypeError('"ostream" must have a write attr.')
    self._ostream = ostream
    self._packer  = msgpack.Packer()

  def write_doc(self, doc):
    if not isinstance(doc, Document):
      raise ValueError('You can only stream instances of docrep.Document')

    # find all of the types defined
    types = types_from_doc(doc)  # { klass : Type }

    # run along each of the Annotations and update the _dr_index attributes
    for name, annotations in doc._dr_annotations.iteritems():
      val = getattr(doc, name)
      if isinstance(annotations, Singleton):
        if val:
          val._dr_index = 0
      else:
        for i, obj in enumerate(val):
          obj._dr_index = i

    # construct the header
    header = [None] * len(types)
    for klass, t in types.iteritems():
      x = [t.name, t.nelem, []]
      for field in t.fields:
        f = field.copy()
        ptr_klass = f.get(FIELD_TYPE_POINTER_TO)
        if ptr_klass is not None:
          f[FIELD_TYPE_POINTER_TO] = types[ptr_klass].number
        x[2].append(f)
      header[t.number] = x
    self._pack(header)

    # write out each of the annotation sets
    for t in types.itervalues():
      tmp = cStringIO.StringIO()

      if t.is_meta:
        self._pack(self._serialize(doc, t), tmp)
      elif t.is_singleton:
        self._pack(self._serialize(getattr(doc, t.plural), t), tmp)
      else:
        msg_objs = []
        for obj in getattr(doc, t.plural):
          msg_objs.append(self._serialize(obj, t))
        self._pack(msg_objs, tmp)

      self._pack(t.number)
      self._pack(len(tmp.getvalue()))
      self._ostream.write(tmp.getvalue())

  def _pack(self, obj, out=None):
    if out is None:
      out = self._ostream
    out.write(self._packer.pack(obj))

  def _serialize(self, obj, t):
    msg_obj = {}
    for pyname, field in obj._dr_fields.iteritems():
      val = getattr(obj, pyname)
      if hasattr(field, 'to_wire'):
        val = field.to_wire(val)
      if val is None:
        continue
      if isinstance(field, Pointer):
        if field.is_collection:
          val = map(swizzle_ptr, val)
        else:
          val = swizzle_ptr(val)
      elif isinstance(field, Range):
        val = (val.start, val.stop)
      elif isinstance(val, unicode):
        val = val.encode('utf-8')
      msg_obj[t.pyname_to_index(pyname)] = val
    return msg_obj
