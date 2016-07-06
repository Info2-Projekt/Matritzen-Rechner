#define CBTR // Definieren, für Trace-Ausgaben
#ifdef CBTR
#include <iostream>
#define CBTRACELN(s) std::cout << (s) << endl;
#define CBTRACE(s) std::cout << (s);
#else
#define CBTRACELN(s)
#define CBTRACE(s)
#endif




#include "CHttpConnection.h"
#include "CFile.h"
#include "CMemory.h"

#include <vector>


using namespace std;

namespace Codebook {

CHttpConnection::CHttpConnection()
: m_iconn(0)
{ }

bool CHttpConnection::connect(const std::string &host, unsigned int port) {
  CBTRACELN("CHttpConnection::connect: Verbindung wird aufgebaut");
/*
** Eventuell bestehende Verbindung abbrechen
*/
  disconnect();
/*
** Internet-Verbindung mit entsprechender
** Adresse und Port herstellen
*/
  m_iconn=new CInternetConnection(host,port);
  if(m_iconn->isAvailable()) {
    CBTRACELN("Erfolg!");
    return(true);
  }
/*
** Verbindungsaufbau fehlgeschlagen?
** => CInternetConnection-Objekt löschen
*/
  else {
    m_lastError="Verbindungaufbau fehlgeschlagen!";
    CBTRACELN("Verbindungaufbau fehlgeschlagen!");
    delete(m_iconn);
    m_iconn=0;
    return(false);
  }
}

bool CHttpConnection::connect(const CUrl &url) {
/*
** Host und Port aus CUrl holen
*/
  string host=url.getDomain();
  string sport=url.getPort();
  unsigned int port;

  if(sport=="")
    port=80;
  else
    port=atol(sport.c_str());
/*
** connect für Host und Port aufrufen
*/
  return(connect(host,port));
}

void CHttpConnection::disconnect() {
  if(m_iconn) {
    delete(m_iconn);
    m_iconn=0;
  }
}


//***************************************************************
// sendRequest
//***************************************************************

bool CHttpConnection::sendRequest(const CHttpRequest &request) {
  CBTRACELN("\nsendRequest\n-----------------------------------------------------");
/*
** Request senden
*/
  string sreq=request.getRequest();
  CBTRACE(sreq);
  unsigned int sent=m_iconn->sendLine(sreq);
/*
** Sind weniger Zeichen gesendet worden als
** der Request lang ist?
** => Fehler!
*/
  if(sent<sreq.size()) {
    m_lastError="sendRequest fehlgeschlagen!";
    CBTRACELN("sendRequest fehlgeschlagen!");
    return(false);
  }
  else {
    CBTRACELN("Erfolg!");
    return(true);
  }
}

//***************************************************************
// receiveResponse
//***************************************************************

bool CHttpConnection::receiveResponse(vector<string> &response) {
  CBTRACELN("\nreceiveResponse\n-----------------------------------------------------");
  string line;
/*
** Server-Response zeilenweise einlesen
*/
  while((line=m_iconn->receiveLine())!="") {
    CBTRACELN("Empfangen: "+line);
    response.push_back(line);
  }
  if(response.size()==0)
    return(false);
  else
    return(true);
}


//***************************************************************
// getHeadHeader
//***************************************************************

bool CHttpConnection::getHeadHeader(const CUrl &url, vector<string> &header) {
  CBTRACELN("\ngetHeadHeader\n-----------------------------------------------------");
/*
** Head-Request formulieren
*/
  CHttpRequest request;
  request.setFromUrl(url);
  request.setRequestType(CHttpRequest::rtHEAD);
  request.setUserAgent(CHttpRequest::UAExplorer6);
/*
** Verbindung aufbauen
*/
  if(!connect(url)) {
    m_lastError="Fehler bei connect";
    CBTRACELN("Fehler bei connect");
    return(false);
  }
/*
** Request senden
*/
  if(!sendRequest(request)) {
    m_lastError="Fehler bei sendRequest";
    CBTRACELN("Fehler bei sendRequest");
    return(false);
  }
/*
** Antwort empfangen
*/
  if(!receiveResponse(header)) {
    m_lastError="Fehler bei receiveResponse";
    CBTRACELN("Fehler bei receiveResponse");
    return(false);
  }

  CBTRACELN("Erfolg!");
  return(true);
}

bool CHttpConnection::getHeadHeader(const CUrl &url, CHttpResponse &header) {
  vector<string> vr;
  if(!getHeadHeader(url, vr))
    return(false);
  header.extract(vr);
  return(true);
}


//***************************************************************
// getTraceHeader
//***************************************************************

bool CHttpConnection::getTraceHeader(const CUrl &url, vector<string> &header, int maxforwards) {
  CBTRACELN("\ngetTraceHeader\n-----------------------------------------------------");
/*
** Head-Request formulieren
*/
  CHttpRequest request;
  request.setFromUrl(url);
  request.setRequestType(CHttpRequest::rtTRACE);
  request.setUserAgent(CHttpRequest::UAExplorer6);
  request.setMaxForwards(maxforwards);
/*
** Verbindung aufbauen
*/
  if(!connect(url)) {
    m_lastError="Fehler bei connect";
    CBTRACELN("Fehler bei connect");
    return(false);
  }
/*
** Request senden
*/
  if(!sendRequest(request)) {
    m_lastError="Fehler bei sendRequest";
    CBTRACELN("Fehler bei sendRequest");
    return(false);
  }
/*
** Antwort empfangen
*/
  if(!receiveResponse(header)) {
    m_lastError="Fehler bei receiveResponse";
    CBTRACELN("Fehler bei receiveResponse");
    return(false);
  }

  CBTRACELN("Erfolg!");
  return(true);
}

bool CHttpConnection::getTraceHeader(const CUrl &url, CHttpResponse &header, int maxforwards) {
  vector<string> vr;
  if(!getTraceHeader(url, vr, maxforwards))
    return(false);
  header.extract(vr);
  return(true);
}


//***************************************************************
// getFile
//***************************************************************

bool CHttpConnection::getFile(const CUrl &url, const string &filename) {
  CHttpRequest request;
  request.setFromUrl(url);
  request.setUserAgent(CHttpRequest::UAExplorer6);

  return(getFile(url,filename,request));
}


//***************************************************************
// getFile
//***************************************************************

bool CHttpConnection::getFile(const CUrl &url, const string &filename, const CHttpRequest &request) {
  CBTRACELN("\ngetFile\n-----------------------------------------------------");

/*
** Verbindung zum Server herstellen
*/
  if(!connect(url)) {
    m_lastError="Fehler bei connect";
    CBTRACELN("Fehler bei connect");
    return(false);
  }
/*
** Request senden
*/
  if(!sendRequest(request)) {
    m_lastError="Fehler bei sendRequest";
    CBTRACELN("Fehler bei sendRequest");
    return(false);
  }
/*
** Server-Response empfangen
*/
  vector<string> vresponse;
  if(!receiveResponse(vresponse)) {
    m_lastError="Fehler bei receiveResponse";
    CBTRACELN("Fehler bei receiveResponse");
    return(false);
  }
/*
** Response in ein CHttpResponse-Objekt umwandeln
*/
  bool success;
  CHttpResponse response;
  response.extract(vresponse);
  int code=response.getStatusCode();
/*
** Status zwischen 301 und 307?
** => Ressource wurde verschoben. Neue Location aus
** Response holen und getFile neu aufrufen
*/
  if((code>=301)&&(code<=307)&&(response.getLocation()!="")) {
    CBTRACELN("\nRessource verschoben. Neue Location bearbeiten...");
    disconnect();
    return(getFile(CUrl(response.getLocation()), filename));
  }
/*
** Alle Status-Codes außerhalb des 200er-Bereichs
** als Fehler betrachten
*/
  else if((response.getStatusCode()<200)||(response.getStatusCode()>=300)) {
    m_lastError=toString(response.getStatusCode())+" "+response.getStatusText();
    CBTRACELN("Server meldet Fehler");
    return(false);
  }
  else {
/*
** Je nachdem, ob Transfer an einem Stück oder
** in Chunks benötigt wird, die entsprechende
** Methode aufrufen
*/
    if(!response.isTransferChunked())
      success=normalFileTransfer(filename, response);
    else {
      success=chunkedFileTransfer(filename, response);
/*
** Hat ein Transfer in Chunks stattgefunden?
** => Hinter den Chunks nach eventuellen Entity-Headern
** Ausschau halten
*/
      if(success) {
        vresponse.clear();
        receiveResponse(vresponse);
        response.extract(vresponse,false);
      }
    }
  }
/*
** Komplette Server-Antwort im Objekt speichern
*/
  m_lastResponse=response;
  return(success);
}


//***************************************************************
// normalFileTransfer
//***************************************************************

bool CHttpConnection::normalFileTransfer(const std::string &filename, const CHttpResponse &response) {
/*
** Datei auf lokalem Datenträger zum Schreiben öffnen
*/
  CFile file;
  if(!file.open(filename,true,true)) {
    m_lastError="FEHLER! Lokale Datei konnte nicht zum Schreiben geoeffnet werden!";
    CBTRACELN("FEHLER! Lokale Datei konnte nicht zum Schreiben geoeffnet werden!");
    return(false);
  }
  CMemory<char> buf(10000);
  unsigned int amount;
  CHugeNumber totallyRead, bufsize("10000");

/*
** Wenn Content-Length verfügbar, dann werden abgezählte
** Daten empfangen
*/
  if(response.isContenLengthAvailable()) {
    CHugeNumber length=response.getContentLength();
    CBTRACELN("Abgezaehlte Daten ("+length.getAsPointedNumber()+") werden gelesen!");
    do {
/*
** Darauf achten, dass nicht mehr Daten empfangen
** werden sollen als benötigt werden (weil read sonst
** blockiert.)
*/
      if(length>=bufsize)
        amount=m_iconn->receive(buf,10000);
      else
        amount=m_iconn->receive(buf,length.getAsUnsignedLong());

      file.writeBlock(buf,amount);
      totallyRead+=static_cast<unsigned long>(amount);
      length-=static_cast<unsigned long>(amount);
      CBTRACE("Bisher uebertragen: "+totallyRead.getAsPointedNumber()+" Bytes\r");
/*
** Keine Daten mehr empfangen, obwohl noch nicht alle Daten
** angekommen sind?
** => Fehler!
*/
      if((totallyRead==0)&&(length!=0)) {
        m_lastError="chunkedTransfer: Übertragung abgebrochen";
        CBTRACE("chunkedTransfer: Übertragung abgebrochen");
        return(false);
      }
    } while(length!=0);
  }
/*
** Wenn keine Content-Length verfügbar, dann Daten
** empfangen, bis der Server die Verbindung trennt
*/
  else {
    CBTRACELN("Daten lesen bis zum Ende!");
    do {
      amount=m_iconn->receive(buf,10000);
      file.writeBlock(buf,amount);
      totallyRead+=static_cast<unsigned long>(amount);
      CBTRACE("Bisher uebertragen: "+totallyRead.getAsPointedNumber()+" Bytes\r");
    } while(amount);
  }

  CBTRACELN("\nErfolg!");
  return(true);
}


//***************************************************************
// chunkedFileTransfer
//***************************************************************

bool CHttpConnection::chunkedFileTransfer(const std::string &filename, const CHttpResponse &response) {
  CBTRACELN("Uebertragung in Chunks beginnt...");
/*
** Datei auf lokalem Datenträger zum Schreiben öffnen
*/
  CFile file;
  if(!file.open(filename,true,true)) {
    m_lastError="FEHLER! Lokale Datei konnte nicht zum Schreiben geoeffnet werden!";
    CBTRACELN("FEHLER! Lokale Datei konnte nicht zum Schreiben geoeffnet werden!");
    return(false);
  }
  CMemory<char> buf(10000);
  unsigned int amount;
  CHugeNumber totallyRead, bufsize("10000");
/*
** Größe des ersten Chunks einlesen
*/
  CHugeNumber length=hexToValue(m_iconn->receiveLine());
/*
** Solange eingelesene Chunk-Größe != 0
*/
  while(length!=0) {
    CBTRACELN("Chungroesse : "+length.getAsPointedNumber()+" Bytes");
    do {
/*
** Darauf achten, dass nicht mehr Daten empfangen
** werden als Chunk groß ist
*/
      if(length>=bufsize)
        amount=m_iconn->receive(buf,10000);
      else
        amount=m_iconn->receive(buf,length.getAsUnsignedLong());

      file.writeBlock(buf,amount);
      totallyRead+=static_cast<unsigned long>(amount);
      length-=static_cast<unsigned long>(amount);
      CBTRACE("Bisher uebertragen: "+totallyRead.getAsPointedNumber()+" Bytes\r");
      if((totallyRead==0)&&(length!=0)) {
        m_lastError="chunkedTransfer: Übertragung abgebrochen";
        CBTRACE("chunkedTransfer: Übertragung abgebrochen");
        return(false);
      }

    } while(length!=0);

    CBTRACELN("");
/*
** CRLF hinter Chunk-Datn einlesen
*/
    m_iconn->receiveLine();
/*
** Größe des nächsten Chunks einlesen
*/
    length=hexToValue(m_iconn->receiveLine());
  }

  CBTRACELN("\nErfolg!");
  return(true);
}

}; // namespace Codebook

