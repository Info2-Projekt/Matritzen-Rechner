#ifndef __CTIMESPAN_H
#define __CTIMESPAN_H

#include<iostream>
#include <string>
#include "CMoment.h"

namespace Codebook {

class CTimespan {
private:
  CMoment m_start;
  CMoment m_end;

  void checkRanges();

public:
  class NoOverlapping{};

  // Konstruktoren
  CTimespan();
  CTimespan(const CMoment &b, const CMoment &e);
  CTimespan(const CMoment &b, const CTimevector &e);

/*
** Methoden für Start-Zeitpunkt-Attribute
*/
  // Zeit und Datum per Methode
  CMoment getStart() const {return(m_start);}
  CDate getStartDate() const {return(m_start.getDate());}
  CTime getStartTime() const {return(m_start.getTime());}
  void setStart(const CMoment &z) {m_start=z;}
  void setStartDate(const CDate &d) {m_start.setDate(d);}
  void setStartTime(const CTime &z) {m_start.setTime(z);}
  // Lesen und Setzten der einzelnen Attribute
  int getStartHour() const {return(m_start.getTime().getHour());}
  int getStartMinute() const {return(m_start.getTime().getMinute());}
  int getStartSecond() const {return(m_start.getTime().getSecond());}
  int getStartMillisecond() const {return(m_start.getTime().getMillisecond());}
  void setStartHour(long w) {m_start.getTime().setHour(w);}
  void setStartMinute(long w) {m_start.getTime().setMinute(w);}
  void setStartSecond(long w) {m_start.getTime().setSecond(w);}
  void setStartMillisecond(long w) {m_start.getTime().setMillisecond(w);}
  int getStartYear() const {return(m_start.getDate().getYear());}
  int getStartMonth() const {return(m_start.getDate().getMonth());}
  int getStartDay() const {return(m_start.getDate().getDay());}
  void setStartYear(long w) {m_start.getDate().setYear(w);}
  void setStartMonth(long w) {m_start.getDate().setMonth(w);}
  void setStartDay(long w) {m_start.getDate().setDay(w);}
  std::string getStartDDMMYYYYHHMMSS() const {return(m_start.getDDMMYYYYHHMMSS()); }

/*
** Methoden für End-Zeitpunkt-Attribute
*/
  // Zeit und Datum per Methode
  CMoment getEnd() const {return(m_end);}
  CDate getEndDate() const {return(m_end.getDate());}
  CTime getEndTime() const {return(m_end.getTime());}
  void setEnd(const CMoment &z) {m_end=z;}
  void setEndDate(const CDate &d) {m_end.setDate(d);}
  void setEndTime(const CTime &z) {m_end.setTime(z);}
  // Lesen und Setzten der einzelnen Attribute
  int getEndHour() const {return(m_end.getTime().getHour());}
  int getEndMinute() const {return(m_end.getTime().getMinute());}
  int getEndSecond() const {return(m_end.getTime().getSecond());}
  int getEndMillisecond() const {return(m_end.getTime().getMillisecond());}
  void setEndHour(long w) {m_end.getTime().setHour(w);}
  void setEndMinute(long w) {m_end.getTime().setMinute(w);}
  void setEndSecond(long w) {m_end.getTime().setSecond(w);}
  void setEndMillisecond(long w) {m_end.getTime().setMillisecond(w);}
  int getEndYear() const {return(m_end.getDate().getYear());}
  int getEndMonth() const {return(m_end.getDate().getMonth());}
  int getEndDay() const {return(m_end.getDate().getDay());}
  void setEndYear(long w) {m_end.getDate().setYear(w);}
  void setEndMonth(long w) {m_end.getDate().setMonth(w);}
  void setEndDay(long w) {m_end.getDate().setDay(w);}
  std::string getEndDDMMYYYYHHMMSS() const {return(m_end.getDDMMYYYYHHMMSS()); }

  std::string getBE() const;

  CTimespan operator+(const CTimevector &z) const;
  CTimespan operator-(const CTimevector &z) const;
  bool operator<(const CTimespan &z) const;
  bool operator==(const CTimespan &z) const;

  bool isIdentical(const CTimespan &z) const;
  bool isOverlapping(const CTimespan &z) const;
  CTimespan getOverlappingArea(const CTimespan &z) const;
//  bool isEqual(const CTime &z, long eps) const;

};

}; // namespace Codebook
#endif
