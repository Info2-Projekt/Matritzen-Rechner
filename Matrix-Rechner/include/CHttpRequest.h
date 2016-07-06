
#ifndef __CHTTPREQUEST_H
#define __CHTTPREQUEST_H

#include <string>
#include "CHugeNumber.h"
#include "CUrl.h"

namespace Codebook {


class CHttpRequest {
public:
  enum RequestType {rtGET=0, rtPOST, rtHEAD, rtPUT, rtDELETE, rtTRACE, rtCONNECT};
  enum ProtocolType {HTTP10=0, HTTP11};
  static const std::string const RTStrings[];
  static const std::string const PTStrings[];
  static const std::string UAExplorer6;
  static const std::string UANetscape6;

private:
  RequestType m_requestType;
  ProtocolType m_protocol;
  std::string m_filePath;
  std::string m_host;
  std::string m_range;
  std::string m_referer;
  std::string m_userAgent;
  std::string m_maxForwards;

public:
  CHttpRequest()
  : m_requestType(rtGET), m_protocol(HTTP11)
  { }

  void setRequestType(RequestType rt) {
    m_requestType=rt;
  }

  void setProtocol(ProtocolType pt) {
    m_protocol=pt;
  }

/*
** filepath = Pfad mit Dateinamen ohne führendes /
*/
  void setFilePath(const std::string &filepath) {
    m_filePath=filepath;
  }

  void setHost(const std::string &host, const std::string &port);
  void setHost(const std::string &host, unsigned int port) {
    setHost(host,toString(port));
  }

  void setHost(const CUrl &url) {
    setHost(url.getDomain(), url.getPort());
  }

  void clearHost() {
    m_host="";
  }

  void setRange(unsigned long begin, unsigned long end);
  void setRange(CHugeNumber begin, CHugeNumber end);
  void clearRange() {
    m_range="";
  }
  void setReferer(const std::string &ref) {
    m_referer=ref;
  }

  void setReferer(const CUrl &ref) {
    m_referer=ref.getUrl();
  }

  void setUserAgent(const std::string &agent) {
    m_userAgent=agent;
  }

  void setMaxForwards(const std::string &mf) {
    m_maxForwards=mf;
  }

  void setMaxForwards(int mf) {
    setMaxForwards(toString(mf));
  }

  void setFromUrl(const CUrl &url);

  std::string getRequest() const;

};


}; // namespace Codebook

#endif
