
#ifdef WITH_LIBJPEG
#  include "../../qpdf/include/qpdf/Pl_DCT.hh"
#else
// Replace real decoder with pass-through to not require libjpeg

#include <qpdf/Pipeline.hh>

class Pl_DCT : public Pipeline {
public:
  Pl_DCT(const char *identifier, Pipeline *next)
    : Pipeline(identifier, next)
  { }

  void write(unsigned char *buf, size_t len) {
    getNext()->write(buf, len);
  }

  void finish() {
    getNext()->finish();
  }
};

#endif

