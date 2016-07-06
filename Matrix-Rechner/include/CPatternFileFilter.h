
#ifndef __CPATTERNFILEFILTER_H
#define __CPATTERNFILEFILTER_H

#include "CFileFilter.h"
//#include "CFileInfo.h"
//#include "CAutomat.h"

#include <string>

namespace Codebook {



class CPatternFileFilter : public CFileFilter{
private:
  std::string m_searchPattern;
  bool m_sCaseSensitivity;
  CAutomat m_searchAutomat;

public:
  CPatternFileFilter(std::string pattern="?*",
                      std::string spattern="?*",
                      bool casesensi=false,
                      bool scasesensi=false,
                      FTYPE types=BOTH,
                      CFileSize minsize=CFileSize(0,0),
                      CFileSize maxsize=CFileSize(0xffffffff,0xffffffff),
                      CMoment minmodi=CMoment(1,1,1,0,0,0,0),
                      CMoment maxmodi=CMoment(3000,12,31,23,59,59,999),
                      CMoment mincrea=CMoment(1,1,1,0,0,0,0),
                      CMoment maxcrea=CMoment(3000,12,31,23,59,59,999))

  : CFileFilter(pattern, casesensi, types, minsize, maxsize, minmodi, maxmodi,
                mincrea, maxcrea),
    m_searchPattern(spattern),
    m_sCaseSensitivity(scasesensi),
    m_searchAutomat((scasesensi)?spattern:toLowerString(spattern))
  {}


  void setSearchPattern(const std::string &p) {
    m_searchPattern=p;
  }

  void setSCaseSensitivity(bool cs) {
    m_sCaseSensitivity=cs;
  }

  std::string getSearchPattern() const {
    return(m_searchPattern);
  }

  bool getSCaseSensitivity() const {
    return(m_sCaseSensitivity);
  }


  virtual bool match(const CFileInfo &fi) const;

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CPatternFileFilter &d);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CPatternFileFilter &d);


};

}; // namespace Codebook

#endif
