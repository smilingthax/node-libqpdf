#include "QPDFObjectHandle.hh"
#include "helper.hh"

Nan::Persistent<v8::Function> QOH::constructor;

NAN_MODULE_INIT(QOH::Init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(QOH::New);
  tpl->SetClassName(Nan::New("QPDFObjectHandle").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetMethod(tpl, "parse", parse);

#define X(name, code) Nan::SetPrototypeMethod(tpl, #name, name);
  QOH_BOOL_GETTER
#undef X

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("QPDFObjectHandle").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> QOH::NewInstance(const QPDFObjectHandle &value)
{
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> ret = Nan::NewInstance(cons).ToLocalChecked();
  Unwrap(ret)->obj = value;
  return ret;
}

QOH *QOH::This(Nan::NAN_METHOD_ARGS_TYPE info)
{
  return Unwrap(info.Holder());
}

NAN_METHOD(QOH::New)
{
  if (info.IsConstructCall()) {
    QOH *qoh = new QOH;
    qoh->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons).ToLocalChecked());
  }
}

//  parse(object_str, object_description="")
//  parse(PointerHolder<InputSource>, ...)  // TODO?
NAN_METHOD(QOH::parse)
{
  const int argc=info.Length();
  if (argc<1) {
    Nan::ThrowTypeError("At least one argument expected");
  } else if (argc<=2) {
    std::string obj_str, obj_desc;
    if (info[0]->IsString()) {
      Nan::Utf8String arg0(info[0]);  // ->ToString()  ?
      obj_str.assign(arg0.operator*(), arg0.length());
    } else {
      Nan::ThrowTypeError("Expected string as first argument");
      return;
    }
    if (!info[1]->IsUndefined()) {
      // TODO? type check - or just let it .toString ?
      Nan::Utf8String arg1(info[1]);
      if (arg1.length()) {
        obj_desc.assign(arg1.operator*(), arg1.length());
      }
    }

    CATCHER({
      QPDFObjectHandle ret = QPDFObjectHandle::parse(obj_str, obj_desc);
      info.GetReturnValue().Set(NewInstance(ret));
    });

  } else {
    Nan::ThrowTypeError("Expected only 2 arguments");
  }
}

#define X(name, code) \
  NAN_METHOD(QOH::name)                         \
  {                                             \
    QPDFObjectHandle &obj = This(info)->obj;    \
    CATCHER({                                   \
      code                                      \
      info.GetReturnValue().Set(obj . name ()); \
    });                                         \
  }
QOH_BOOL_GETTER
#undef X

