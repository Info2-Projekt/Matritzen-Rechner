
#ifndef __CCHARCIPREDICATE_H
#define __CCHARCIPREDICATE_H

#include<functional>
#include<cctype>

namespace Codebook {

class CCharCIPredicate : public std::unary_function<char, bool> {
private:
  char m_char;
public:
  CCharCIPredicate(char c)
    : m_char(tolower(c))
  {}
  bool operator()(char o) {
    return(m_char==tolower(o));
  }
};

class CCharCIBinPredicate : public std::binary_function<char, char, bool> {
public:
  bool operator()(char o1, char o2) {
    return(tolower(o1)==tolower(o2));
  }
};

}; // namespace Codebook

#endif
