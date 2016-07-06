/*
** Repräsentation der Datei-Informationen
*/

#ifndef __CFILEINFO_H
#define __CFILEINFO_H

#include "CHugeNumber.h"
#include "CMoment.h"
#include "IString.h"
#include "CBinaryStream.h"
#include <string>
#include <vector>

namespace Codebook {


class CFileSize {
public:
  unsigned long m_lowsize;
  unsigned long m_highsize;
/*
** Konstruktor
*/
  CFileSize(unsigned long low, unsigned long high)
    : m_lowsize(low), m_highsize(high)
  {}
/*
** Standard-Konstruktor
*/
  CFileSize()
    : m_lowsize(0), m_highsize(0)
  {}
/*
** Kopier-Konstruktor
*/
  CFileSize(const CFileSize &fs)
    : m_lowsize(fs.m_lowsize), m_highsize(fs.m_highsize)
  {}
  
  CFileSize(const CHugeNumber &n) {
    *this=n.getAsFileSize();
  }
/*
** Vergleichsoperatoren
*/
  bool operator<(const CFileSize &fs) const {
    if (m_highsize<fs.m_highsize)
      return(true);
    if ((m_highsize==fs.m_highsize)&&
      (m_lowsize<fs.m_lowsize))
      return(true);
    return(false);
  }

  bool operator==(const CFileSize &fs) const {
    return((m_lowsize==fs.m_lowsize)&&
          (m_highsize==fs.m_highsize));
  }
/*
** Rechenoperatoren
*/
  CFileSize operator+(const CFileSize &fs) const {
    CFileSize tmp;
    tmp.m_highsize=m_highsize+fs.m_highsize;
    tmp.m_lowsize=m_lowsize+fs.m_lowsize;
    if(tmp.m_lowsize<m_lowsize)
      tmp.m_highsize++;
    return(tmp);
  }

  CFileSize &operator+=(const CFileSize &fs) {
    m_highsize+=fs.m_highsize;
    if((m_lowsize+fs.m_lowsize)<m_lowsize)
      m_highsize++;
    m_lowsize+=fs.m_lowsize;
    return(*this);
  }

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CFileSize &fs){
    os << fs.m_lowsize;
    os << fs.m_highsize;
    return(os);
  }

  friend CBinaryIStream &operator>>(CBinaryIStream &is, CFileSize &fs){
    is >> fs.m_lowsize;
    is >> fs.m_highsize;
    return(is);
  }

};

class CFileInfo {
public:
  typedef std::vector<CFileInfo> filelist_type;
  enum FTYPE {DIR=0, FILE, INVALID};

  std::string m_path;
  std::string m_name;
  FTYPE m_type;
  CFileSize m_size;
  CMoment m_created;
  CMoment m_modified;
  filelist_type *m_filelist;


  CFileInfo()
  : m_type(INVALID), m_filelist(0)
  {}

  CFileInfo(const CFileInfo &fi)
  : m_filelist(0) {
/*
** Aufruf des =-Operators
*/
    *this=fi;
  }

/*
** Destruktor, um eventuell existierende
** Unterlisten freizugeben
*/
  ~CFileInfo() {
    if(m_filelist)
      delete(m_filelist);
  }


  CFileInfo &operator=(const CFileInfo &fi) {
/*
** Zuweisung an sich selbst?
** => Abbruch
*/
    if(this==&fi)
      return(*this);
/*
** Existiert alte Filelist?
** => Freigeben
*/
    if(m_filelist)
      delete(m_filelist);
/*
** Besitzt Quelle eine Filelist?
** => Neue filelist anlegen und kopieren
*/
    if(fi.m_filelist) {
      m_filelist=new filelist_type;
      *m_filelist=*fi.m_filelist;
    }
    else
      m_filelist=0;
/*
** Restliche Attribute kopieren
*/
    m_path=fi.m_path;
    m_name=fi.m_name;
    m_type=fi.m_type;
    m_size=fi.m_size;
    m_created=fi.m_created;
    m_modified=fi.m_modified;
    return(*this);
  }

  CFileInfo getShallowCopy() const {
    CFileInfo cpy;
    cpy.m_path=m_path;
    cpy.m_name=m_name;
    cpy.m_type=m_type;
    cpy.m_size=m_size;
    cpy.m_created=m_created;
    cpy.m_modified=m_modified;
    cpy.m_filelist=0;
    return(cpy);
  }

/*
** Vergleichsoperatoren, um Einträge sortieren zu können
*/

  bool operator<(const CFileInfo &fi) const {
/*
** Verzeichnisse vor Dateien vor ungültigen Einträgen
*/
    if(m_type<fi.m_type)
      return(true);
    if(m_type>fi.m_type)
      return(false);
/*
** Namen vergleichen, Groß- und Kleinschreibung ignorieren
*/
    insensitive_string a=m_name.c_str();
    insensitive_string b=fi.m_name.c_str();
    if(a<b)
      return(true);
    if(a>b)
      return(false);
/*
** Wenn Namen gleich sind, dann zwischen Groß-
** und Kleinschreibung unterscheiden
*/
    if(m_name<=fi.m_name)
      return(true);
    else
      return(false);
  }
/*
  std::string m_path;
  std::string m_name;
  FTYPE m_type;
  CFileSize m_size;
  CMoment m_created;
  CMoment m_modified;
  filelist_type *m_filelist;
*/

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CFileInfo &fi){
    os << fi.m_path;
    os << fi.m_name;
    os.write(&fi.m_type, sizeof(fi.m_type));
    os << fi.m_size;
    os << fi.m_created;
    os << fi.m_modified;
    bool subdir=(fi.m_filelist!=0);
    os << subdir;
    if(subdir)
      os << *fi.m_filelist;
    return(os);
  }

  friend CBinaryIStream &operator>>(CBinaryIStream &is, CFileInfo &fi){
    is >> fi.m_path;
    is >> fi.m_name;
    is.read(&fi.m_type, sizeof(fi.m_type));
    is >> fi.m_size;
    is >> fi.m_created;
    is >> fi.m_modified;
    if(fi.m_filelist) {
      delete(fi.m_filelist);
      fi.m_filelist=0;
    }
    bool subdir;
    is >> subdir;
    if(subdir) {
      fi.m_filelist=new CFileInfo::filelist_type;
      is >> *fi.m_filelist;
    }
    return(is);
  }

};

}; // namespace Codebook

#endif
