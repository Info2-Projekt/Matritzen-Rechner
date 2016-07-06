#ifndef __CDATE_H
#define __CDATE_H

#include "CTime.h"
#include "CBinaryStream.h"
#include <string>

namespace Codebook {

class CDate {
private:
  int m_day;
  int m_month;
  int m_year;
  long m_daysPast;

  void calcDaysPast();
  void calcDate();
  void checkRanges();

public:

  static bool isLeapyear(int year);
  static int daysPerMonth(int month, int year);
  static long calcDaysPast(int year, int month, int day);
  static std::string calcZodiacSign(int day, int month);
  std::string calcZodiacSign();
  static std::string calcWeekday(int day, int month, int year);
  static CDate calcEasterSunday(int year);
  static CDate calcAshWednesday(int year);
  static CDate calcWeiberfastnacht(int year);
  static CDate calcWhitsun(int year);
  static CDate calcAscensionDay(int year);
  static CDate calcCorpusChristi(int year);
  static CDate calcMothersDay(int year);
  static int calcCalendarWeeksPerYear(int year);
  static CDate getBeginOfCalendarWeek(int year, int cweek);
  int getCalendarWeek() const;
  static std::string getMonthSmallAsHtml(int year, int month);
  static std::string getMonthDetailedAsHtml(int year, int month);
  
  static std::string getCalendarWeekAsHtml(int year, int cweek,
                                           const CTime &begin=CTime(8,0),
                                           const CTime &end=CTime(18,0),
                                           int intervall=30,
                                           int width=100,
                                           int height=25);
  static int calcAge(const CDate &d);
  int calcAge();

  CDate(int day, int month, int year);
  CDate(long);
  CDate(const std::string &date);
  CDate();

  int getDay() const {
    return(m_day);
  }
  int getMonth() const {
    return(m_month);
  }
  int getYear() const {
    return(m_year);
  }
  long getDaysPast() const {
    return(m_daysPast);
  }
  void setDay(int w) {
    m_day=w; 
    checkRanges(); 
    calcDaysPast();
  }
  void setMonth(int w) {
    m_month=w; 
    checkRanges(); 
    calcDaysPast();
  }
  void setYear(int w) {
    m_year=w; 
    checkRanges(); 
    calcDaysPast();
  }
  std::string getDDMM() const;
  std::string getDDMMYYYY() const;
  int getWeekday() const;
  std::string getWeekdayName() const;
  std::string getMonthName() const;


  CDate operator+(long days) const;
  CDate operator-(long days) const;
  long operator-(const CDate &d) const;

  bool operator<(const CDate &d) const;
  bool operator==(const CDate &d) const;

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CDate &d);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CDate &d);

};

}; // namespace Codebook
#endif

