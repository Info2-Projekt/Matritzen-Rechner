#include "CTextFileFilter.h"
#include "FileFunctions.h"

namespace Codebook {

using namespace std;



bool CTextFileFilter::match(const CFileInfo &fi) const {
/*
** Kriterien der Basisklasse pr�fen.
*/
  if(!CFileFilter::match(fi))
    return(false);
/*
** Dateityp nicht FILE?
** => Keine weitere �berpr�fung
*/
  if(fi.m_type!=CFileInfo::FILE)
    return(false);

/*
** Datei nach Text durchsuchen. Je nach Ber�cksichtigung
** von Gro�-/Kleinschreibung entsprechende Funktion aufrufen
*/
  try {
    if(m_sCaseSensitivity)
      return(findFirstInFile(fi.m_path+"/"+fi.m_name,m_searchText)!=CFileArray::npos);
    else
      return(findFirstInFileCI(fi.m_path+"/"+fi.m_name,m_searchText)!=CFileArray::npos);

  }
  catch(...) {
    throw CFileArray::file_error("CTextFileFilter::match()");
  }
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CTextFileFilter &d) {
  os << static_cast<CFileFilter>(d);
  os << d.m_searchText;
  os << d.m_sCaseSensitivity;
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CTextFileFilter &d) {
  is >> static_cast<CFileFilter>(d);
  is >> d.m_searchText;
  is >> d.m_sCaseSensitivity;
  return(is);
}






}; // namespace Codebook
