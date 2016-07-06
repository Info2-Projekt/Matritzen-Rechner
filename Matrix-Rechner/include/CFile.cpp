
#include "CFile.h"


using namespace std;

namespace Codebook {

CFile::CFile()
: m_file(0)
{}

//*********

CFile::CFile(const string &name, bool create, bool erase)
: m_file(0) {
  if(!open(name,create))
    throw file_error(m_filename);
}

bool CFile::open(const string &name, bool create, bool erase) {
/*
** Existierende Datei öffnen bzw. löschen
*/
  if(erase)
    m_file=new fstream(name.c_str(), ios::in|ios::out|ios::binary|ios::trunc);
  else
    m_file=new fstream(name.c_str(), ios::in|ios::out|ios::binary);

/*
** Soll bei nicht existenter Datei eine neue angelegt werden?
** => Entsprechende Überprüfungen und Schritte einleiten
*/
  if(create) {
/*
** Fehlgeschlagen? Vielleicht Datei nicht existent
** ==> Neue Datei öffnen
*/
    if(!m_file->is_open()) {
      delete(m_file);
      m_file=new fstream(name.c_str(), ios::in|ios::out|ios::binary|ios::trunc);
    }
  }
/*
** Wieder fehlgeschlagen?
** => Abbruch
*/
  if(!m_file->is_open())
    return(false);

  m_filename=name;
/*
** Dateipositionszeiger ans Dateiende setzen und
** Dateigröße ermitteln
*/
  m_file->seekg(0,ios::end);
  if(m_file->fail())
    return(false);

  m_size=m_file->tellg();
  if(m_file->fail())
    return(false);
/*
** Dateipositionszeiger an Dateianfang setzen
*/
  m_file->seekg(0,ios::beg);
  if(m_file->fail())
    return(false);

  return(true);
}

//*********

CFile::~CFile() {
  if(m_file)
    delete(m_file);
}

//*********

void CFile::readBlock(void *addr,streampos size, streampos pos) {
/*
** Dateipositionszeiger setzen
*/
  m_file->seekg(pos,ios::beg);
  if(m_file->fail())
    throw file_error(m_filename);
/*
** Block lesen
*/
  m_file->read(reinterpret_cast<char*>(addr),size);
  if(m_file->fail())
    throw file_error(m_filename);
}


//*********

void CFile::writeBlock(const void *addr, streampos size, streampos pos) {
/*
** Dateipositionszeiger setzen
*/
  m_file->seekp(pos,ios::beg);
  if(m_file->fail())
    throw file_error(m_filename);
/*
** Block schreiben
*/
  m_file->write(reinterpret_cast<const char*>(addr), size);
  if(m_file->fail())
    throw file_error(m_filename);
}

//*********

void CFile::readBlock(void *addr, streampos size) {
/*
** Block lesen
*/
  m_file->read(reinterpret_cast<char*>(addr), size);
  if(m_file->fail())
    throw file_error(m_filename);
}


//*********

void CFile::writeBlock(const void *addr, streampos size) {
/*
** Block schreiben
*/
  m_file->write(reinterpret_cast<const char*>(addr), size);
  if(m_file->fail())
    throw file_error(m_filename);
}

//*********

void CFile::expand(std::streampos size) {
/*
** Dateipositionszeiger ans Dateiende setzen
*/
  m_file->seekg(0,ios::end);
  if(m_file->fail())
    throw file_error(m_filename);
/*
** So viele Zeichen schreiben, wie zur gewünschten
** Vergrößerung notwendig sind
*/
  char c='\0';
  for(streampos count=0; count<(size-m_size); count+=1)
    writeBlock(&c,1);
  if(m_file->fail())
    throw file_error(m_filename);
/*
** Neue Dateigröße ermitteln
*/
  m_size=m_file->tellg();
}




}; // namespace Codebook

