#include "QPDF.hh"
#include "helper.hh"

Nan::Persistent<v8::Function> QF::constructor;
Nan::Persistent<v8::FunctionTemplate> QF::tmpl;

NAN_MODULE_INIT(QF::Init)
{
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(QF::New);
  tpl->SetClassName(Nan::New("QPDF").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tmpl.Reset(tpl);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("QPDF").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

QF *QF::This(Nan::NAN_METHOD_ARGS_TYPE info)
{
  return Unwrap(info.Holder());
}

NAN_METHOD(QF::New)
{
  if (info.IsConstructCall()) {
    QF *qf = new QF;
    qf->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(Nan::NewInstance(cons).ToLocalChecked());
  }
}

