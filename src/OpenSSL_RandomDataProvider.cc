#include "OpenSSL_RandomDataProvider.hh"
#include <openssl/rand.h>
#include <stdexcept>

void OpenSSL_RandomDataProvider::provideRandomData(unsigned char *data, size_t len)
{
  if (!RAND_bytes(data, len)) {
    throw std::runtime_error("RAND_bytes failed");
  }
}

