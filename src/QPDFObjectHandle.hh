#include <nan.h>
#include <qpdf/QPDFObjectHandle.hh>

#define QOH_BOOL_GETTER \
  X(isInitialized, {}) \
  X(isBool,        { obj.assertInitialized(); }) \
  X(isNull,        { obj.assertInitialized(); }) \
  X(isInteger,     { obj.assertInitialized(); }) \
  X(isReal,        { obj.assertInitialized(); }) \
  X(isName,        { obj.assertInitialized(); }) \
  X(isString,      { obj.assertInitialized(); }) \
  X(isOperator,    { obj.assertInitialized(); }) \
  X(isInlineImage, { obj.assertInitialized(); }) \
  X(isArray,       { obj.assertInitialized(); }) \
  X(isDictionary,  { obj.assertInitialized(); }) \
  X(isStream,      { obj.assertInitialized(); }) \
  X(isReserved,    { obj.assertInitialized(); }) \
  X(isIndirect,    {}) \
  X(isScalar,      { obj.assertInitialized(); })

class QOH : public Nan::ObjectWrap {
public:
  static inline QOH *Unwrap(v8::Local<v8::Object> obj) {
    return Nan::ObjectWrap::Unwrap<QOH>(obj);
  }

  static v8::Local<v8::Object> NewInstance(const QPDFObjectHandle &value);

  static NAN_MODULE_INIT(Init);

private:

  static QOH *This(Nan::NAN_METHOD_ARGS_TYPE info);

  static NAN_METHOD(New);

  static NAN_METHOD(parse);

#define X(name, code) static NAN_METHOD(name);
  QOH_BOOL_GETTER
#undef X

  static Nan::Persistent<v8::Function> constructor;

  QPDFObjectHandle obj;
};

