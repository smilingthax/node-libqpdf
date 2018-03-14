#include "helper.hh"

void JsThrow(const std::exception &e)
{
  Nan::ThrowError(e.what());
}

void JsThrow()
{
  Nan::ThrowError("Uncaught C++ Exception from libqdf");
}

v8::Local<v8::String> ToString(const std::string &str)
{
  return Nan::New(str).ToLocalChecked();
}

v8::Local<v8::Object> ToBuffer(const std::string &str)
{
  return Nan::CopyBuffer(&str[0], str.size()).ToLocalChecked(); // str.data();
}

