#include "QPDFWriter.hh"
#include "QPDF.hh"
#include "helper.hh"

Nan::Persistent<v8::Function> QWr::constructor;
Nan::Persistent<v8::FunctionTemplate> QWr::tmpl;

NAN_MODULE_INIT(QWr::Init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(QWr::New);
  tpl->SetClassName(Nan::New("QPDFWriter").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tmpl.Reset(tpl);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("QPDFWriter").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

QWr *QWr::This(Nan::NAN_METHOD_ARGS_TYPE info)
{
  return Unwrap(info.Holder());
}

NAN_METHOD(QWr::New)
{
  if (info.IsConstructCall()) {
    if (info.Length()!=1) {
      Nan::ThrowTypeError("Wrong number of arguments");
      return;
    } else if (!QF::Is(info[0])) {
      Nan::ThrowTypeError("Expected QPDF as first argument");
      return;
    }

    QWr *qwr = new QWr(QF::Ref(info[0]));
    qwr->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons).ToLocalChecked());
  }
}

