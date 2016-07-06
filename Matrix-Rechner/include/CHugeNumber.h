#ifndef __CHUGENUMBER_H
#define __CHUGENUMBER_H

#include "StringFunctions.h"
#include "CBinaryStream.h"
#include <deque>
#include <string>
#include <iostream>

namespace Codebook {

class CFileSize;

class CHugeNumber {
private:
  typedef char digit_type;

  class CDigit {
  public:
    digit_type m_digit;
    CDigit(int w=0);
    CDigit(char w);

    bool operator+=(const CDigit &d);
    bool operator+=(int d);
    bool operator-=(const CDigit &d);
    bool operator-=(int d);
    bool operator==(const CDigit &d) const;
    bool operator!=(const CDigit &d) const;
    bool operator<(const CDigit &d) const;
    bool operator>(const CDigit &d) const;
    bool operator<=(const CDigit &d) const;
    bool operator>=(const CDigit &d) const;
  };

public:
  typedef std::deque<CDigit> number_type;

private:
  number_type m_number;
  short m_sign;

  static int compare(const number_type &v1, const number_type &v2);
  static number_type add(const number_type &v1, const number_type &v2, unsigned int p=0);
  static number_type sub(const number_type &v1, const number_type &v2);
  static number_type mul(const number_type &v1, const number_type &v2);
  static number_type div(const number_type &v1, const number_type &v2);
  static number_type mod(const number_type &v1, const number_type &v2);
  static void increase(number_type &v, int p=0);
  static void decrease(number_type &v, int p=0);
  static void shorten(number_type &v);
  static bool isZero(const CHugeNumber &n);
  CHugeNumber(const number_type &v, short sign=1);

public:
  CHugeNumber(const std::string &w="0");
  CHugeNumber(long w);
  CHugeNumber(unsigned long w);
  CHugeNumber(int w);
  CHugeNumber(unsigned long low, unsigned long high);

  bool operator<(const CHugeNumber &n) const;
  bool operator>(const CHugeNumber &n) const;
  bool operator<=(const CHugeNumber &n) const;
  bool operator>=(const CHugeNumber &n) const;
  bool operator==(const CHugeNumber &n) const;
  bool operator!=(const CHugeNumber &n) const;

  CHugeNumber &operator+=(const CHugeNumber &n);
  CHugeNumber &operator-=(const CHugeNumber &n);
  CHugeNumber &operator*=(const CHugeNumber &n);
  CHugeNumber &operator/=(const CHugeNumber &n);
  CHugeNumber &operator%=(const CHugeNumber &n);

  CHugeNumber &operator++();
  CHugeNumber &operator--();
  CHugeNumber operator++(int);
  CHugeNumber operator--(int);

  friend CHugeNumber operator+(const CHugeNumber &n1, const CHugeNumber &n2);
  friend CHugeNumber operator-(const CHugeNumber &n1, const CHugeNumber &n2);
  friend CHugeNumber operator*(const CHugeNumber &n1, const CHugeNumber &n2);
  friend CHugeNumber operator/(const CHugeNumber &n1, const CHugeNumber &n2);
  CHugeNumber operator%(const CHugeNumber &n) const;
  CHugeNumber operator<<(int w) const;
  CHugeNumber operator>>(int w) const;
  friend std::ostream &operator<<(std::ostream &ostr, const CHugeNumber &n);
  std::string getAsPointedNumber() const;
  std::string getAsString() const;
  CFileSize getAsFileSize() const;
  unsigned long getAsUnsignedLong() const;
  long getAsLong() const;

};

}; // namespace Codebook
#endif

