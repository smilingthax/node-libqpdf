#include <nan.h>
#include <qpdf/QPDFWriter.hh>

#define QWr_INT_GETTER \
  X(getObj) \
  X(getGen)

class QWr : public Nan::ObjectWrap {
public:
  static inline QWr *Unwrap(v8::Local<v8::Object> obj) {
    return Nan::ObjectWrap::Unwrap<QWr>(obj);
  }

  static inline bool Is(v8::Local<v8::Value> arg) {
    return Nan::New(tmpl)->HasInstance(arg);
  }

  static NAN_MODULE_INIT(Init);

private:
  QWr(QPDF &pdf) : writer(pdf) {}

  static QWr *This(Nan::NAN_METHOD_ARGS_TYPE info);

  static NAN_METHOD(New);

  static Nan::Persistent<v8::Function> constructor;
  static Nan::Persistent<v8::FunctionTemplate> tmpl;

  QPDFWriter writer;
};

