
#ifndef __CGERMANPASSPORT_H
#define __CGERMANPASSPORT_H

#include <string>
#include "CDate.h"

namespace Codebook {

class CGermanPassport {
private:
  std::string m_id;
public:
  std::string getId() {
    return(m_id);
  }
  CGermanPassport(const std::string &id);
  CGermanPassport(const std::string &serial,
                  const std::string &birth,
                  const std::string &sex,
                  const std::string &expires,
                  const std::string &check);

  bool isIdValid();
  CDate getAge();
  bool isOfAge();
  CDate getExpirationDate();
  bool isExpired();
  bool isMale();
  bool isFemale();

};


}; // namespace Codebook

#endif
