

#include "CHttpRequest.h"
#include "CInternetConnection.h"


using namespace std;

namespace Codebook {

const std::string CHttpRequest::UAExplorer6="Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)";
const std::string CHttpRequest::UANetscape6="Mozilla/5.0 (Windows; U; Win98; en-US; m18) Gecko/20010131 Netscape6/6.01";

const string const CHttpRequest::RTStrings[]={"GET", "POST", "HEAD", "PUT", "DELETE", "TRACE", "CONNECT"};
const string const CHttpRequest::PTStrings[]={"HTTP/1.0", "HTTP/1.1"};


void CHttpRequest::setHost(const string &host, const string &port) {
  m_host=host;
  if((port!="")&&(port!="80"))
    m_host+=":"+port;
}

//*********

void CHttpRequest::setRange(unsigned long begin, unsigned long end) {
  m_range="bytes="+toString(begin)+"-"+toString(end);
}

//*********

void CHttpRequest::setRange(CHugeNumber begin, CHugeNumber end) {
  m_range="bytes="+begin.getAsString()+"-"+end.getAsString();
}

//*********

void CHttpRequest::setFromUrl(const CUrl &url) {
  m_requestType=rtGET;
  m_protocol=HTTP11;
  m_userAgent=UAExplorer6;
  setHost(url);
/*
** Filepath mit Parametern erzeugen
*/
  m_filePath=url.getPath();
  string params=url.getParametersAsString();
  if(params!="")
    m_filePath+="?"+params;
}

//*********

string CHttpRequest::getRequest() const{
  string request;

/*
** GET- oder HEAD-Request?
*/
  switch(m_requestType) {
    case rtGET:
    case rtPOST:
    case rtHEAD:
/*
** Alle definierten Header zum Request hinzufügen
*/
      request+=RTStrings[m_requestType]+" /"+m_filePath+" "+PTStrings[m_protocol]+CInternetConnection::CRLF;
      if(m_host!="")
        request+="Host: "+m_host+CInternetConnection::CRLF;
      if(m_range!="")
        request+="Range: "+m_range+CInternetConnection::CRLF;
      if(m_referer!="")
        request+="Referer: "+m_referer+CInternetConnection::CRLF;
      if(m_userAgent!="")
        request+="User-Agent: "+m_userAgent+CInternetConnection::CRLF;
      request+=CInternetConnection::CRLF;
      break;

/*
** TRACE-Request?
*/
    case rtTRACE:
/*
** Alle definierten Header zum Request hinzufügen
*/
      request+=RTStrings[m_requestType]+" /"+m_filePath+" "+PTStrings[m_protocol]+CInternetConnection::CRLF;
      if(m_host!="")
        request+="Host: "+m_host+CInternetConnection::CRLF;
      if(m_maxForwards!="")
        request+="Max-Forwards: "+m_maxForwards+CInternetConnection::CRLF;
      if(m_range!="")
        request+="Range: "+m_range+CInternetConnection::CRLF;
      if(m_referer!="")
        request+="Referer: "+m_referer+CInternetConnection::CRLF;
      if(m_userAgent!="")
        request+="User-Agent: "+m_userAgent+CInternetConnection::CRLF;
      request+=CInternetConnection::CRLF;
      break;

    default:
      return("");
  }

  return(request);
}



}; // namespace Codebook

