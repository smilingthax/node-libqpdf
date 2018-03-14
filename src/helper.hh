#ifndef helper_hh_
#define helper_hh_

#include <nan.h>
#include <string>

#include <stdexcept>

void JsThrow(const std::exception &e);
void JsThrow();

#define CATCHER(code)                 \
  try {                               \
    code                              \
  } catch (const std::exception &e) { \
    JsThrow(e);                       \
  } catch (...) {                     \
    JsThrow();                        \
  }

v8::Local<v8::String> ToString(const std::string &str);
v8::Local<v8::Object> ToBuffer(const std::string &str);

#endif
