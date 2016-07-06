#include "CStopwatch.h"

#include <sstream>
#include <iomanip>

using namespace std;

namespace Codebook {


CStopwatch::CStopwatch()
  : m_running(false)
{}

bool CStopwatch::start() {
  if(m_running)
    return(false);
  m_start=clock();
  m_running=true;
  return(true);
}

bool CStopwatch::stop() {
  if(!m_running)
    return(false);
  unsigned long elapsed=static_cast<unsigned long>((clock()-m_start)*(1000.0/CLOCKS_PER_SEC));
  m_elapsed=m_elapsed+CTimevector(elapsed/86400000,elapsed%86400000);
  m_running=false;
  return(true);
}

void CStopwatch::reset() {
  m_elapsed=CTimevector();
}

string CStopwatch::getDHHMMSSMMM() const {

  stringstream str;
  CTime time(m_elapsed.getMilliseconds());

  str.fill('0');
  str << m_elapsed.getDays() << "D ";
  str << setw(2) << time.getHour() << ":";
  str << setw(2) << time.getMinute() << ":";
  str << setw(2) << time.getSecond() << ":";
  str << setw(3) << time.getMillisecond();// << "M";
  return(str.str());
}


}; // namespace Codebook

