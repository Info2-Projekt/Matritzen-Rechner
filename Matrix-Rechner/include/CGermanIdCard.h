
#ifndef __CGERMANIDCARD_H
#define __CGERMANIDCARD_H

#include <string>
#include "CDate.h"

namespace Codebook {

class CGermanIdCard {
private:
  std::string m_id;
public:
  std::string getId() {
    return(m_id);
  }
  static bool checkChecksum(const std::string &s);
  CGermanIdCard(const std::string &id);
  CGermanIdCard(const std::string &serial,
                const std::string &birth,
                const std::string &expires,
                const std::string &check);

  bool isIdValid();
  CDate getAge();
  bool isOfAge();
  CDate getExpirationDate();
  bool isExpired();

};


}; // namespace Codebook

#endif
