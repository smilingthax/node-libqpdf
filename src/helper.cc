#include "helper.hh"
#include <nan.h>

void JsThrow(const std::exception &e)
{
  Nan::ThrowError(e.what());
}

void JsThrow()
{
  Nan::ThrowError("Uncaught C++ Exception from libqdf");
}

