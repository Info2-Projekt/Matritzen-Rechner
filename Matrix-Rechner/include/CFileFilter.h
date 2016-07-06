
#ifndef __CFILEFILTER_H
#define __CFILEFILTER_H

#include "CMoment.h"
#include "CFileInfo.h"
#include "CAutomat.h"

#include <string>

namespace Codebook {



class CFileFilter {
public:
  enum FTYPE {DIR=1, FILE=2, BOTH=3};
private:
  bool m_caseSensitivity;
  CMoment m_minCreated;
  CMoment m_maxCreated;
  CMoment m_minModified;
  CMoment m_maxModified;
  CFileSize m_minSize;
  CFileSize m_maxSize;
  std::string m_namePattern;
  FTYPE m_types;
  CAutomat m_nameAutomat;

public:
  CFileFilter(std::string pattern="?*",
              bool casesensi=false,
              FTYPE types=BOTH,
              CFileSize minsize=CFileSize(0,0),
              CFileSize maxsize=CFileSize(0xffffffff,0xffffffff),
              CMoment minmodi=CMoment(1,1,1,0,0,0,0),
              CMoment maxmodi=CMoment(3000,12,31,23,59,59,999),
              CMoment mincrea=CMoment(1,1,1,0,0,0,0),
              CMoment maxcrea=CMoment(3000,12,31,23,59,59,999))

  : m_namePattern(pattern),
    m_caseSensitivity(casesensi),
    m_types(types),
    m_minSize(minsize),
    m_maxSize(maxsize),
    m_minModified(minmodi),
    m_maxModified(maxmodi),
    m_minCreated(mincrea),
    m_maxCreated(maxcrea),
    m_nameAutomat((casesensi)?pattern:toLowerString(pattern))
  {}

  void setPattern(const std::string &p) {
    m_namePattern=p;
  }

  void setCaseSensitivity(bool cs) {
    m_caseSensitivity=cs;
  }

  void setTypes(FTYPE f) {
    m_types=f;
  }

  void setMinSize(const CFileSize &fs) {
    m_minSize=fs;
  }

  void setMaxSize(const CFileSize &fs) {
    m_maxSize=fs;
  }

  void setMinCreated(const CMoment &m) {
    m_minCreated=m;
  }

  void setMaxCreated(const CMoment &m) {
    m_maxCreated=m;
  }

  void setMinModified(const CMoment &m) {
    m_minModified=m;
  }

  void setMaxModified(const CMoment &m) {
    m_maxModified=m;
  }

  std::string getPattern() const {
    return(m_namePattern);
  }

  CFileSize getMinSize() const {
    return(m_minSize);
  }

  CFileSize getMaxSize() const {
    return(m_maxSize);
  }

  CMoment getMinCreated() const {
    return(m_minCreated);
  }

  CMoment getMaxCreated() const {
    return(m_maxCreated);
  }

  CMoment getMinModified() const {
    return(m_minModified);
  }

  CMoment getMaxModified() const {
    return(m_maxModified);
  }

  bool getCaseSensitivity() const {
    return(m_caseSensitivity);
  }

  FTYPE getTypes() const {
    return(m_types);
  }


  virtual bool match(const std::string &n) const;
  virtual bool match(const CFileInfo &fi) const;
  static std::string convertWinPattern(const std::string &p);

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CFileFilter &d);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CFileFilter &d);


};

}; // namespace Codebook

#endif
