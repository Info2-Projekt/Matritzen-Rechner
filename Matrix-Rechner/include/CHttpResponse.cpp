#define CBTR // Definieren, für Trace-Ausgaben
#ifdef CBTR
#include <iostream>
#define CBTRACELN(s) std::cout << (s) << endl;
#define CBTRACE(s) std::cout << (s);
#else
#define CBTRACELN(s)
#define CBTRACE(s)
#endif

#include "CHttpResponse.h"
#include "StringFunctions.h"

//#include <string>

//#include "StringFunctions.h"
//#include "CHugeNumber.h"
//#include "FileFunctions.h"
//#include "CFile.h"
//#include "CMemory.h"

using namespace std;

namespace Codebook {

void CHttpResponse::extract(const vector<string> &response, bool withHead) {
  CBTRACELN("\nCHttpResponse::extract...\n-----------------------------------------------------");

  vector<insensitive_string> components;
  vector<string>::const_iterator iter=response.begin();

/*
** Ist ein Response-Kopf auszuwerten?
*/
  if(withHead) {
    components=chopIntoWords(insensitive_string(iter->c_str())," ");
/*
** Protokoll
*/
    m_protocol=components[0].c_str();
/*
** Status-Code
*/
    m_serverStatusCode=atoi(components[1].c_str());
/*
** Status-Text
*/
    m_serverStatus=iter->substr(components[0].size()+components[1].size()+2);
    CBTRACELN("\""+m_protocol+"\" \""+toString(m_serverStatusCode)+"\" \""+m_serverStatus+"\"");

    m_unknownHeaders.clear();
    ++iter;
  }

  while(iter!=response.end()) {
/*
** Header in einzelne Teile zerlegen
*/
    components=chopIntoWords(insensitive_string(iter->c_str())," ");
/*
** Date?
*/
    if(components[0]=="date:") {
      if(components.size()==7) {
        m_date=CMoment(iter->substr(iter->find(":")+2));
        CBTRACELN("Erkannt: Date "+m_date.getDDMMYYYYHHMMSS());
      }
      else {
        CBTRACELN("HEADER FEHLERHAFT: "+ *iter);
      }
    }
/*
** Last-Modified?
*/
    else if(components[0]=="last-modified:") {
      if(components.size()==7) {
        m_lastModified=CMoment(iter->substr(iter->find(":")+2));
        CBTRACELN("Erkannt: Last-Modified "+m_lastModified.getDDMMYYYYHHMMSS());
      }
      else {
        CBTRACELN("HEADER FEHLERHAFT: "+ *iter);
      }
    }
/*
** Expires?
*/
    else if(components[0]=="expires:") {
      if(components.size()==7) {
        m_expires=CMoment(iter->substr(iter->find(":")+2));
        CBTRACELN("Erkannt: Expires "+m_expires.getDDMMYYYYHHMMSS());
      }
      else {
        CBTRACELN("HEADER FEHLERHAFT: "+ *iter);
      }
    }
/*
** Accept-Ranges?
*/
    else if(components[0]=="accept-ranges:") {
      m_acceptRanges=components[1].c_str();
      CBTRACELN("Erkannt: Accept-Ranges "+m_acceptRanges);
    }
/*
** ETag?
*/
    else if(components[0]=="etag:") {
      m_eTag=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: ETag "+m_eTag);
    }
/*
** Location?
*/
    else if(components[0]=="location:") {
      m_location=components[1].c_str();
      CBTRACELN("Erkannt: Location "+m_location);
    }
/*
** Server?
*/
    else if(components[0]=="server:") {
      m_server=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Server "+m_server);
    }
/*
** Content-Language?
*/
    else if(components[0]=="content-language:") {
      m_contentLanguage=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Content-Language "+m_contentLanguage);
    }
/*
** Content-Length?
*/
    else if(components[0]=="content-length:") {
      m_contentLength=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Content-Length "+m_contentLength);
    }
/*
** Content-Location?
*/
    else if(components[0]=="content-location:") {
      m_contentLocation=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Content-Location "+m_contentLocation);
    }
/*
** Content-Type?
*/
    else if(components[0]=="content-type:") {
      m_contentType=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Content-Type "+m_contentType);
    }
/*
** Connection?
*/
    else if(components[0]=="connection:") {
      m_connection=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Connection "+m_connection);
    }
/*
** Transfer-Encoding?
*/
    else if(components[0]=="transfer-encoding:") {
      m_transferEncoding=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Transfer-Encoding "+m_transferEncoding);
    }
/*
** Set-Cookie?
*/
    else if(components[0]=="set-cookie:") {
      m_setCookie=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Set-Cookie "+m_setCookie);
    }
/*
** Via:
*/
    else if(components[0]=="via:") {
      m_via=iter->substr(iter->find(":")+2);
      CBTRACELN("Erkannt: Via "+m_via);
    }
/*
** Von CHttpResponse nicht unterstützter Header!
** Wird in m_unknownHeaders abgelegt
*/
    else {
      CBTRACELN("UNKNOWN: "+ *iter);
      m_unknownHeaders.push_back(*iter);
    }
    ++iter;
  }
}



}; // namespace Codebook

