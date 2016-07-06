
#include "CFileArray.h"


using namespace std;

namespace Codebook {

streampos CFileArray::npos=-1;

CFileArray::CFileArray(const string &name, bool create, unsigned long puffsize)
: m_mem(puffsize), m_file(name,create), m_pufferSize(puffsize) {
  m_changed=false;
  m_blockPos=0;
  m_size=m_file.size();
/*
** Dateianfang in Speicher lesen
*/
  readBlock();

}

//*********

void CFileArray::readBlock() {
/*
** Liegen in der Datei ab Position m_blockPos weniger
** Daten als der Puffer gro� ist?
*/
  if((m_blockPos+m_pufferSize)>m_size)
/*
** JA: Blockgr��e auf Gr��e der restlichen Daten
** in der Datei setzen.
*/
    m_blockSize=m_size-m_blockPos;
  else
/*
** NEIN: Blockgr��e auf Gr��e des Pufferspeichers setzen
*/
    m_blockSize=m_pufferSize;
/*
** Daten in Pufferspeicher lesen
*/
  m_file.readBlock(m_mem,m_blockSize,m_blockPos);
}

//*********

void CFileArray::writeBlock() {
/*
** Pufferspeicher an die entsprechende Position
** in der Datei schreiben.
*/
  m_file.writeBlock(m_mem,m_blockSize,m_blockPos);
}

//*********

CFileArray::~CFileArray() {
/*
** Noch ge�nderte Daten im Pufferspeicher?
** => Daten vor Zers�rung in Datei schreiben.
*/
  if(m_changed) {
    writeBlock();
    m_changed=false;
  }
}


//*********

char &CFileArray::CharRef(streampos b) {
/*
** Wird auf Daten hinter dem Dateiende zugegriffen?
** => Datei bis zu dieser Stelle vergr��ern
*/
  if(b>=m_size) {
    m_file.expand(b+1L);
    m_size=m_file.size();
  }
/*
** Liegt gew�nschtes char-Element in dem Datei-Bereich,
** der sich im Pufferspeicher befindet?
** => char-Element zur�ckliefern
*/
  if((b>=m_blockPos)&&(b<(m_blockPos+m_blockSize)))
    return(m_mem[b-m_blockPos]);
  else {
/*
** Sollte gew�nschtes char-Element nicht im Pufferspeicher
** liegen, dann:
** Befinden sich im Pufferspeicher ge�nderte Daten?
** => Abspeichern
*/
    if(m_changed) {
      writeBlock();
      m_changed=false;
    }
/*
** Liegt gew�nschtes char-Element dicht am Dateianfang?
** => Neuer Block liegt am Dateianfang.
*/
    if((m_pufferSize/2)>=b)
      m_blockPos=0;
    else
/*
** Wenn nicht, dann Blockposition so w�hlen, dass
** sich das gew�nschte char-Element in der Mitte des
** Blocks befindet
*/
      m_blockPos=b-static_cast<streampos>(m_pufferSize/2);
/*
** Block einlesen und gew�nschtes char-Element zur�ckliefern.
*/
    readBlock();
    return(m_mem[b-m_blockPos]);
  }
}

//*********

CFileArray::CProxy CFileArray::operator[](unsigned long b) {
/*
** Ein CProxy-Objekt f�r das gew�nschte char-Element zur�ckliefern
*/
  return(CProxy(*this,b));
}


//*********




}; // namespace Codebook

