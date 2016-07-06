#include "CPatternFileFilter.h"

namespace Codebook {

using namespace std;
using namespace rel_ops;


bool CPatternFileFilter::match(const CFileInfo &fi) const {
/*
** Kriterien der Basisklasse prüfen.
*/
  if(!CFileFilter::match(fi))
    return(false);
/*
** Dateityp nicht FILE?
** => Keine weitere Überprüfung
*/
  if(fi.m_type!=CFileInfo::FILE)
    return(false);


/*
** Datei nach Text durchsuchen. Je nach Berücksichtigung
** von Groß-/Kleinschreibung entsprechende Methode aufrufen
*/
  try {
  CFileArray fa(fi.m_path+"/"+fi.m_name);

    if(m_sCaseSensitivity)
      return(m_searchAutomat.find(fa)!=CFileArray::npos);
    else
      return(m_searchAutomat.findCI(fa)!=CFileArray::npos);

  }
  catch(...) {
    throw CFileArray::file_error("CPatternFileFilter::match()");
  }
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CPatternFileFilter &d) {
  os << static_cast<CFileFilter>(d);
  os << d.m_searchPattern;
  os << d.m_sCaseSensitivity;
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CPatternFileFilter &d) {
  is >> static_cast<CFileFilter>(d);
  is >> d.m_searchPattern;
  is >> d.m_sCaseSensitivity;
  d.m_searchAutomat.rebuild(d.m_searchPattern);
  return(is);
}






}; // namespace Codebook
