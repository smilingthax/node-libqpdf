#include "QPDFObjGen.hh"
#include "helper.hh"

Nan::Persistent<v8::Function> QOGen::constructor;
Nan::Persistent<v8::FunctionTemplate> QOGen::tmpl;

NAN_MODULE_INIT(QOGen::Init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(QOGen::New);
  tpl->SetClassName(Nan::New("QPDFObjGen").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tmpl.Reset(tpl);

#define X(name) Nan::SetPrototypeMethod(tpl, #name, name);
  QOGen_INT_GETTER
#undef X

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("QPDFObjGen").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> QOGen::NewInstance(const QPDFObjGen &value)
{
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> ret = Nan::NewInstance(cons).ToLocalChecked();
  Unwrap(ret)->og = value;
  return ret;
}

QOGen *QOGen::This(Nan::NAN_METHOD_ARGS_TYPE info)
{
  return Unwrap(info.Holder());
}

NAN_METHOD(QOGen::New)
{
  if (info.IsConstructCall()) {
    QOGen *qog = new QOGen;
    qog->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons).ToLocalChecked());
  }
}

#define X(name) \
  NAN_METHOD(QOGen::name)                           \
  {                                                 \
    if (info.Length() != 0) {                       \
      Nan::ThrowTypeError("Expected no arguments"); \
      return;                                       \
    }                                               \
    QPDFObjGen &og = This(info)->og;                \
    CATCHER({                                       \
      info.GetReturnValue().Set(og . name ());      \
    });                                             \
  }
QOGen_INT_GETTER
#undef X

