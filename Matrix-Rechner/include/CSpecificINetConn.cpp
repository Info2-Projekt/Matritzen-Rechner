#define CBTR // Definieren für Trace-Ausgaben
#ifdef CBTR
#include <iostream>
#define CBTRACELN(s) std::cout << (s) << endl;
#define CBTRACE(s) std::cout << (s);
#else
#define CBTRACELN(s)
#define CBTRACE(s)
#endif

#include "CSpecificINetConn.h"
#include "StringFunctions.h"
#include "CMemory.h"

using namespace std;

namespace Codebook {

WSADATA CSpecificINetConn::m_wsaData={0,0,0,0,0,0,0};

void CSpecificINetConn::initialize() {
/*
** Microsoft Foundation Classes initialisieren
*/
  if (!AfxWinInit(GetModuleHandle(NULL), NULL, GetCommandLine(), 0))
    throw internet_error("CSpecificINetConn::initialise: MFC-Initialisierung fehlgeschlagen "+toString(GetLastError()));

/*
** Winsock initialisieren
*/
  if(WSAStartup(MAKEWORD( 2, 0 ), &m_wsaData) != 0)
    throw internet_error("CSpecificINetConn::initialise: Winsock 2.0-Initialisierung fehlgeschlagen "+toString(GetLastError()));
}

/*
** Alle Ausnahmen werden aufgefagen, m_available gibt Auskunft
** über erfolgreiche Konstruktion
*/

CSpecificINetConn::CSpecificINetConn(const string &host, unsigned int port)
: m_socket(0), m_sfile(0), m_archive(0), m_available(false) {
  try {
/*
** Socket anlegen und interne Verbindungen herstellen
*/
    m_socket=new CSocket();
    if(m_socket->Create()) {
      CBTRACELN("Socket kreiert!");
/*
** Socketfile und Archiv für den Datenempfang anlegen
** und mit dem Socket verbinden
*/
      m_sfile=new CSocketFile(m_socket);
      m_archive=new CArchive(m_sfile,CArchive::load,10000);
      CBTRACELN("CSocketFile und CArchive konstruiert!");
/*
** Verbindung mit Server, dessen Namen in host gespeichert ist,
** über Port port aufnehmen
*/
      if(m_socket->Connect(host.c_str(),port)) {
        CBTRACELN("Socket verbunden mit "+host+" Port "+toString(port));
        m_available=true;
      }
    }
  }
  catch(...) {
    CBTRACELN("Ausnahme in CSpecificINetConn-Konstruktor");
  }
}

CSpecificINetConn::~CSpecificINetConn() {
  if(m_archive)
    delete(m_archive);
  if(m_sfile)
    delete(m_sfile);
  if(m_socket)
    delete(m_socket);
}

/*
** Alle Ausnahmen aufgefangen
** Bei Fehler res=0
*/

int CSpecificINetConn::receive(void *buffer, int bsize){
  int res=0;
  try {
    res=m_archive->Read(buffer,bsize);
  }
  catch(...) {
    CBTRACELN("Ausnahme in CSpecificINetConn::receive");
  }
  return(res);
}

/*
** Alle Ausnahmen aufgefangen
** Bei Fehler res=0
*/
int CSpecificINetConn::send(const void *buffer, int bsize){

  int res=0;
  try {
    res=m_socket->Send(buffer,bsize);
  }
  catch(...) {
    CBTRACELN("Ausnahme in CSpecificINetConn::send");
  }
  if(res==SOCKET_ERROR) {
    CBTRACELN("SOCKET_ERROR in CSpecificINetConn::send "+toString(m_socket->GetLastError()));
    return(0);
  }
  return(res);
}

}; // namespace Codebook

