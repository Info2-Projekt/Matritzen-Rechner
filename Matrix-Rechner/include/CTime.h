#ifndef __CTIME_H
#define __CTIME_H

#include "CBinaryStream.h"
#include <string>

namespace Codebook {

class CTime {
private:
  int m_hour;
  int m_minute;
  int m_second;
  int m_millisecond;
  long m_millisecondsPast;
//  bool m_isOverflow;

  void calcMillisecondsPast();
  void calcTime();
  void checkRanges();

public:

  CTime(int hour, int minute, int second=0, int millisecond=0);
  CTime(long);
  CTime(const std::string &time);
  CTime();

//  bool isOverflow() {return(m_isOverflow);}
//  void clearOverflow() {m_isOverflow=false;}

  int getHour() const {
    return(m_hour);
  }

  int getMinute() const {
    return(m_minute);
  }

  int getSecond() const {
    return(m_second);
  }

  int getMillisecond() const {
    return(m_millisecond);
  }

  void setHour(long w) {
    m_hour=w;
    checkRanges();
    calcMillisecondsPast();
  }

  void setMinute(long w) {
    m_minute=w;
    checkRanges();
    calcMillisecondsPast();
  }

  void setSecond(long w) {
    m_second=w;
    checkRanges();
    calcMillisecondsPast();
  }

  void setMillisecond(long w) {
    m_millisecond=w;
    checkRanges();
    calcMillisecondsPast();
  }

  void setMillisecondsPast(long w) {
    m_millisecond=w;
    calcTime();
  }
  long getMillisecondsPast() const {
    return(m_millisecondsPast);
  }

  std::string getHHMM() const;
  std::string getHHMMSS() const;
  std::string getHHMMSSMMM() const;
  CTime operator+(long mseconds) const;
  CTime operator-(long mseconds) const;
  CTime operator+(const CTime &z) const;
  CTime operator-(const CTime &z) const;
  bool operator<(const CTime &z) const;
  bool operator==(const CTime &z) const;
  bool isEqual(const CTime &z, long eps) const;

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CTime &t);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CTime &t);

};


}; // namespace Codebook
#endif

