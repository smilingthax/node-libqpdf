#include <nan.h>

#include <qpdf/QUtil.hh>
#include "OpenSSL_RandomDataProvider.hh"

#include "QPDFObjectHandle.hh"

// void init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module)
static NAN_MODULE_INIT(init)
{
#ifndef _WIN32
  static OpenSSL_RandomDataProvider rdp;
  QUtil::setRandomDataProvider(&rdp);
#endif

  QOH::Init(target);

  // ... target
}

NODE_MODULE(libqpdf, init)

