#include"CTime.h"
#include "StringFunctions.h"
#include<time.h>
#include<sstream>
#include<iomanip>
#include <iostream>

namespace Codebook {

using namespace std;

void CTime::checkRanges() {
  if((m_hour<0)||(m_hour>23))
    throw out_of_range("CTime::checkRanges");
  if((m_minute<0)||(m_minute>59))
    throw out_of_range("CTime::checkRanges");
  if((m_second<0)||(m_second>59))
    throw out_of_range("CTime::checkRanges");
  if((m_millisecond<0)||(m_millisecond>999))
    throw out_of_range("CTime::checkRanges");
}


void CTime::calcMillisecondsPast() {
  m_millisecondsPast=(((((m_hour*60)+m_minute)*60)+m_second)*1000)+m_millisecond;
}

void CTime::calcTime() {
  long mseconds=m_millisecondsPast;

  m_millisecond=mseconds%1000;
  mseconds/=1000;
  m_second=mseconds%60;
  mseconds/=60;
  m_minute=mseconds%60;
  m_hour=mseconds/60;

  checkRanges();
}


string CTime::getHHMM() const {

  stringstream str;

  str.fill('0');
  str << setw(2) << m_hour << ":";
  str << setw(2) << m_minute;
  return(str.str());
}

string CTime::getHHMMSS() const {

  stringstream str;

  str.fill('0');
  str << setw(2) << m_hour << ":";
  str << setw(2) << m_minute << ":";
  str << setw(2) << m_second;
  return(str.str());
}

string CTime::getHHMMSSMMM() const {

  stringstream str;

  str.fill('0');
  str << setw(2) << m_hour << ":";
  str << setw(2) << m_minute << ":";
  str << setw(2) << m_second << ":";
  str << setw(3) << m_millisecond;
  return(str.str());
}
// Standard-Konstruktor liefert aktuelles Datum

CTime::CTime() {
  time_t t;
  time(&t);
  tm *ts;
  ts=localtime(&t);
  m_hour=ts->tm_hour;
  m_minute=ts->tm_min;
  m_second=ts->tm_sec;
  m_millisecond=0;

  checkRanges();
  calcMillisecondsPast();
}

CTime::CTime(const string &time) {
/*
** Uhrzeit in Einzelteile zerlegen
*/
  vector<string> components=chopIntoWords(time," :");
/*
** Es dürfen nur drei oder vier Einzelteile sein.
*/
  if((components.size()<3)||(components.size()>4))
    throw invalid_argument("CTime::CTime(string)");
/*
** Einzelteile umwandeln
*/
  m_hour=atol(components[0].c_str());
  m_minute=atol(components[1].c_str());
  m_second=atol(components[2].c_str());
/*
** Gegebenenfalls noch die Millisekunden umwandeln
*/
  if(components.size()==4)
    m_millisecond=atol(components[3].c_str());
  else
    m_millisecond=0;

  checkRanges();
  calcMillisecondsPast();
}

CTime::CTime(int hour, int minute, int second, int millisecond)
:m_hour(hour), m_minute(minute), m_second(second), m_millisecond(millisecond) {

  checkRanges();
  calcMillisecondsPast();

}

CTime::CTime(long milliseconds)
: m_millisecondsPast(milliseconds) {
  if(m_millisecondsPast<0) {
    m_millisecondsPast+=24*60*60*1000;
  } else if(m_millisecondsPast>=24*60*60*1000) {
    m_millisecondsPast%=24*60*60*1000;
  }

  calcTime();
}

CTime CTime::operator+(long mseconds) const {
  return(CTime(m_millisecondsPast+mseconds));
}

CTime CTime::operator+(const CTime &z) const {
  return(CTime(m_millisecondsPast+m_millisecondsPast));
}


CTime CTime::operator-(long mseconds) const {
  return(CTime(m_millisecondsPast-mseconds));
}

CTime CTime::operator-(const CTime &z) const {
  return(CTime(labs(m_millisecondsPast-m_millisecondsPast)));
}

bool CTime::operator<(const CTime &z) const {
  return(m_millisecondsPast<z.m_millisecondsPast);
}

bool CTime::operator==(const CTime &z) const {
  return(m_millisecondsPast==z.m_millisecondsPast);
}

bool CTime::isEqual(const CTime &z, long eps) const {
  return(labs(m_millisecondsPast-z.m_millisecondsPast)<=eps);
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CTime &t) {
  os << t.m_millisecondsPast;
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CTime &t) {
  is >> t.m_millisecondsPast;
  t.calcTime();
  t.checkRanges();
  return(is);
}


}; // namespace Codebook
