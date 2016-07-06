
#ifndef __CTEXTFILEFILTER_H
#define __CTEXTFILEFILTER_H

#include "CFileFilter.h"
//#include "CFileInfo.h"
//#include "CAutomat.h"

#include <string>

namespace Codebook {



class CTextFileFilter : public CFileFilter{
private:
  std::string m_searchText;
  bool m_sCaseSensitivity;

public:
  CTextFileFilter(std::string pattern="?*",
                      std::string stext="",
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
    m_searchText(stext),
    m_sCaseSensitivity(scasesensi)
  {}


  void setSearchText(const std::string &p) {
    m_searchText=p;
  }

  void setSCaseSensitivity(bool cs) {
    m_sCaseSensitivity=cs;
  }

  std::string getSearchText() const {
    return(m_searchText);
  }

  bool getSCaseSensitivity() const {
    return(m_sCaseSensitivity);
  }


  virtual bool match(const CFileInfo &fi) const;

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CTextFileFilter &d);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CTextFileFilter &d);


};

}; // namespace Codebook

#endif
