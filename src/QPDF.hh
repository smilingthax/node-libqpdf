#include <nan.h>
#include <qpdf/QPDF.hh>

class QF : public Nan::ObjectWrap {
public:
  static inline QF *Unwrap(v8::Local<v8::Object> obj) {
    return Nan::ObjectWrap::Unwrap<QF>(obj);
  }

  static inline bool Is(v8::Local<v8::Value> arg) {
    return Nan::New(tmpl)->HasInstance(arg);
  }

  static inline QPDF &Ref(v8::Local<v8::Value> arg) {
    // only safe after Is(arg) ...
    return Unwrap(arg->ToObject())->pdf;
  }

  static NAN_MODULE_INIT(Init);

private:
  static QF *This(Nan::NAN_METHOD_ARGS_TYPE info);

  static NAN_METHOD(New);

  static Nan::Persistent<v8::Function> constructor;
  static Nan::Persistent<v8::FunctionTemplate> tmpl;

  QPDF pdf;
};

