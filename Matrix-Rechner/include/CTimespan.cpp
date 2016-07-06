#include "CTimespan.h"
//#include "dfunktionen.h"
#include <string>
#include <utility>

namespace Codebook {

using namespace std;
using namespace rel_ops;

CTimespan::CTimespan()
: m_start(), m_end() {
checkRanges();
}

CTimespan::CTimespan(const CMoment &b, const CMoment &e)
: m_start(b), m_end(e) {
checkRanges();
}

CTimespan::CTimespan(const CMoment &b, const CTimevector &z)
: m_start(b), m_end(b+z) {
checkRanges();
}

void CTimespan::checkRanges() {
  if(m_start>=m_end)
    throw (out_of_range("CTimespan::checkRanges()"));
}

string CTimespan::getBE() const {
  return(m_start.getDDMMYYYYHHMMSS()+" - "+m_end.getDDMMYYYYHHMMSS());
}

// Die Vergleichsoperatoren bergleichen den Start-Termin

bool CTimespan::operator<(const CTimespan &z) const {
  return(m_start<z.m_start);
}

bool CTimespan::operator==(const CTimespan &z) const {
  return(m_start==z.m_start);
}

bool CTimespan::isOverlapping(const CTimespan &z) const {
  return(!(((m_start<z.m_start)&&(m_end<z.m_start))||
         ((z.m_start<m_start)&&(z.m_end<m_start))));
}

bool CTimespan::isIdentical(const CTimespan &z) const {
  return((m_start==z.m_start)&&(m_end==z.m_end));
}

CTimespan CTimespan::getOverlappingArea(const CTimespan &z) const {
  if(!isOverlapping(z))
    throw (NoOverlapping());

  return(CTimespan((m_start>z.m_start)?m_start:z.m_start,
                   (m_end<z.m_end)?m_end:z.m_end));
}

}; // namespace Codebook