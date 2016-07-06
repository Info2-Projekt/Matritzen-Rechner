#define CBTR // Definieren, für Trace-Ausgaben
#ifdef CBTR
#include <iostream>
#define CBTRACELN(s) std::cout << (s) << endl;
#define CBTRACE(s) std::cout << (s);
#else
#define CBTRACELN(s)
#define CBTRACE(s)
#endif

#include "CInternetConnection.h"
#include "StringFunctions.h"
#include "CMemory.h"

using namespace std;

namespace Codebook {

const char CInternetConnection::EOL='\n';
const char CInternetConnection::CR='\r';
const std::string CInternetConnection::CRLF="\r\n";


CInternetConnection::CInternetConnection(const string &host, unsigned int port)
: CSpecificINetConn(host,port)
{ }


//*********

string CInternetConnection::receiveLine(){
  string line;
  char c;
/*
** Schleife läuft, bis EOL (LF) empfangen wurde
*/
  do {
/*
** Kein Zeichen mehr empfangen?
** => Abbruch und bisher empfangene Zeichen zurückliefern
*/
    if(!receive(&c,1))
      return(line);
/*
** CR und LF (EOL) überlesen
*/
    if((c!=CR)&&(c!=EOL))
      line+=c;
  } while (c!=EOL);

  return(line);
}

//*********

int CInternetConnection::sendLine(const std::string &line) {
  return(send(line.c_str(), line.size()));
}

//*********

int CInternetConnection::sendLineCRLF(const std::string &line) {
  return(sendLine(line+CRLF));
}




}; // namespace Codebook

