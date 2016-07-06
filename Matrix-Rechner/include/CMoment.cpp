#include"CMoment.h"
#include "CTimevector.h"
#include "StringFunctions.h"
#include <ctime>

namespace Codebook {

using namespace std;

CMoment::CMoment()
  : m_date(), m_time() {
}

CMoment::CMoment(const CDate &d)
  : m_date(d), m_time(0,0,0,0) {
}

CMoment::CMoment(const CDate &d, const CTime &z)
  : m_date(d), m_time(z) {
}

CMoment::CMoment(int year, int month, int day, int hour, int minute, int second, int msecond)
: m_date(day, month, year), m_time(hour, minute, second, msecond) {
}

CMoment::CMoment(const CTimevector &z)
: m_date(z.getDays()), m_time(z.getMilliseconds()) {
}

CMoment::CMoment(const std::string &date, const std::string &time)
: m_date(date), m_time(time) {
}

CMoment::CMoment(const std::string &moment) {
  vector<string> c=chopIntoWords(moment, " .:-");
  if(c.size()!=8)
    throw invalid_argument("CMoment::CMoment(string)");
  m_date=CDate(c[1]+" "+c[2]+" "+c[3]);
  m_time=CTime(c[4]+":"+c[5]+":"+c[6]);
}

string CMoment::getDDMMYYYYHHMMSS() const {
  return(m_date.getDDMMYYYY()+" "+m_time.getHHMMSS());
}

string CMoment::getRFC1123Date() const {
  static const char* const months[]={"Jan","Feb", "Mar", "Apr", "May", "Jun", 
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  static const char* const days[]={"Mon","Tue", "Wed", "Thu", "Fri", "Sat", 
                                     "Sun"};

  stringstream str;
  str.fill('0');

  str << days[m_date.getWeekday()] << ", ";
  str << setw(2) << m_date.getDay() << " ";
  str << months[m_date.getMonth()-1] << " ";
  str << setw(4) << m_date.getYear() << " ";
  str << setw(2) << m_time.getHour() << ":";
  str << setw(2) << m_time.getMinute() << ":";
  str << setw(2) << m_time.getSecond() << " GMT";
  return(str.str());
}



CMoment CMoment::convertGmtToMet() const {
  time_t t;
  time(&t);
  tm *ts;
  ts=localtime(&t);
/*
** Wenn Sommerzeit, dann zwei Stunden,
** andernfalls eine Stunde addieren.
*/
  if(ts->tm_isdst)
    return(*this+CTimevector(0,7200000));
  else
    return(*this+CTimevector(0,3600000));
}

//*********

CMoment CMoment::convertMetToGmt() const {
  time_t t;
  time(&t);
  tm *ts;
  ts=localtime(&t);
/*
** Wenn Sommerzeit, dann zwei Stunden,
** andernfalls eine Stunde abziehen.
*/
  if(ts->tm_isdst)
    return(*this-CTimevector(0,7200000));
  else
    return(*this-CTimevector(0,3600000));
}


CMoment CMoment::operator+(const CTimevector &z) const {
  return(CTimevector(*this)+z);
}

CMoment CMoment::operator-(const CTimevector &z) const {
  return(CTimevector(*this)-z);
}

bool CMoment::operator<(const CMoment &z) const {
  return(CTimevector(*this)<z);
}
bool CMoment::operator==(const CMoment &z) const {
  return(CTimevector(*this)==z);
}

bool CMoment::isEqual(const CMoment &z, long eps) const {
  return((m_date==z.m_date)&&(m_time.isEqual(z.m_time,eps)));
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CMoment &m) {
  os << m.m_date;
  os << m.m_time;
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CMoment &m) {
  is >> m.m_date;
  is >> m.m_time;
  return(is);
}

}; // namespace Codebook
