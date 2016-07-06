#ifndef __CTIMEVECTOR_H
#define __CTIMEVECTOR_H

#include <string>

namespace Codebook {

class CDate;
class CTime;
class CMoment;

class CTimevector {
private:
  long m_days;
  long m_mseconds;

public:
  CTimevector();
  CTimevector(const CDate &d);
  CTimevector(const CTime &z);
  CTimevector(const CMoment &z);
  CTimevector(const CDate &d, const CTime &z);
  CTimevector(long d, long z);

  long getDays() const {
    return(m_days);
  }

  long getMilliseconds() const {
    return(m_mseconds);
  }

  CTimevector operator+(const CTimevector &z) const;
  CTimevector operator-(const CTimevector &z) const;

  bool operator<(const CTimevector &z) const;
  bool operator==(const CTimevector &z) const;
};



}; // namespace Codebook
#endif
