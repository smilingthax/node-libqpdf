#include "enums.hh"
#include <qpdf/QPDFObject.hh>
#include <qpdf/Constants.h>

// these are special: they live on QPDFObject::
#define QOtype \
  X(ot_uninitialized) \
  X(ot_reserved)      \
  X(ot_null)          \
  X(ot_boolean)       \
  X(ot_integer)       \
  X(ot_real)          \
  X(ot_string)        \
  X(ot_name)          \
  X(ot_array)         \
  X(ot_dictionary)    \
  X(ot_stream)        \
  X(ot_operator)      \
  X(ot_inlineimage)

#define Q_error_code \
  X(qpdf_e_success)     \
  X(qpdf_e_internal)    \
  X(qpdf_e_system)      \
  X(qpdf_e_unsupported) \
  X(qpdf_e_password)    \
  X(qpdf_e_damaged_pdf) \
  X(qpdf_e_pages)

#define Q_object_stream \
  X(qpdf_o_disable)  \
  X(qpdf_o_preserve) \
  X(qpdf_o_generate)

#define Q_stream_data \
  X(qpdf_s_uncompress) \
  X(qpdf_s_preserve)   \
  X(qpdf_s_compress)

// Q_stream_encode_flags
//   qpdf_ef_compress, qpdf_ef_normalize
// Q_stream_decode_level
//   qpdf_dl_none, qpdf_dl_generalized, qpdf_dl_specialized, qpdf_dl_all

#define Q_r3_print \
  X(qpdf_r3p_full) \
  X(qpdf_r3p_low) \
  X(qpdf_r3p_none)

#define Q_r3_modify \
  X(qpdf_r3m_all)      \
  X(qpdf_r3m_annotate) \
  X(qpdf_r3m_form)     \
  X(qpdf_r3m_assembly) \
  X(qpdf_r3m_none)

NAN_MODULE_INIT(InitEnums)
{
#define SetConst(obj, name, val) \
  Nan::Set(obj, Nan::New(#name).ToLocalChecked(), Nan::New<v8::Integer>(val));

  v8::Local<v8::Object> qo = Nan::New<v8::Object>();
  Nan::Set(target, Nan::New("QPDFObject").ToLocalChecked(), qo);
#define X(name) SetConst(qo, name, QPDFObject:: name)
  QOtype
#undef X

#define X(name) SetConst(target, name, name)
  Q_error_code
  Q_object_stream
  Q_stream_data
//  Q_stream_encode_flags
//  Q_stream_decode_level
  Q_r3_print
  Q_r3_modify
#undef X

}

