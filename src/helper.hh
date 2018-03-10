#ifndef helper_hh_
#define helper_hh_

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

#endif
