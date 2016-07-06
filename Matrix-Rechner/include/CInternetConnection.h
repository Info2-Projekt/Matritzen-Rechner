
#ifndef __CINTERNETCONNECTION_H
#define __CINTERNETCONNECTION_H

#include "CSpecificINetConn.h"
#include <string>

namespace Codebook {


class CInternetConnection : public CSpecificINetConn {
public:
  static const char EOL;
  static const char CR;
  static const std::string CRLF;

  CInternetConnection(const std::string &host, unsigned int port);

  std::string receiveLine();
  int sendLine(const std::string &line);
  int sendLineCRLF(const std::string &line);
};

}; // namespace Codebook

#endif
