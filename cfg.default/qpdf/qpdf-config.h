
#if defined(_MSC_VER) && _MSC_VER < 1700
  #undef HAVE_STDINT_H
  #undef HAVE_INTTYPES_H
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int32 uint32_t;
#else
  #define HAVE_STDINT_H 1
  #define HAVE_INTTYPES_H 1
#endif

// NOTE: already set by node-gyp via -D ...
//#define _FILE_OFFSET_BITS 64

#ifndef _WIN32
  // we'll use node's openssl.
  #define SKIP_OS_SECURE_RANDOM 1
#endif

