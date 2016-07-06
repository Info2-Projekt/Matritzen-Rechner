#include "CFileFilter.h"

namespace Codebook {

using namespace std;
using namespace rel_ops;

bool CFileFilter::match(const string &n) const {
/*
** Gro�- und Kleinschreibung nicht beachten?
** => Entsprechende CAutomat-Methode aufrufen
*/
  if(m_caseSensitivity)
    return(m_nameAutomat.match(n,0,string::npos));
  else
    return(m_nameAutomat.matchCI(n,0,string::npos));
}

//*********

bool CFileFilter::match(const CFileInfo &fi) const{
/*
** Pr�fen, ob gew�nschter Dateityp
*/
  switch(fi.m_type) {
    case CFileInfo::DIR:
      if(!(m_types&DIR))
        return(false);
      break;

    case CFileInfo::FILE:
      if(!(m_types&FILE))
        return(false);
      break;
  }
/*
** Dateinamen �berpr�fen
** Gro�- und Kleinschreibung nicht beachten?
** => Entsprechende CAutomat-Methode aufrufen
*/
  bool matches;
  if(m_caseSensitivity)
    matches=m_nameAutomat.match(fi.m_name,0,string::npos);
  else
    matches=m_nameAutomat.matchCI(fi.m_name,0,string::npos);
  if(!matches)
    return(false);

/*
** Dateigr��e �berpr�fen
*/
  if((fi.m_size<m_minSize)||(fi.m_size>m_maxSize))
    return(false);
/*
** Erstellungsdatum �berpr�fen
*/
  if((fi.m_created<m_minCreated)||(fi.m_created>m_maxCreated))
    return(false);
/*
** Datum der letzten �nderung �berpr�fen
*/
  if((fi.m_modified<m_minModified)||(fi.m_modified>m_maxModified))
    return(false);

  return(true);
}

//*********

string CFileFilter::convertWinPattern(const string &p) {
  string str;
/*
** Alle Zeichen des Musters durchlaufen
*/
  for(size_t i=0; i<p.size(); i++) {
/*
** Oder-Verkn�pfung umwandeln
*/
    if((p[i]==';')&&(i!=p.size()-1))
      str+="|";
/*
** H�llenbildung umwandeln
*/
    else if(p[i]=='*')
      str+="?*";
/*
** Positive H�lle kodieren
*/
    else if(p[i]=='+')
      str+="~+";
/*
** Sonderzeichen kodieren
*/
    else if(p[i]=='~')
      str+="~~";
/*
** " und abschlie�endes ; ignorieren
*/
    else if((p[i]=='\"')||
            (p[i]==';'));
/*
** Eckige Klammern kodieren
*/
    else if((p[i]=='[')||
            (p[i]==']')) {
      str+="~";
      str+=p[i];
    }
/*
** Geschweifte Klammern kodieren
*/
    else if((p[i]=='{')||
           (p[i]=='}')) {
      str+="~";
      str+=p[i];
    }
/*
** Zeichen unver�ndert �bernehmen
*/
    else
      str+=p[i];
  }
  return(str);
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CFileFilter &ff) {
  os << ff.m_caseSensitivity;
  os << ff.m_minCreated;
  os << ff.m_maxCreated;
  os << ff.m_minModified;
  os << ff.m_maxModified;
  os << ff.m_minSize;
  os << ff.m_maxSize;
  os << ff.m_namePattern;
  os.write(&ff.m_types, sizeof(ff.m_types));
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CFileFilter &ff) {
  is >> ff.m_caseSensitivity;
  is >> ff.m_minCreated;
  is >> ff.m_maxCreated;
  is >> ff.m_minModified;
  is >> ff.m_maxModified;
  is >> ff.m_minSize;
  is >> ff.m_maxSize;
  is >> ff.m_namePattern;
  is.read(&ff.m_types, sizeof(ff.m_types));
  ff.m_nameAutomat.rebuild(ff.m_namePattern);
  return(is);
}





}; // namespace Codebook
