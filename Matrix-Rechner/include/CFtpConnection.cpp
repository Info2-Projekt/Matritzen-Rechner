#define CBTR // Definieren, für Server-Ausgaben
#ifdef CBTR
#include <iostream>
#define CBTRACELN(s) std::cout << (s) << endl;
#define CBTRACE(s) std::cout << (s);
#else
#define CBTRACELN(s)
#define CBTRACE(s)
#endif




#include "CFtpConnection.h"
#include "StringFunctions.h"
#include "FileFunctions.h"
#include "CFile.h"
#include "CMemory.h"
#include "CHugeNumber.h"

using namespace std;

namespace Codebook {


const string CFtpConnection::AnonymousName="anonymous";
const string CFtpConnection::AnonymousPwd="test";

CFtpConnection::ServerReply CFtpConnection::splitReply(const string &line) const {
  ServerReply reply;
  if(line.size()>4) {
    reply.first=line.substr(0,3);
    reply.second=line.substr(4);
  }
  return(reply);
}

//*********

CFtpConnection::ServerReply CFtpConnection::waitForCmdGroup(int group) {
  ServerReply reply;
  do {
    reply=splitReply(m_iconn->receiveLine());
/*
** Keine Server-Antwort?
** => Ausnahme werfen
*/
    if(reply.first=="") {
      CBTRACELN("waitForCmdGroup: Keine Server-Antwort! => Ausnahme werfen");
      throw internet_error("waitForCmdGroup: Keine Server-Antwort");
    }

#ifdef CBTR
    if(atoi(reply.first.substr(1,1).c_str())!=group)
      CBTRACELN("UEBERLESEN: "+rebuildReply(reply));
#endif

/*
** Einlesen wiederholen, bis gültige Antwort
** empfangen wurde.
*/
  } while(atoi(reply.first.substr(1,1).c_str())!=group);
  return(reply);
}

//*********

CFtpConnection::ServerReply CFtpConnection::waitForSpecificCmd(int cmd) {
  ServerReply reply;
  do {
    reply=splitReply(m_iconn->receiveLine());
/*
** Keine Server-Antwort?
** => Ausnahme werfen
*/
    if(reply.first=="") {
      CBTRACELN("waitForSpecificCmd: Keine Server-Antwort! => Ausnahme werfen");
      throw internet_error("waitForSpecificCmd: Keine Server-Antwort");
    }

#ifdef CBTR
    if(atoi(reply.first.substr(0,3).c_str())!=cmd)
      CBTRACELN("UEBERLESEN: "+rebuildReply(reply));
#endif

/*
** Einlesen wiederholen, bis gültige Antwort
** empfangen wurde.
*/
  } while(atoi(reply.first.substr(0,3).c_str())!=cmd);
  return(reply);
}

//*********

CFtpConnection::ServerReply CFtpConnection::waitForAnyCmd() {
  ServerReply reply=splitReply(m_iconn->receiveLine());
/*
** Keine Server-Antwort?
** => Ausnahme werfen
*/
  if(reply.first=="") {
    CBTRACELN("waitForAnyCmd: Keine Server-Antwort! => Ausnahme werfen");
    throw internet_error("waitForAnyCmd: Keine Server-Antwort");
  }

  return(reply);
}

//*********

string CFtpConnection::rebuildReply(const CFtpConnection::ServerReply &reply) const {
  return(reply.first+" "+reply.second);
}


//***************************************************************
// convertListEntryToFileInfo
//***************************************************************

CFileInfo CFtpConnection::convertListEntryToFileInfo(const string &line) {
  CBTRACELN(line);
  string::size_type p1=0,p2;
/*
** Zugriffsrechte und Dateityp extrahieren
*/
  while(line[p1]!=' ') p1++;
  string permissions=line.substr(0,p1);
/*
** Bis zur Dateigröße vorlaufen
*/
  while(line[p1]==' ') p1++;
  while(line[p1]!=' ') p1++;
  while(line[p1]==' ') p1++;
  while(line[p1]!=' ') p1++;
  while(line[p1]==' ') p1++;
  while(line[p1]!=' ') p1++;
  while(line[p1]==' ') p1++;
/*
** Dateigröße extrahieren
*/
  p2=p1;
  while(line[p1]!=' ') p1++;
  CHugeNumber filesize(line.substr(p2,p1-p2));
/*
** Bis zum Dateinamen vorlaufen
*/
  while(line[p1]==' ') p1++;
  while(line[p1]!=' ') p1++;
  while(line[p1]==' ') p1++;
  while(line[p1]!=' ') p1++;
  while(line[p1]==' ') p1++;
  while(line[p1]!=' ') p1++;
  while(line[p1]==' ') p1++;
/*
** Dateinamen extrahieren
*/
  string name=line.substr(p1);

  CFileInfo fi;
/*
** Dateityp extrahieren
*/
  if(tolower(permissions[0])=='d')
    fi.m_type=CFileInfo::DIR;
  else if(tolower(permissions[0])=='-')
    fi.m_type=CFileInfo::FILE;
/*
** Informationen in einem CFileInfo-Objekt speichern
*/
  fi.m_name=name;
  fi.m_path=m_currentDir;
  fi.m_size=filesize.getAsFileSize();

#ifdef CBTR
  if(fi.m_type==CFileInfo::DIR)
    cout << "[DIR] ";
  cout << fi.m_name << " " << filesize.getAsPointedNumber() << endl;
#endif

  return(fi);
}


//***************************************************************
// Konstruktor
//***************************************************************

CFtpConnection::CFtpConnection(const string &host, unsigned int port, bool newlogin, int retries)
: m_iconn(0) {

  int trynum=1;
  bool success=false;
  do {
    m_iconn=0;
    CBTRACELN("\nCFtpConnection Konstruktor...\n-----------------------------------------------------");
    CBTRACELN("Verbindungsaufbau (Versuch "+toString(trynum)+")");
/*
** Verbindung mit Server herstellen
*/
    m_iconn=new CInternetConnection(host, port);
/*
** Steht Verbindung?
*/
    if(m_iconn->isAvailable()) {
/*
** Bei neuem Login auf Welcome-Nachricht warten
*/
      if(newlogin) {
        try {
          ServerReply reply=waitForCmdGroup(2);
          CBTRACELN("Empfangen: "+rebuildReply(reply));
          if(reply.first!="220") {
            CBTRACELN("Abbruch! Kein 220-Code bei Anmeldung");
          }
          else
            success=true;
        }
        catch(...) {
          CBTRACELN("Abbruch bei EMpfang der Welcome-Meldung!");

        }
      }
      else
        success=true;
    }
/*
** Verbindung fehlgeschlagen?
** => CInternetConnection-Objekt löschen
*/
    if(!success)
      delete(m_iconn);
  } while((!success)&&(retries>trynum++));

  if(success) {
    CBTRACELN("Erfolg!");
  }
/*
** Alle Versuche durch und immer noch kein Erfolg
*/
  else {
    CBTRACELN("Endgültiger Abbruch!");
    throw internet_error("CFtpConnection::CFtpConnection");
  }
}

CFtpConnection::CFtpConnection(const CUrl &url, bool newlogin, int retries)
: m_iconn(0) {
/*
** Host und Port  aus URL holen
*/
  string host=url.getDomain();
  unsigned int port;
  if(url.getPort()!="")
    port=atol(url.getPort().c_str());
  else
    port=21;

  int trynum=1;
  bool success=false;
  do {
    m_iconn=0;
    CBTRACELN("\nCFtpConnection Konstruktor...\n-----------------------------------------------------");
    CBTRACELN("Verbindungsaufbau (Versuch "+toString(trynum)+")");
/*
** Verbindung mit Server herstellen
*/
    m_iconn=new CInternetConnection(host, port);
/*
** Steht Verbindung?
*/
    if(m_iconn->isAvailable()) {
/*
** Bei neuem Login auf Welcome-Nachricht warten
*/
      if(newlogin) {
        try {
          ServerReply reply=waitForCmdGroup(2);
          CBTRACELN("Empfangen: "+rebuildReply(reply));
          if(reply.first!="220") {
            CBTRACELN("Abbruch! Kein 220-Code bei Anmeldung");
          }
          else
            success=true;
        }
        catch(...) {
          CBTRACELN("Abbruch bei EMpfang der Welcome-Meldung!");

        }
      }
      else
        success=true;
    }
/*
** Verbindung fehlgeschlagen?
** => CInternetConnection-Objekt löschen
*/
    if(!success)
      delete(m_iconn);
  } while((!success)&&(retries>trynum++));

  if(success) {
    CBTRACELN("Erfolg!");
  }
/*
** Alle Versuche durch und immer noch kein Erfolg
*/
  else {
    CBTRACELN("Endgültiger Abbruch!");
    throw internet_error("CFtpConnection::CFtpConnection");
  }
}

CFtpConnection::~CFtpConnection() {
  if(m_iconn)
    delete(m_iconn);
}

//***************************************************************
// Login
//***************************************************************

bool CFtpConnection::login(const string &username, const string &pwd, const string &account) {
  CBTRACELN("\nlogin...\n-----------------------------------------------------");
/*
** Usernamen senden
*/
  m_iconn->sendLineCRLF("USER "+username);
  CBTRACELN("Gesendet : USER "+username);
  ServerReply reply=waitForCmdGroup(3);
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** User ist nicht bekannt o.ä.
** => Abbruch
*/
  if((reply.first[0]=='1')||
     (reply.first[0]=='4')||
     (reply.first[0]=='5')) {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }
/*
** Username allein reicht für Login
** => Erfolg!
*/
  if(reply.first[0]=='2') {
    CBTRACELN("Erfolg!");
    m_currentDir=m_rootDir=getCurrentDir();
    return(true);
  }

/*
** Es wird das Passwort benötigt
*/
  m_iconn->sendLineCRLF("PASS "+pwd);
  CBTRACELN("Gesendet : PASS "+pwd);
  reply=waitForCmdGroup(3);
  CBTRACELN("Empfangen: "+rebuildReply(reply));

/*
** User oder Pwd inkorrekt
** => Abbruch
*/
  if((reply.first[0]=='1')||
     (reply.first[0]=='4')||
     (reply.first[0]=='5')) {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }
/*
** Passwort gültig und ausreichend
** => Erfolg!
*/
  if(reply.first[0]=='2') {
    CBTRACELN("Erfolg!");
    m_currentDir=m_rootDir=getCurrentDir();
    return(true);
  }

/*
** Es wird ein Account benötigt
*/
  m_iconn->sendLineCRLF("ACCT "+account);
  CBTRACELN("Gesendet : ACCT "+account);
  reply=waitForCmdGroup(3);
  CBTRACELN("Empfangen: "+rebuildReply(reply));

/*
** Account o.k.
** => Erfolg!
*/
  if(reply.first[0]=='2') {
    CBTRACELN("Erfolg!");
    m_currentDir=m_rootDir=getCurrentDir();
    return(true);
  }

/*
** Login fehlgeschlagen
*/
  m_lastError=rebuildReply(reply);
  CBTRACELN("Abbruch!");
  return(false);
}

bool CFtpConnection::login(const CUrl &url, const string &account) {
/*
** Username und Passwort aus URL holen
*/
  string username=url.getUsername();
  if(username=="")
    username=AnonymousName;

  string password=url.getPassword();
  if(password=="")
    password=AnonymousPwd;
/*
** Echte login-Methode aufrufen
*/
  return(login(username,password,account));

}


//***************************************************************
// getCurrentDir
//***************************************************************

string CFtpConnection::getCurrentDir() {
  CBTRACELN("\ngetCurrentDir...\n-----------------------------------------------------");
/*
** Befehl senden
*/
  m_iconn->sendLineCRLF("PWD");
  CBTRACELN("Gesendet : PWD");
/*
** Auf spezielle Antwort (Code 257) auf
** PWD warten
*/
  ServerReply reply=waitForSpecificCmd(257);
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Das Verzeichnis steht im Antwort-Text zwischen
** zwei doppelten Anführungszeichen
*/
  string::size_type beg=reply.second.find("\"")+1;
  string::size_type len=reply.second.find("\"",beg)-beg;
  string dir=reply.second.substr(beg,len);
  CBTRACELN("Extrahiertes Verzeichnis: "+dir);

  return(dir);
}

//***************************************************************
// setType
//***************************************************************

bool CFtpConnection::setType(CFtpConnection::FtpType type) {
  CBTRACELN("\nsetType...\n-----------------------------------------------------");
/*
** Type senden
*/
  m_iconn->sendLineCRLF(static_cast<string>("TYPE ")+static_cast<char>(type));
  CBTRACELN(static_cast<string>("Gesendet : TYPE ")+static_cast<char>(type));
  ServerReply reply=waitForCmdGroup(0);
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Type erfolgereich gesetzt?
** => Erfolg!
*/
  if(reply.first[0]=='2') {
    CBTRACELN("Erfolg!");
    return(true);
  }

/*
** setType fehlgeschlagen
*/
  m_lastError=rebuildReply(reply);
  CBTRACELN("Abbruch!");
  return(false);
}

//***************************************************************
// setPort
//***************************************************************

/*
bool CFtpConnection::setPort() {
  CBTRACELN("\nsetPort...\n-----------------------------------------------------");

  string addr=m_iconn->getAddress();
  unsigned int port=m_iconn->getPort();
  CBTRACELN("Adresse:"+addr+" Port: "+toString(port));

  for(string::iterator siter=addr.begin(); siter!=addr.end(); ++siter)
    if(*siter=='.')
      *siter=',';

  addr+=","+toString(port/256)+","+toString(port%256);


  m_iconn->sendLineCRLF("PORT "+addr);
  CBTRACELN("Gesendet : PORT "+addr);
  ServerReply reply=splitReply(m_iconn->receiveLine());
  CBTRACELN("Empfangen: "+rebuildReply(reply));

  if(reply.first[0]=='2') {
    CBTRACELN("Erfolg!");
    return(true);
  }


  setLastError(rebuildReply(reply));
  CBTRACELN("Abbruch!");
  return(false);
}
*/

//***************************************************************
// setPassive
//***************************************************************

CFtpConnection::Address CFtpConnection::setPassive() {
  CBTRACELN("\nsetPassive...\n-----------------------------------------------------");

/*
** Passive-Befehl senden
*/
  m_iconn->sendLineCRLF("PASV");
  CBTRACELN("Gesendet : PASV");
  ServerReply reply=waitForCmdGroup(2);
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Passive-Modus nicht eingestellt?
** => Abbruch!
*/
  if(reply.first[0]!='2') {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    throw internet_error("CFtpConnection::setPassive: "+rebuildReply(reply));
  }

/*
** Adresse und Port extrahieren
*/
  Address addr;
  string info=reply.second.substr(reply.second.find("(")+1);
  info=info.substr(0,info.find(")"))+",";
  CBTRACELN("Extrahierte Information: "+info);

  int vals[6];
  string::size_type beg=0,end=info.find(",");
  for(int i=0;i<6;i++) {
    vals[i]=atoi(info.substr(beg,end-beg+1).c_str());
    beg=end+1;
    end=info.find(",",beg);
  }
/*
** Adresse und Port in "nutzbares" Format übertragen
*/
  addr.first=toString(vals[0])+"."+
             toString(vals[1])+"."+
             toString(vals[2])+"."+
             toString(vals[3]);
  addr.second=vals[4]*256+vals[5];
  CBTRACELN("Passive Adresse: "+addr.first);
  CBTRACELN("Passiver Port  : "+toString(addr.second));

  CBTRACELN("Erfolg!");
  return(addr);
}


//***************************************************************
// list
//***************************************************************

bool CFtpConnection::list(CFileInfo::filelist_type &filelist) {
/*
** Type auf Text setzen
*/
  setType(A);
/*
** In den passiven Modus wechseln
*/
  Address paddr=setPassive();
  CBTRACELN("\nlist...\n-----------------------------------------------------");

/*
** List-Befehl senden
*/
  m_iconn->sendLineCRLF("LIST");
  CBTRACELN("Gesendet : LIST");
  CFtpConnection receiver(paddr.first, paddr.second,false,10);
  ServerReply reply=waitForAnyCmd();
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Wird keine Datei-Liste gesendet?
** => Abbruch!
*/
  if(reply.first[0]!='1') {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }
/*
** Dateiliste einlesen
*/
  string line;
  while((line=receiver.m_iconn->receiveLine())!="") {
    filelist.push_back(convertListEntryToFileInfo(line));
  }

  reply=waitForAnyCmd();
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Übertragung nicht korrekt beendet?
** => Abbruch!
*/
  if(reply.first[0]!='2') {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }

  CBTRACELN("Erfolg!");
  getCurrentDir();
  return(true);
}

//***************************************************************
// changeDir
//***************************************************************
// path ohne / am Ende

bool CFtpConnection::changeDir(const string &path) {
  CBTRACELN("\nchangeDir...\n-----------------------------------------------------");
/*
** CWD-Befehl senden
*/
  m_iconn->sendLineCRLF("CWD "+path);
  CBTRACELN("Gesendet : CWD "+path);
  ServerReply reply=waitForCmdGroup(5);
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Informiert Status-Code nicht über Erfolg?
** => Abbruch
*/
  if(reply.first[0]!='2') {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }
/*
** Zur Bestätigung aktuelles Verzeichnis holen
*/
  CBTRACELN("Erfolg!");
  m_currentDir=getCurrentDir();
  return(true);
}


//***************************************************************
// retrieveFile mit string als Quelle
//***************************************************************
// serverFile ist nur ein Dateiname und wird im aktuellen FTP-Verzeichnis vermutet

bool CFtpConnection::retrieveFile(const string &serverFile, const string &localFile) {
/*
** Übertragungsart auf Binär setzen
*/
  setType(I);
/*
** In den passiven Modus wechseln
*/
  Address paddr=setPassive();
  CBTRACELN("\nretrieveFile...\n-----------------------------------------------------");
/*
** Lokale Datei zum Schreiben öffnen
*/
  CFile file;
  if(!file.open(localFile,true,true)) {
    CBTRACELN("FEHLER! Lokale Datei konnte nicht zum Schreiben geoeffnet werden!");
    return(false);
  }

/*
** Retrieve-Befehl senden
*/
  m_iconn->sendLineCRLF("RETR "+serverFile);
  CBTRACELN("Gesendet : RETR "+serverFile);
  CFtpConnection receiver(paddr.first, paddr.second,false);
  ServerReply reply=waitForAnyCmd();
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Werden keine Daten gesendet?
** => Abbruch!
*/
  if(reply.first[0]!='1') {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }
  CMemory<char> buf(10000);
  unsigned int amount;
  CHugeNumber totallyRead;
/*
** Daten empfangen, bis Server die
** Datenverbindung abbricht
*/
  do {
    amount=receiver.m_iconn->receive(buf,10000);
    file.writeBlock(buf,amount);
    totallyRead+=static_cast<unsigned long>(amount);
    CBTRACE("Bisher uebertragen: "+totallyRead.getAsPointedNumber()+" Bytes\r");
  } while(amount);

  CBTRACELN("");
  reply=waitForCmdGroup(2);
  CBTRACELN("Empfangen: "+rebuildReply(reply));
/*
** Übertragung nicht korrekt beendet?
** => Abbruch!
*/
  if(reply.first[0]!='2') {
    m_lastError=rebuildReply(reply);
    CBTRACELN("Abbruch!");
    return(false);
  }

  CBTRACELN("Erfolg!");
  getCurrentDir();
  return(true);

}

//***************************************************************
// statisches retrieveFile mit CUrl als Quelle
//***************************************************************
// localpath muss absoluter Pfad sein ohne / am Ende

bool CFtpConnection::retrieveFile(const CUrl &url, const string &localPath) {
  CBTRACELN("\nretrieveFile...\n-----------------------------------------------------");
/*
** Pfade und Dateinamen extrahieren
*/
  string serverPath=url.getPath();
  if(serverPath=="") {
    CBTRACELN("Abbruch! (kein gueltiger Pfad)");
    return(false);
  }
  string localFile=localPath;
  string fileName;
  string::size_type spos=serverPath.rfind("/");
  if(spos!=string::npos) {
    fileName=serverPath.substr(spos+1);
    localFile+="/"+fileName;
    serverPath="/"+serverPath.substr(0,spos+1);
  }
  else {
    fileName=serverPath;
    localFile+="/"+fileName;
    serverPath="/";
  }

  CBTRACELN("Dateiname   : "+fileName);
  CBTRACELN("FTP-Pfad    : "+serverPath);
  CBTRACELN("Lokaler Pfad: "+localFile);
/*
** Port aus CUrl-Objekt extrahieren. Wenn keiner
** vorhanden, dann Port 21 nehmen
*/
  unsigned int port;
  if(url.getPort()!="")
    port=atol(url.getPort().c_str());
  else
    port=21;
/*
** Usernamen aus CUrl-Objekt extrahieren. Ist keiner vorhanden
** wird der in AnonymousName definierte verwendet
*/
  string username=url.getUsername();
  if(username=="")
    username=AnonymousName;
/*
** Passwort aus CUrl-Objekt extrahieren. Ist keins vorhanden
** wird das in AnonymousPwd definierte verwendet
*/
  string password=url.getPassword();
  if(password=="")
    password=AnonymousPwd;

  try {
/*
** Verbindungsaufbau zu Server
*/
    CFtpConnection ftp(url.getDomain(), port);
/*
** Einloggen
*/
    if(!ftp.login(username, password)) {
      CBTRACELN("RetrieveFile: Login fehlgeschlagen");
      return(false);
    }
/*
** In das Verzeichnis der gewünschten Ressource wechseln
*/
    if(!ftp.changeDir(serverPath)) {
      CBTRACELN("RetrieveFile: Verzeichniswechsel fehlgeschlagen");
      return(false);
    }
/*
** Datei empfangen
*/
    if(!ftp.retrieveFile(fileName,localFile)) {
      CBTRACELN("RetrieveFile: Dateiuebertragung fehlgeschlagen");
      return(false);
    }
  }
  catch(internet_error) {
    CBTRACELN("RetrieveFile fehlgeschlagen (Exception!)");
    return(false);
  }

  CBTRACELN("retrieveFile Erfolg!");
  return(true);
}


//***************************************************************
// getDir mit CUrl als Quelle
//***************************************************************

bool CFtpConnection::getDir(const CUrl &url, CFileInfo::filelist_type &files) {
  CBTRACELN("\ngetDir...\n-----------------------------------------------------");
  string serverPath="/"+url.getPath();

  CBTRACELN("FTP-Pfad    : "+serverPath);

/*
** Port aus CUrl-Objekt extrahieren. Wenn keiner
** vorhanden, dann Port 21 nehmen
*/
  unsigned int port;
  if(url.getPort()!="")
    port=atol(url.getPort().c_str());
  else
    port=21;

/*
** Usernamen aus CUrl-Objekt extrahieren. Ist keiner vorhanden
** wird der in AnonymousName definierte verwendet
*/
  string username=url.getUsername();
  if(username=="")
    username=AnonymousName;

/*
** Passwort aus CUrl-Objekt extrahieren. Ist keins vorhanden
** wird das in AnonymousPwd definierte verwendet
*/
  string password=url.getPassword();
  if(password=="")
    password=AnonymousPwd;

  try {
/*
** Verbindungsaufbau zu Server
*/
    CFtpConnection ftp(url.getDomain(),port);
/*
** Einloggen
*/
    if(!ftp.login(username, password)) {
      CBTRACELN("getDir: Login fehlgeschlagen");
      return(false);
    }
/*
** In das auszulesende Verzeichnis wechseln
*/
    if(!ftp.changeDir(serverPath)) {
      CBTRACELN("getDir: Verzeichniswechsel fehlgeschlagen");
      return(false);
    }
/*
** Verzeichnis auslesen
*/
    if(!ftp.list(files)) {
      CBTRACELN("getDir: list fehlgeschlagen");
      return(false);
    }
  }
  catch(internet_error) {
    CBTRACELN("getDir fehlgeschlagen (Exception!)");
    return(false);
  }

  CBTRACELN("getDir Erfolg!");
  return(true);
}

//***************************************************************
// statische getCompleteDir mit CUrl als Quelle
//***************************************************************
// Bei Rootverzeichnis sind m_path und m_name leer

bool CFtpConnection::getCompleteDir(const CUrl &url, CFileInfo &fi) {
  CBTRACELN("\ngetCompleteDir...\n-----------------------------------------------------");
/*
** Auszulesendes Verzeichnis bestimmen
*/
  string serverPath="/"+url.getPath();
  if((fi.m_type==CFileInfo::INVALID)&&(serverPath.size()>1)) {
    string::size_type spos=serverPath.rfind("/");
    fi.m_path=serverPath.substr(0,spos);
    fi.m_name=serverPath.substr(spos+1);
  }

  CBTRACELN("Pfad: \""+fi.m_path+"\" Name: \""+fi.m_name+"\"");
  CBTRACELN("FTP-Pfad    : "+serverPath);

/*
** Port aus CUrl-Objekt extrahieren. Wenn keiner
** vorhanden, dann Port 21 nehmen
*/
  unsigned int port;
  if(url.getPort()!="")
    port=atol(url.getPort().c_str());
  else
    port=21;

/*
** Usernamen aus CUrl-Objekt extrahieren. Ist keiner vorhanden
** wird der in AnonymousName definierte verwendet
*/
  string username=url.getUsername();
  if(username=="")
    username=AnonymousName;

/*
** Passwort aus CUrl-Objekt extrahieren. Ist keins vorhanden
** wird das in AnonymousPwd definierte verwendet
*/
  string password=url.getPassword();
  if(password=="")
    password=AnonymousPwd;

  try {
/*
** Verbindungsaufbau zu Server
*/
    CFtpConnection ftp(url.getDomain(),port);
/*
** Einloggen
*/
    if(!ftp.login(username, password)) {
      CBTRACELN("SgetCompleteDir: Login fehlgeschlagen");
      return(false);
    }
/*
** In das auszulesende Verzeichnis wechseln
*/
    if(!ftp.changeDir(serverPath)) {
      CBTRACELN("SgetCompleteDir: Verzeichniswechsel fehlgeschlagen");
      return(false);
    }

    if(fi.m_type==CFileInfo::INVALID)
      fi.m_type=CFileInfo::DIR;

    fi.m_filelist=new CFileInfo::filelist_type;
/*
** Aktuelles  Verzeichnis einlesen
*/
    if(!ftp.list(*fi.m_filelist)) {
      CBTRACELN("SgetCompleteDir: list fehlgeschlagen");
      return(false);
    }
/*
** Einträge sortieren
*/
    sortFileList(*fi.m_filelist);
/*
** Alle Einträge durchlaufen und für Verzeichnisse
** die nicht-statische getCompleteDir-Methode aufrufen
*/
    for(CFileInfo::filelist_type::iterator iter=fi.m_filelist->begin(); iter!=fi.m_filelist->end(); ++iter) {
      if(iter->m_type==CFileInfo::DIR) {
        CBTRACELN("Rekursiver Aufruf fuer: "+serverPath+"/"+iter->m_name);
        ftp.getCompleteDir(serverPath+"/"+iter->m_name, *iter);
      }
    }
/*
** Für aktuelles Verzeichnis die Größe bestimmmen
*/
    fi.m_size=CFileSize();
    for(iter=fi.m_filelist->begin(); iter!=fi.m_filelist->end(); ++iter)
      fi.m_size+=iter->m_size;

  }
  catch(internet_error) {
    CBTRACELN("SgetCompleteDir fehlgeschlagen (Exception!)");
    return(false);
  }

  CBTRACELN("SgetCompleteDir Erfolg!");
  return(true);
}


//***************************************************************
// getCompleteDir mit String als Verzeichnis
//***************************************************************

bool CFtpConnection::getCompleteDir(const std::string &dir, CFileInfo &fi) {

  try {
/*
** In das auszulesende Unterverzeichnis wechseln
*/
    if(!changeDir(dir)) {
      CBTRACELN("getCompleteDir: Verzeichniswechsel fehlgeschlagen");
      return(false);
    }

    fi.m_filelist=new CFileInfo::filelist_type;

/*
** Aktuelles  Verzeichnis einlesen
*/
    if(!list(*fi.m_filelist)) {
      CBTRACELN("getCompleteDir: list fehlgeschlagen");
      return(false);
    }
/*
** Einträge sortieren
*/
    sortFileList(*fi.m_filelist);
/*
** Alle Einträge durchlaufen und für Verzeichnisse
** die nicht-statische getCompleteDir-Methode aufrufen
*/
    for(CFileInfo::filelist_type::iterator iter=fi.m_filelist->begin(); iter!=fi.m_filelist->end(); ++iter) {
      if(iter->m_type==CFileInfo::DIR) {
        CBTRACELN("Rekursiver Aufruf fuer: "+dir+"/"+iter->m_name);
        getCompleteDir(dir+"/"+iter->m_name, *iter);
      }
    }
/*
** Für aktuelles Verzeichnis die Größe bestimmmen
*/
    fi.m_size=CFileSize();
    for(iter=fi.m_filelist->begin(); iter!=fi.m_filelist->end(); ++iter)
      fi.m_size+=iter->m_size;

  }
  catch(internet_error) {
    CBTRACELN("getCompleteDir fehlgeschlagen (Exception!)");
    return(false);
  }

  CBTRACELN("getCompleteDir Erfolg!");
  return(true);
}



}; // namespace Codebook

