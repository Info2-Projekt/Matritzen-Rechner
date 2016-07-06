#include "CTimevector.h"
#include "CMoment.h"

namespace Codebook {

using namespace std;

CTimevector::CTimevector()
: m_days(0), m_mseconds(0)
{}

CTimevector::CTimevector(const CDate &d)
: m_days(d.getDaysPast()), m_mseconds(0)
{}

CTimevector::CTimevector(const CTime &z)
: m_days(0), m_mseconds(z.getMillisecondsPast())
{}

CTimevector::CTimevector(const CMoment &z)
: m_days(z.getDaysPast()), m_mseconds(z.getMillisecondsPast())
{}

CTimevector::CTimevector(const CDate &d, const CTime &z)
: m_days(d.getDaysPast()), m_mseconds(z.getMillisecondsPast())
{}

CTimevector::CTimevector(long d, long z)
: m_days(d), m_mseconds(z)
{}

CTimevector CTimevector::operator+(const CTimevector &z) const{
  CTimevector tmp(m_days+z.m_days, m_mseconds+z.m_mseconds);
  if(tmp.m_mseconds>=24*60*60*1000) {
    tmp.m_mseconds%=24*60*60*1000;
    tmp.m_days++;
  }
  return(tmp);
}

CTimevector CTimevector::operator-(const CTimevector &z) const{
  CTimevector tmp;

  if(z<*this) {
    tmp.m_days=m_days-z.m_days;
    tmp.m_mseconds=m_mseconds-z.m_mseconds;
    if(tmp.m_mseconds<0) {
      tmp.m_mseconds+=24*60*60*1000;
      tmp.m_days--;
    }
  } else {
    tmp.m_days=z.m_days-m_days;
    tmp.m_mseconds=z.m_mseconds-m_mseconds;
    if(tmp.m_mseconds<0) {
      tmp.m_mseconds+=24*60*60*1000;
      tmp.m_days--;
    }
  }
  return(tmp);
}

bool CTimevector::operator<(const CTimevector &z) const {
  if(m_days<z.m_days)
    return(true);
  if((m_days==z.m_days)&&(m_mseconds<z.m_mseconds))
    return(true);
  return(false);
}
bool CTimevector::operator==(const CTimevector &z) const {
  return((m_days==z.m_days)&&(m_mseconds==z.m_mseconds));
}


}; // namespace Codebook
