#include "QPDFObjectHandle.hh"
#include "QPDFObjGen.hh"
#include "helper.hh"

Nan::Persistent<v8::Function> QOH::constructor;
Nan::Persistent<v8::FunctionTemplate> QOH::tmpl;

NAN_MODULE_INIT(QOH::Init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(QOH::New);
  tpl->SetClassName(Nan::New("QPDFObjectHandle").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tmpl.Reset(tpl);

  Nan::SetMethod(tpl, "parse", parse);

#define X(name, code) Nan::SetPrototypeMethod(tpl, #name, name);
  QOH_BOOL_GETTER
  QOH_GETTER
  QOH_MUTATE_NOARG
#undef X

#define X(name) Nan::SetPrototypeMethod(tpl, #name, name);
  QOH_MANUAL
#undef X

#define X(name) Nan::SetMethod(tpl, #name, name);
  QOH_STATIC_NEW
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

// NOTE: may throw!
bool QOH::VecFromArray(std::vector<QPDFObjectHandle> &ret, v8::Local<v8::Value> arg) // {{{
{
  if (!arg->IsArray()) {
    return false;
  }

  v8::Local<v8::Array> arr = arg.As<v8::Array>(); // or: v8::Local<v8::Array>::Cast(arg);
  const uint32_t len = arr->Length();
  ret.reserve(len);

  for (uint32_t i=0; i<len; i++) {
    v8::Local<v8::Value> val = arr->Get(i);
    if (!Is(val)) {
      return false;
    }
    ret.push_back(Ref(val));
  }
  return true;
}
// }}}

v8::Local<v8::Array> QOH::VecToArray(const std::vector<QPDFObjectHandle> &vec) // {{{
{
  const size_t len = vec.size();
  v8::Local<v8::Array> ret = Nan::New<v8::Array>(len);
  for (size_t i=0; i<len; i++) {
    Nan::Set(ret, i, NewInstance(vec[i]));
  }
  return ret;
}
// }}}

v8::Local<v8::Map> QOH::StrMapToMap(const std::map<std::string,QPDFObjectHandle> &map) // {{{
{
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Map> ret = v8::Map::New(isolate);

  for (std::map<std::string,QPDFObjectHandle>::const_iterator it=map.begin(), end=map.end(); it!=end; ++it) {
    ret->Set(isolate->GetCurrentContext(), Nan::New(it->first).ToLocalChecked(), NewInstance(it->second));
  }
  return ret;
}
// }}}

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
      obj_str.assign(*arg0, arg0.length());
    } else {
      Nan::ThrowTypeError("Expected string as first argument");
      return;
    }
    if (!info[1]->IsUndefined()) {
      // TODO? type check - or just let it .toString ?
      Nan::Utf8String arg1(info[1]);
      if (arg1.length()) {
        obj_desc.assign(*arg1, arg1.length());
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
  NAN_METHOD(QOH::name)                             \
  {                                                 \
    if (info.Length() != 0) {                       \
      Nan::ThrowTypeError("Expected no arguments"); \
      return;                                       \
    }                                               \
    QPDFObjectHandle &obj = This(info)->obj;        \
    CATCHER({                                       \
      code                                          \
      info.GetReturnValue().Set(obj . name ());     \
    });                                             \
  }
QOH_BOOL_GETTER
#undef X

#define X(name, code) \
  NAN_METHOD(QOH::name)                             \
  {                                                 \
    if (info.Length() != 0) {                       \
      Nan::ThrowTypeError("Expected no arguments"); \
      return;                                       \
    }                                               \
    QPDFObjectHandle &obj = This(info)->obj;        \
    CATCHER({                                       \
      info.GetReturnValue().Set(code);              \
    });                                             \
  }
QOH_GETTER
#undef X

#define X(name, code) \
  NAN_METHOD(QOH::name)                             \
  {                                                 \
    if (info.Length() != 0) {                       \
      Nan::ThrowTypeError("Expected no arguments"); \
      return;                                       \
    }                                               \
    QPDFObjectHandle &obj = This(info)->obj;        \
    CATCHER({                                       \
      code                                          \
    });                                             \
  }
QOH_MUTATE_NOARG
#undef X

NAN_METHOD(QOH::getArrayItem)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsUint32()) {
    Nan::ThrowTypeError("Expected uint32 as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  const uint32_t idx = info[0]->Uint32Value();
  CATCHER({
    obj.assertInitialized();
    info.GetReturnValue().Set(NewInstance(obj.getArrayItem(idx)));
  });
}

NAN_METHOD(QOH::hasKey)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  Nan::Utf8String str(info[0]);
  CATCHER({
    obj.assertInitialized();
    info.GetReturnValue().Set(obj.hasKey(std::string(*str, str.length())));
  });
}

NAN_METHOD(QOH::getKey)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  Nan::Utf8String str(info[0]);
  CATCHER({
    obj.assertInitialized();
    info.GetReturnValue().Set(NewInstance(obj.getKey(std::string(*str, str.length()))));
  });
}

NAN_METHOD(QOH::getKeys)
{
  if (info.Length() != 0) {
    Nan::ThrowTypeError("Expected no arguments");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  CATCHER({
    obj.assertInitialized();
    const std::set<std::string> &set = obj.getKeys();

    // TODO?! nan?
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Set> ret = v8::Set::New(isolate);

    for (std::set<std::string>::const_iterator it=set.begin(), end=set.end(); it!=end; ++it) {
      ret->Add(isolate->GetCurrentContext(), Nan::New(*it).ToLocalChecked());
    }
    info.GetReturnValue().Set(ret);
  });
}

NAN_METHOD(QOH::isOrHasName)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  Nan::Utf8String str(info[0]);
  CATCHER({
    obj.assertInitialized();
    info.GetReturnValue().Set(obj.isOrHasName(std::string(*str, str.length())));
  });
}

NAN_METHOD(QOH::setArrayItem)
{
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsUint32()) {
    Nan::ThrowTypeError("Expected uint32 as first argument");
    return;
  } else if (!Is(info[1])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as second argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  const uint32_t idx = info[0]->Uint32Value();
  CATCHER({
    obj.assertInitialized();
    obj.setArrayItem(idx, Ref(info[1]));
  });
}

NAN_METHOD(QOH::setArrayFromVector)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  CATCHER({
    obj.assertInitialized();
    std::vector<QPDFObjectHandle> vec;

    if (VecFromArray(vec, info[0])) {
      obj.setArrayFromVector(vec);
    } else {
      Nan::ThrowTypeError("Expected Array of (only) QPDFObjectHandles");
    }
  });
}

NAN_METHOD(QOH::insertItem)
{
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsUint32()) {
    Nan::ThrowTypeError("Expected uint32 as first argument");
    return;
  } else if (!Is(info[1])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as second argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  const uint32_t idx = info[0]->Uint32Value();
  CATCHER({
    obj.assertInitialized();
    obj.insertItem(idx, Ref(info[1]));
  });
}

NAN_METHOD(QOH::appendItem)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!Is(info[0])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  CATCHER({
    obj.assertInitialized();
    obj.appendItem(Ref(info[0]));
  });
}

NAN_METHOD(QOH::eraseItem)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsUint32()) {
    Nan::ThrowTypeError("Expected uint32 as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  const uint32_t idx = info[0]->Uint32Value();
  CATCHER({
    obj.assertInitialized();
    obj.eraseItem(idx);
  });
}

NAN_METHOD(QOH::replaceKey)
{
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as first argument");
    return;
  } else if (!Is(info[1])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as second argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  Nan::Utf8String str(info[0]);
  CATCHER({
    obj.assertInitialized();
    obj.replaceKey(std::string(*str, str.length()), Ref(info[1]));
  });
}

NAN_METHOD(QOH::removeKey)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  Nan::Utf8String str(info[0]);
  CATCHER({
    obj.assertInitialized();
    obj.removeKey(std::string(*str, str.length()));
  });
}

NAN_METHOD(QOH::replaceOrRemoveKey)
{
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as first argument");
    return;
  } else if (!Is(info[1])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as second argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  Nan::Utf8String str(info[0]);
  CATCHER({
    obj.assertInitialized();
    obj.replaceOrRemoveKey(std::string(*str, str.length()), Ref(info[1]));
  });
}

NAN_METHOD(QOH::replaceDict)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!Is(info[0])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj,
                   &new_dict = Ref(info[0]);
  CATCHER({
    obj.assertInitialized();
    new_dict.assertInitialized(); // TODO? better error msg?
    obj.appendItem(new_dict);
  });
}

NAN_METHOD(QOH::addPageContents)
{
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!Is(info[0])) {
    Nan::ThrowTypeError("Expected QPDFObjectHandle as first argument");
    return;
  } else if (!info[1]->IsBoolean()) {
    Nan::ThrowTypeError("Expected Boolean as second argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj,
                   &new_contents = Ref(info[0]);
  const bool first = info[1]->BooleanValue();
  CATCHER({
    obj.assertInitialized();
    new_contents.assertInitialized(); // TODO? better error msg?
    obj.addPageContents(new_contents, first);
  });
}

NAN_METHOD(QOH::rotatePage)
{
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsInt32()) {
    Nan::ThrowTypeError("Expected int32 as first argument");
    return;
  } else if (!info[1]->IsBoolean()) {
    Nan::ThrowTypeError("Expected Boolean as second argument");
    return;
  }

  QPDFObjectHandle &obj = This(info)->obj;
  const int32_t angle = info[0]->Int32Value();
  const bool relative = info[1]->BooleanValue();
  CATCHER({
    obj.assertInitialized();
    obj.rotatePage(angle, relative);
  });
}

NAN_METHOD(QOH::newNull)
{
  if (info.Length() != 0) {
    Nan::ThrowTypeError("Expected no arguments");
    return;
  }

  CATCHER({
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newNull()));
  });
}

NAN_METHOD(QOH::newBool)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsBoolean()) {
    Nan::ThrowTypeError("Expected Boolean as argument");
    return;
  }

  const bool value = info[0]->BooleanValue();
  CATCHER({
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newBool(value)));
  });
}

NAN_METHOD(QOH::newInteger)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsNumber()) {
    Nan::ThrowTypeError("Expected Integer as argument");
    return;
  }

  const int64_t value = info[0]->IntegerValue();
  CATCHER({
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newInteger(value)));
  });
}

NAN_METHOD(QOH::newReal)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsNumber()) {
    Nan::ThrowTypeError("Expected Number as argument");
    return;
  }

  const double value = info[0]->NumberValue();
  CATCHER({
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newReal(value)));
  });
}

NAN_METHOD(QOH::newName)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  Nan::Utf8String str(info[0]);
  CATCHER({
    std::string value(*str, str.length());
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newName(value)));
  });
}

NAN_METHOD(QOH::newString)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  const Nan::Utf8String str(info[0]);
  CATCHER({
    std::string value(*str, str.length());
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newString(value)));
  });
}

NAN_METHOD(QOH::newOperator)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  const Nan::Utf8String str(info[0]);
  CATCHER({
    std::string value(*str, str.length());
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newOperator(value)));
  });
}

NAN_METHOD(QOH::newInlineImage)
{
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Expected String as argument");
    return;
  }

  const Nan::Utf8String str(info[0]);
  CATCHER({
    std::string value(*str, str.length());
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newInlineImage(value)));
  });
}

NAN_METHOD(QOH::newArray)
{
  if (info.Length() == 0) {
    CATCHER({
      info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newArray()));
    });
    return;
  } else if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  CATCHER({
    std::vector<QPDFObjectHandle> vec;
    if (VecFromArray(vec, info[0])) {
      info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newArray(vec)));
    } else {
      Nan::ThrowTypeError("Expected Array of (only) QPDFObjectHandles");
    }
  });
}

NAN_METHOD(QOH::newDictionary)
{
  if (info.Length() == 0) {
    CATCHER({
      info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newDictionary()));
    });
    return;
  } else if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  } else if (!info[0]->IsMap()) {
    Nan::ThrowTypeError("Expected Map as argument");
    return;
  }

  // TODO?! nan?   (- also cf. VecFromArray...)
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Array> arr = info[0].As<v8::Map>()->AsArray(); // or: v8::Local<v8::Map>::Cast(info[0])->AsArray();
  const uint32_t len = arr->Length();
#define COMMA ,   // required for c++ macros with <> and ,
  CATCHER({
    std::map<std::string COMMA QPDFObjectHandle> map;

    for (uint32_t i=1; i<len; i+=2) {
      v8::Local<v8::Value> key = arr->Get(i-1);
      v8::Local<v8::Value> val = arr->Get(i);
      if (!key->IsString() || !Is(val)) {
        Nan::ThrowTypeError("Expected Map of <String, QPDFObjectHandle>");
        return;
      }
      Nan::Utf8String keystr(key);
      map.emplace(std::string(*keystr, keystr.length()), Ref(val));
    }
    info.GetReturnValue().Set(NewInstance(QPDFObjectHandle::newDictionary(map)));
  });
}

