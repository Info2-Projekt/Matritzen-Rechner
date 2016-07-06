
#ifndef __CSTOPWATCH_H
#define __CSTOPWATCH_H

#include "CTime.h"
#include "CTimevector.h"
#include <ctime>

namespace Codebook {

class CStopwatch {
private:
  bool m_running;
  clock_t m_start;
  CTimevector m_elapsed;

public:
  CTimevector getElapsed() const {
    return(m_elapsed);
  }

  int getDays() const {
    return(m_elapsed.getDays());
  }

  int getHours() const {
    return(CTime(m_elapsed.getMilliseconds()).getHour());
  }

  int getMinutes() const {
    return(CTime(m_elapsed.getMilliseconds()).getMinute());
  }

  int getSeconds() const {
    return(CTime(m_elapsed.getMilliseconds()).getSecond());
  }

  int getMilliseconds() const {
    return(CTime(m_elapsed.getMilliseconds()).getMillisecond());
  }

  CStopwatch();

  bool start();
  bool stop();
  void reset();
  std::string getDHHMMSSMMM() const;

};



}; // namespace Codebook

#endif
