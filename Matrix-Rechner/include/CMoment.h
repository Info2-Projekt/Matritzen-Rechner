#ifndef __CMOMENT_H
#define __CMOMENT_H

#include <string>
#include "CDate.h"
#include "CTime.h"
#include "CBinaryStream.h"

namespace Codebook {

class CTimevector;

class CMoment {
private:
  CDate m_date;
  CTime m_time;

public:
/*
** Konstruktoren
*/
  CMoment();
  CMoment(const std::string &moment);
  CMoment(const std::string &date, const std::string &time);
  CMoment(const CTimevector &z);
  CMoment(const CDate &d);
  CMoment(const CDate &d, const CTime &z);
  CMoment(int year,
          int month,
          int day,
          int hour=0,
          int minute=0,
          int second=0,
          int msecond=0);

/*
** Zeit und Datum per Methode
*/
  CDate getDate() const {
    return(m_date);
  }

  CTime getTime() const {
    return(m_time);
  }

  void setDate(const CDate &d) {
    m_date=d;
  }

  void setTime(const CTime &z) {
    m_time=z;
  }
/*
** Lesen und Setzten der einzelnen Attribute
*/
  int getHour() const {
    return(m_time.getHour());
  }

  int getMinute() const {
    return(m_time.getMinute());
  }

  int getSecond() const {
    return(m_time.getSecond());
  }

  int getMillisecond() const {
    return(m_time.getMillisecond());
  }

  int getMillisecondsPast() const {
    return(m_time.getMillisecondsPast());
  }

  void setHour(long w) {
    m_time.setHour(w);
  }

  void setMinute(long w) {
    m_time.setMinute(w);
  }

  void setSecond(long w) {
    m_time.setSecond(w);
  }

  void setMillisecond(long w) {
    m_time.setMillisecond(w);
  }

  int getYear() const {
    return(m_date.getYear());
  }

  int getMonth() const {
    return(m_date.getMonth());
  }

  int getDay() const {
    return(m_date.getDay());
  }

  int getDaysPast() const {
    return(m_date.getDaysPast());
  }

  void setYear(long w) {
    m_date.setYear(w);
  }

  void setMonth(long w) {
    m_date.setMonth(w);
  }

  void setDay(long w) {
    m_date.setDay(w);
  }


  std::string getDDMMYYYYHHMMSS() const;
  std::string getRFC1123Date() const;

  CMoment convertGmtToMet() const;
  CMoment convertMetToGmt() const;

  CMoment operator+(const CTimevector &z) const;
  CMoment operator-(const CTimevector &z) const;
  bool operator<(const CMoment &z) const;
  bool operator==(const CMoment &z) const;
  bool isEqual(const CMoment &z, long eps) const;

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CMoment &m);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CMoment &m);

};


}; // namespace Codebook
#endif

