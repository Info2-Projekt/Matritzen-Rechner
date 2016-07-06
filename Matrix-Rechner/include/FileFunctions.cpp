/*
** Allgemeine Dateifunktionen
*/

#include "FileFunctions.h"
#include "CFileArray.h"
#include "StringFunctions.h"
#include "CHugeNumber.h"


#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

namespace Codebook {

bool fileExists(const std::string &name) {
  return(getFileInfo(name).m_type!=CFileInfo::INVALID);
}

//*********

bool moveFile(const string &sname, const string &dname, bool dontOverwrite) {
  if(copyFile(sname.c_str(), dname.c_str(), dontOverwrite))
    return(removeFile(sname));
  else
    return(false);
}

//*********

bool createPath(const string &name) {
/*
** Verzeichnis erfolgreich erstellt?
** => Fertig
*/
  if(createDirectory(name))
    return(true);
  else {
/*
** Lässt sich Pfad weiter zerlegen?
** => createPath rekursiv aufrufen
*/
    if(name.rfind("/")!=string::npos) {
      createPath(name.substr(0,name.rfind("/")));
      return(createDirectory(name));
    }
    else
      return(false);
  }
}

//*********

bool copyDirectory(const string &sname,
                   const string &dname,
                   bool copyMainDir,
                   bool dontOverwrite) {

  string targetpath=dname;
/*
** Ganzes Verzeichnis kopiert werden?
** => Verzeichnis zu Zielpfad hinzufügen
*/
  if(copyMainDir)
    targetpath+="/"+sname.substr(sname.rfind("/")+1);

  bool success=true;
  CFileInfo::filelist_type files;
/*
** Zielpfad des aktuellen Verzeichnisses anlegen
*/
  createPath(targetpath);
/*
** Inhalt des Verzeichnisses ermitteln
*/
  if(getDir(sname,files)) {
/*
** Alle Einträge des Verzeichnisses bearbeiten
*/
    for(CFileInfo::filelist_type::iterator iter=files.begin(); iter!=files.end(); ++iter){
      switch(iter->m_type) {
/*
** Unterverzeichnis?
** => Durch rekursiven Aufruf von copyDirectory kopieren
*/
        case CFileInfo::DIR:
          if((!(copyDirectory(sname+"/"+iter->m_name,
                              targetpath+"/"+iter->m_name,
                              false,
                              dontOverwrite)))&&(dontOverwrite))
            success=false;
          break;
/*
** Datei?
** => Mit copyFile kopieren
*/
        case CFileInfo::FILE:
          if((!(copyFile(sname+"/"+iter->m_name,
                         targetpath+"/"+iter->m_name,
                         dontOverwrite)))&&(dontOverwrite))
            success=false;
          break;
      }
    }
  }
  return(success);
}

//*********

bool removeDirectory(const string &name, bool removeMainDir) {

  bool success=true;
  CFileInfo::filelist_type files;
/*
** Inhalt des Verzeichnisses ermitteln
*/
  if(getDir(name,files)) {
/*
** Alle Einträge des Verzeichnisses bearbeiten
*/
    for(CFileInfo::filelist_type::iterator iter=files.begin(); iter!=files.end(); ++iter){
      switch(iter->m_type) {
/*
** Unterverzeichnis?
** => Durch rekursiven Aufruf von removeDirectory löschen
*/
        case CFileInfo::DIR:
          if(!(removeDirectory(name+"/"+iter->m_name, true)))
            success=false;
          break;
/*
** Datei?
** => Mit removeFile kopieren
*/
        case CFileInfo::FILE:
          if(!(removeFile(name+"/"+iter->m_name)))
            success=false;
          break;
      }
    }
  }
/*
** Soll Wurzelverzeichnis ebenfalls gelöscht werden?
** => Verzeichnis löschen mit removeDirectory
*/
  if(removeMainDir)
    if(!(removeDirectory(name)))
      success=false;

  return(success);
}

//*********

bool moveDirectory(const string &sname,
                   const string &dname,
                   bool moveMainDir,
                   bool dontOverwrite) {
  if(copyDirectory(sname, dname, moveMainDir, dontOverwrite))
    return(removeDirectory(sname, moveMainDir));
  else
    return(false);
}

//*********
                   
void sortFileList(CFileInfo::filelist_type &fl) {
  sort(fl.begin(), fl.end(), less<CFileInfo>());
}

//*********

bool getCompleteDir(const string &name, CFileInfo &fi) {
/*
** Datei-Infos noch nicht geholt?
** => Dann jetzt
*/
  if(fi.m_type==CFileInfo::INVALID) {
    fi=getFileInfo(name);
/*
** Immer noch invalid?
** => Abbruch
*/
    if(fi.m_type==CFileInfo::INVALID)
      return(false);
  }
/*
** Gehört aktuelle Datei-Info zu einem Verzeichnis?
*/
  if(fi.m_type==CFileInfo::DIR) {
/*
** Neue Datei-Liste anlegen
*/
    fi.m_filelist=new CFileInfo::filelist_type;
/*
** Liste füllen und sortieren
*/
    getDir(name, *fi.m_filelist);
    sortFileList(*fi.m_filelist);
/*
** Für Unterverzeichnisse getCompleteDir rekursiv aufrufen
*/
    CFileInfo::filelist_type::iterator iter=fi.m_filelist->begin();
    while((iter!=fi.m_filelist->end())&&(iter->m_type==CFileInfo::DIR)) {
      getCompleteDir(name+"/"+iter->m_name, *iter);
      ++iter;
    }
/*
** Datei-Größe des Verzeichnisses auf 0 setzen
*/
    fi.m_size=CFileSize();
/*
** Größen aller Unterverzeichnisse und Dateien aufaddieren
*/
    for(iter=fi.m_filelist->begin();iter!=fi.m_filelist->end(); ++iter)
      fi.m_size+=iter->m_size;
  }
  return(true);
}

//*********

string dirToHtml(const string &name) {
/*
** Informationen über darzustellendes
** Verzeichnis einholen.
*/
  CFileInfo dirinfo=getFileInfo(name);
/*
** Kein Verzeichnis?
** => Abbruch
*/
  if(dirinfo.m_type!=CFileInfo::DIR)
    return("");
/*
** Verzeichnis-Inhalt ermitteln und sortieren
*/
  CFileInfo::filelist_type fl;
  getDir(name,fl);
  sortFileList(fl);
  return(dirToHtml(fl,dirinfo.m_path));
}

//*********

string dirToHtml(const CFileInfo::filelist_type &filelist, const string &dirname) {
/*
** Pfad als Überschrift mit Linie schreiben
*/
  string str;
  str+="<h2>"+dirname+"/<hr /></h2>\n";
/*
** Tabelle erzeugen und alle Einträge durchlaufen
*/
  str+="<table>\n";
  for(CFileInfo::filelist_type::const_iterator iter=filelist.begin(); iter!=filelist.end(); ++iter) {
    str+="<tr><td>";
/*
** Entsprechend des Eintrag-Typs die Grafik
** t_dir.gif oder t_file.gif verwenden
*/
    switch(iter->m_type) {
      case CFileInfo::DIR:
        str+="<img src=\"t_dir.gif\" width=\"20\" />";
        break;

      case CFileInfo::FILE:
        str+="<img src=\"t_file.gif\" width=\"20\" />";
        break;
    }
/*
** Name des Eintrags schreiben
*/
    str+="</td><td>";
    str+=iter->m_name;
    str+="</td><td width=\"10\">";
/*
** Eintrag vom Typ FILE?
** => Dateigröße schreiben
*/
    if(iter->m_type==CFileInfo::FILE) {
      str+="</td><td>";
      str+=CHugeNumber(iter->m_size.m_lowsize, iter->m_size.m_highsize).getAsPointedNumber();
    }
    else
      str+="</td><td>";
/*
** Datum der letzten Änderung schreiben
*/
    str+="</td><td width=\"10\"></td><td>";
    str+=iter->m_modified.getDDMMYYYYHHMMSS();
/*
** Tabellenzelle und -zeile beenden
*/
    str+="</td></tr>\n";
  }
/*
** Tabelle beenden und String zurückliefern
*/
  str+="</table>\n";
  return(str);
}


//*********

string fileInfoToHtml(const CFileInfo &fi, string line, bool onlyDirs, int imgsize, bool last) {
/*
** Eintrag ist Datei, aber es sollen nur Verzeichnisse dargestellt werden?
** => Abbruch
*/
  if(onlyDirs&&(fi.m_type!=CFileInfo::DIR))
    return("");
  string str;
/*
** in line sind die grafischen Elemente von Funktionen
** aus höheren Rekursionsstufen gespeichert, um die Äste
** korrekt zeichnen zu können
*/
  str+=line;
/*
** Wenn letzer Eintrag, dann Astende zeichnen,
** andernfalls eine Verzweigung
*/
  if(last)
    str+="<img src=\"t_branchend.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
  else
    str+="<img src=\"t_branch.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
/*
** Je nachdem, ob Datei oder Verzeichnis die
** Grafik t_file.gif oder t_dir.gif zeichnen
*/
  if(fi.m_type==CFileInfo::DIR)
    str+="<img src=\"t_dir.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
  else
    str+="<img src=\"t_file.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
/*
** Eintragsnamen und -größe schreiben
*/
  str+=fi.m_name+" ("+CHugeNumber(fi.m_size.m_lowsize, fi.m_size.m_highsize).getAsPointedNumber()+")";
  str+="<br />\n";
/*
** Für tiefere Rekursionsstufe line um die entsprechende
** Astgrafik ergänzen
*/
  if(last)
    line+="<img src=\"t_space.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
  else
    line+="<img src=\"t_line.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
/*
** Besitzt Eintrag Dateien oder Unterverzeichnisse?
** => fileInfoToHtml für jeden Eintrag aufrufen.
*/
  if(fi.m_filelist&&(fi.m_filelist->size()>0))
    for(unsigned int i=0; i<fi.m_filelist->size(); i++)
      str+=fileInfoToHtml((*fi.m_filelist)[i], line, onlyDirs, imgsize, (i==(fi.m_filelist->size()-1)));

  return(str);
}

//*********

string fileInfoToHtml(const CFileInfo &fi, bool onlyDirs, int imgsize, int fontsize) {
  string str;
/*
** Schriftgröße setzen
*/
  str+="<font size=\""+toString(fontsize)+"\">\n";
/*
** Grafik für das Hauptverzeichnis mitsamt
** Namen und Dateigröße setzen
*/
  str+="<img src=\"t_dir.gif\" align=\"middle\" width=\""+toString(imgsize)+"\" />";
  str+=fi.m_name+" ("+CHugeNumber(fi.m_size.m_lowsize, fi.m_size.m_highsize).getAsPointedNumber()+")";
  str+="<br />\n";
/*
** Besitzt Hauptverzeichnis Dateien oder Unterverzeichnisse?
** => Überladenes fileInfoToHtml für jeden Eintrag aufrufen.
*/
  if(fi.m_filelist&&(fi.m_filelist->size()>0))
    for(unsigned int i=0; i<fi.m_filelist->size(); i++)
/*
** Um korrekten grafischen Ast darstellen zu können,
** wird übergeben, ob es der letzte Eintrag ist oder nicht
*/
      str+=fileInfoToHtml((*fi.m_filelist)[i], "", onlyDirs, imgsize, (i==(fi.m_filelist->size()-1)));

  str+="</font>\n";
  return(str);
}

//*********

void findFiles(const string &name, 
               const CFileFilter &filefilter,
               CFileInfo::filelist_type &filelist) {

  CFileInfo::filelist_type direntries;
/*
** Verzeichnis-Einträge auslesen
*/
  getDir(name,direntries);
/*
** Alle Einträge bearbeiten
*/
  for(CFileInfo::filelist_type::iterator iter=direntries.begin(); iter!=direntries.end(); ++iter) {
/*
** Eintrag mit dem FileFilter prüfen und
** ggfs. zu den Suchergebnissen hinzufügen
*/
    if(filefilter.match(*iter))
      filelist.push_back(*iter);
/*
** Ist Eintrag ein Verzeichnis?
** => findFiles rekursiv aufrufen
*/
    if(iter->m_type==CFileInfo::DIR)
      findFiles(name+"/"+iter->m_name,filefilter,filelist);
  }
}

//*********

void findFiles(const CFileInfo &structure, 
               const CFileFilter &filefilter,
               CFileInfo::filelist_type &filelist) {

/*
** Genügt aktuelle FileInfo den Kriterien?
*/
  if(filefilter.match(structure))
    filelist.push_back(structure.getShallowCopy());
/*
** Alle Einträge bearbeiten
*/
  if(structure.m_filelist!=0) {
    for(CFileInfo::filelist_type::iterator iter=structure.m_filelist->begin(); iter!=structure.m_filelist->end(); ++iter) {
/*
** Ist Eintrag ein Verzeichnis?
** => findFiles rekursiv aufrufen
*/
      if(iter->m_type==CFileInfo::DIR)
        findFiles(*iter,filefilter,filelist);
/*
** Andernfalls Eintrag mit dem FileFilter prüfen und
** ggfs. zu den Suchergebnissen hinzufügen
*/
      else
        if(filefilter.match(*iter))
          filelist.push_back((*iter).getShallowCopy());
    }
  }
}

//*********

string searchResultsToHtml(const std::string &name,
                           const CFileFilter &ff,
                           const CFileInfo::filelist_type &fl) {
 
  string str;
/*
** Pfad als Überschrift mit Linie schreiben
*/
  str+="<h2>"+name+"/"+ff.getPattern()+"<hr /></h2>\n";

  CFileSize fs;
/*
** Tabelle erzeugen und alle Einträge durchlaufen
*/
  str+="<table>\n";
  for(CFileInfo::filelist_type::const_iterator iter=fl.begin(); iter!=fl.end(); ++iter) {
    str+="<tr><td>";
/*
** Entsprechend des Eintrag-Typs die Grafik
** t_dir.gif oder t_file.gif verwenden
*/
    switch(iter->m_type) {
      case CFileInfo::DIR:
        str+="<img src=\"t_dir.gif\" width=\"20\" />";
        break;

      case CFileInfo::FILE:
        str+="<img src=\"t_file.gif\" width=\"20\" />";
        break;
    }
/*
** Name des Eintrags schreiben
*/
    str+="</td><td>";
    str+=iter->m_name;
    str+="</td><td width=\"10\">";
/*
** Eintrag vom Typ FILE?
** => Dateigröße schreiben und aufaddieren
*/
    if(iter->m_type==CFileInfo::FILE) {
      str+="</td><td>";
      str+=CHugeNumber(iter->m_size.m_lowsize, iter->m_size.m_highsize).getAsPointedNumber();
      fs+=iter->m_size;
    }
    else
      str+="</td><td>";
/*
** Pfad schreiben
*/
    str+="</td><td width=\"10\"></td><td>";
    str+=iter->m_path;
/*
** Datum der letzten Änderung schreiben
*/
    str+="</td><td width=\"10\"></td><td>";
    str+=iter->m_modified.getDDMMYYYYHHMMSS();
/*
** Tabellenzelle und -zeile beenden
*/
    str+="</td></tr>\n";
  }
/*
** Tabelle beenden und String zurückliefern
*/
  str+="</table>\n";
/*
** Unter einem Strich die Anzahl der gefundenen Objekte
** sowie deren gesamte Speichergröße schreiben
*/
  str+="<hr />"+toString(static_cast<unsigned long>(fl.size()))+" Objekte, ";
  str+=CHugeNumber(fs.m_lowsize, fs.m_highsize).getAsPointedNumber();
  str+=" Bytes";

  return(str);
}

//*********

streampos findFirstInFile(const string &name, const string &search, streampos pos){
  try{
/*
** Datei öffnen und überladene Funktion aufrufen
*/
    CFileArray file(name,false);
    return(findFirstInFile(file,search,pos));
  }
  catch(...) {
    return(CFileArray::npos);
  }
}

//*********

streampos findFirstInFile(CFileArray &file, const string &search, streampos pos){
/*
** Größe der Datei und des
** Suchstrings ermitteln
*/
  streampos fsize=file.size();
  streampos ssize=search.size();
/*
** Kann Suchstring nicht komplett in der Datei liegen?
** => Nicht gefunden
*/
  if(pos+ssize>fsize)
    return(CFileArray::npos);

  streampos x;
  long y;
/*
** Suche nach String in Datei
*/
  for(x=pos,y=0;(x<=fsize-ssize)&&(y<ssize);x+=1,y++)
    if(search[y]!=(file[x])) {x-=y; y=-1;}

  if(y==ssize) return(x-y);
  else return(CFileArray::npos);
}

//*********

streampos findFirstInFileCI(const string &name, const string &search, streampos pos){
  try{
/*
** Datei öffnen und überladene Funktion aufrufen
*/
    CFileArray file(name,false);
    return(findFirstInFileCI(file,search,pos));
  }
  catch(...) {
    return(CFileArray::npos);
  }
}

//*********

streampos findFirstInFileCI(CFileArray &file, const string &search, streampos pos){
/*
** Größe der Datei und des
** Suchstrings ermitteln
*/
  streampos fsize=file.size();
  streampos ssize=search.size();
/*
** Kann Suchstring nicht komplett in der Datei liegen?
** => Nicht gefunden
*/
  if(pos+ssize>fsize)
    return(CFileArray::npos);

  streampos x;
  long y;
/*
** Suche nach String in Datei,
** Groß- Kleinschreibung wird ignoriert
*/
  for(x=pos,y=0;(x<=fsize-ssize)&&(y<ssize);x+=1,y++)
    if(tolower(search[y])!=tolower(file[x])) {x-=y; y=-1;}

  if(y==ssize) return(x-y);
  else return(CFileArray::npos);
}

//*********

streampos findFirstInFile(const string &name, const vector<char> &search, streampos pos){
  try{
/*
** Datei öffnen und überladene Funktion aufrufen
*/
    CFileArray file(name,false);
    return(findFirstInFile(file,search,pos));
  }
  catch(...) {
    return(CFileArray::npos);
  }
}

//*********

streampos findFirstInFile(CFileArray &file, const vector<char> &search, streampos pos){
  try{
/*
** Größe der Datei und des
** Suchstrings ermitteln
*/
    streampos fsize=file.size();
    streampos ssize=search.size();
/*
** Kann Suchstring nicht komplett in der Datei liegen?
** => Nicht gefunden
*/
    if(pos+ssize>fsize)
      return(CFileArray::npos);

    streampos x;
    long y;
/*
** Suche in der Datei nach den Daten im Vektor
*/
    for(x=pos,y=0;(x<=fsize-ssize)&&(y<ssize);x+=1,y++)
      if(search[y]!=(file[x])) {x-=y; y=-1;}

    if(y==ssize) return(x-y);
    else return(CFileArray::npos);
  }
  catch(...) {
    return(CFileArray::npos);
  }
}

//*********

streampos findFirstInFile(const string &name, const deque<char> &search, streampos pos){
  try{
/*
** Datei öffnen und überladene Funktion aufrufen
*/
    CFileArray file(name,false);
    return(findFirstInFile(file,search,pos));
  }
  catch(...) {
    return(CFileArray::npos);
  }
}

//*********

streampos findFirstInFile(CFileArray &file, const deque<char> &search, streampos pos){
  try{
/*
** Größe der Datei und des
** Suchstrings ermitteln
*/
    streampos fsize=file.size();
    streampos ssize=search.size();
/*
** Kann Suchstring nicht komplett in der Datei liegen?
** => Nicht gefunden
*/
    if(pos+ssize>fsize)
      return(CFileArray::npos);

    streampos x;
    long y;
/*
** Sucht in der Datei nach den Daten in der Deque
*/
    for(x=pos,y=0;(x<=fsize-ssize)&&(y<ssize);x+=1,y++)
      if(search[y]!=(file[x])) {x-=y; y=-1;}

    if(y==ssize) return(x-y);
    else return(CFileArray::npos);
  }
  catch(...) {
    return(CFileArray::npos);
  }
}

streampos compareFiles(const string &name1,const string &name2) {
  streampos pos;
  try {
/*
** Beide Dateien öffnen ohne neue Dateien anzulegen
** und kleinere Dateigröße als Ende verwenden.
*/
    CFileArray file1(name1,false), file2(name2, false);
    streampos end=(file1.size()<file2.size())?file1.size():file2.size();
/*
** Dateien zeichenweise vergleichen
*/
    for(pos=0; pos<end; pos+=1)
      if(file1[pos]!=file2[pos])
        return(pos);

/*
** Unterschiedliche Größe?
** => Erste nur noch in einer Datei befindliche Position
**    zurückgeben
*/
    if(file1.size()!=file2.size())
      return(end);

/*
** Bei Gleichheit CfileArray::npos zurückgeben
*/
    return(CFileArray::npos);
  }
/*
** Eventuelle Ausnahmen fangen und eigene werfen
*/
  catch(...) {
    throw CFileArray::file_error("compareFiles()");
  }
}

streampos findFirstPatternInFile(const string &name, 
                                 const string &search, 
                                 bool casesensitivity, 
                                 streampos p) {
  try {
    CFileArray fa(name);
    return(findFirstPatternInFile(fa,search,casesensitivity, p));
  }
  catch(CFileArray::file_error) {
    throw CFileArray::file_error("findfirstPatternInFile()");
  }
}

streampos findFirstPatternInFile(CFileArray &file, 
                                 const string &search, 
                                 bool casesensitivity, 
                                 streampos p) {
  try {
    CAutomat a(search);
    if(casesensitivity)
      return(a.find(file,p));
    else
      return(a.findCI(file,p));
  }
  catch(...) {
    throw invalid_argument("findFirstPatternInFile()");
  }
}





}; // namespace Codebook

