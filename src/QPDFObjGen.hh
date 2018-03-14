#include <nan.h>
#include <qpdf/QPDFObjGen.hh>

#define QOGen_INT_GETTER \
  X(getObj) \
  X(getGen)

class QOGen : public Nan::ObjectWrap {
public:
  static inline QOGen *Unwrap(v8::Local<v8::Object> obj) {
    return Nan::ObjectWrap::Unwrap<QOGen>(obj);
  }

  static inline bool Is(v8::Local<v8::Value> arg) {
    return Nan::New(tmpl)->HasInstance(arg);
  }

  static v8::Local<v8::Object> NewInstance(const QPDFObjGen &value);

  static NAN_MODULE_INIT(Init);

private:
  static QOGen *This(Nan::NAN_METHOD_ARGS_TYPE info);

  static NAN_METHOD(New);

#define X(name) static NAN_METHOD(name);
  QOGen_INT_GETTER
#undef X

  static Nan::Persistent<v8::Function> constructor;
  static Nan::Persistent<v8::FunctionTemplate> tmpl;

  QPDFObjGen og;
};

