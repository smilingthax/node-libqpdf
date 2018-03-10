#include <qpdf/RandomDataProvider.hh>

class OpenSSL_RandomDataProvider : public RandomDataProvider {
public:
  void provideRandomData(unsigned char *data, size_t len); // override
};

