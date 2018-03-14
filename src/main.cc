#include <nan.h>

#include <qpdf/QUtil.hh>
#include "OpenSSL_RandomDataProvider.hh"

#include "enums.hh"
#include "QPDFObjGen.hh"
#include "QPDFObjectHandle.hh"
#include "QPDF.hh"
#include "QPDFWriter.hh"

// void init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module)
static NAN_MODULE_INIT(init)
{
#ifndef _WIN32
  static OpenSSL_RandomDataProvider rdp;
  QUtil::setRandomDataProvider(&rdp);
#endif

  InitEnums(target);
  QOGen::Init(target);
  QOH::Init(target);
  QF::Init(target);
  QWr::Init(target);
}

NODE_MODULE(libqpdf, init)

