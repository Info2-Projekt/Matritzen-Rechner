/*
** Systemspezifische Dateifunktionen
** Hier: Windows API- Funktionen
*/

#include "FileSpecifics.h"

#include <windows.h>
#include <iostream>

using namespace std;

namespace Codebook {

bool getDir(const std::string &name, CFileInfo::filelist_type &files) {
/*
** Windows-Spezifische Struktur für Datei-Infos
*/
  WIN32_FIND_DATA data;
/*
** Erste Datei des Verzeichnisses ermitteln
*/
  HANDLE hdl=FindFirstFile((name+"/*").c_str(), &data);
/*
** Konnte nicht im Verzeichnis gelesen werden?
** => Abbruch
*/
  if(hdl==INVALID_HANDLE_VALUE)
    return(false);
  do {
    CFileInfo fe;
/*
** Erstellungsdatum ermitteln
*/
    SYSTEMTIME stime;
    FileTimeToSystemTime(&data.ftCreationTime, &stime);
    fe.m_created=CMoment(stime.wYear, stime.wMonth, stime.wDay,
      stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds);
/*
** Datum der letzten Änderung ermitteln
*/
    FileTimeToSystemTime(&data.ftLastWriteTime, &stime);
    fe.m_modified=CMoment(stime.wYear, stime.wMonth, stime.wDay,
      stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds);
/*
** Dateiname und Pfad ermitteln
*/
    fe.m_name=data.cFileName;
    fe.m_path=name;
/*
** Dateigröße ermitteln
*/
    fe.m_size=CFileSize(data.nFileSizeLow,data.nFileSizeHigh);
/*
** Typ bestimmen
*/
    if(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
      fe.m_type=CFileInfo::DIR;
    else
      fe.m_type=CFileInfo::FILE;
/*
** Kein Standardverzeichnis? (wie . oder ..)
** => Zur Liste hinzufügen.
*/
    if((fe.m_name!=".")&&(fe.m_name!=".."))
      files.push_back(fe);
/*
** Nächste Datei ermitteln und bei Misserfolg
** die Schleife abbrechen
*/
  } while(FindNextFile(hdl, &data));

  FindClose(hdl);
  return(true);
}

//*********

CFileInfo getFileInfo(const string &name) {
  CFileInfo fe;
/*
** Windows-Spezifische Struktur für Datei-Infos
*/
  WIN32_FIND_DATA data;

/*
** Konnte Datei nicht geöffnet werden?
** => Leere (auf INVALID gesetzte) FileInfo zurückgeben
*/
  HANDLE hdl=FindFirstFile(name.c_str(), &data);
  if(hdl==INVALID_HANDLE_VALUE)
    return(fe);
/*
** Erstellungsdatum ermitteln
*/
  SYSTEMTIME stime;
  FileTimeToSystemTime(&data.ftCreationTime, &stime);
  fe.m_created=CMoment(stime.wYear, stime.wMonth, stime.wDay,
    stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds);
/*
** Datum der letzten Änderung ermitteln
*/
  FileTimeToSystemTime(&data.ftLastWriteTime, &stime);
  fe.m_modified=CMoment(stime.wYear, stime.wMonth, stime.wDay,
    stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds);
/*
** Dateiname und Pfad ermitteln
*/
  fe.m_name=data.cFileName;
  if(name.find("/")!=string::npos)
    fe.m_path=name.substr(0,name.rfind("/"));
  else
    fe.m_path="";
/*
** Dateigröße ermitteln
*/
  fe.m_size=CFileSize(data.nFileSizeLow,data.nFileSizeHigh);
/*
** Typ bestimmen
*/
  if(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
    fe.m_type=CFileInfo::DIR;
  else
    fe.m_type=CFileInfo::FILE;

  FindClose(hdl);
  return(fe);
}

//*********

bool createDirectory(const std::string &name) {
  return(CreateDirectory(name.c_str(),NULL)!=0);
}

//*********

bool removeDirectory(const string &name) {
  return(RemoveDirectory(name.c_str())!=0);
}

//*********

bool removeFile(const string &name) {
  return(DeleteFile(name.c_str())!=0);
}

//*********

bool copyFile(const std::string &sname, const std::string &dname, bool dontOverwrite) {
  return(CopyFile(sname.c_str(), dname.c_str(), dontOverwrite)!=0);
}




}; // namespace Codebook

