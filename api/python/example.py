#!/usr/bin/env python
import docrep

class Sentence(docrep.Annotation):
  my_field = docrep.Range('Token', sname='span')
  #token_begin = docrep.Pointer('Token', sname='begin')
  #token_end   = docrep.Pointer('Token', sname='end')


#class Token(docrep.Token):
  #pos  = docrep.Field(sname='gold_pos')


class Document(docrep.Document):
  tokens    = docrep.Annotations('Token')
  sentences = docrep.Annotations('Sentence')

  #def token_range(self, start, finish):
    #for i in xrange(start._dr_index, finish._dr_index + 1):
      #yield self.tokens[i]


with open('eng.testa.docrep', 'rb') as f:
  reader = docrep.Reader(Document)
  for doc in reader.stream(f):
    print type(doc), doc.filename, len(doc.tokens), len(doc.sentences)
    print [t.raw for t in doc.tokens[doc.sentences[0].span]]
    #for sent in doc.sentences:
      #print list(doc.token_range(sent.token_begin, sent.token_end))
    #print

