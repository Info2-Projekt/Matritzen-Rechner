
#ifndef __CHTTPRESPONSE_H
#define __CHTTPRESPONSE_H

#include <string>
#include<vector>
#include "IString.h"
#include "CMoment.h"


namespace Codebook {


class CHttpResponse {
public:

private:
  std::string m_acceptRanges;
  std::string m_eTag;
  std::string m_location;
  std::string m_retryAfter;
  std::string m_server;
  std::string m_contentLanguage;
  std::string m_contentLength;
  std::string m_contentLocation;
  std::string m_contentType;
  std::string m_transferEncoding;
  std::string m_setCookie;
  std::string m_connection;
  std::string m_via;
  CMoment m_date;
  CMoment m_expires;
  CMoment m_lastModified;
  std::string m_serverStatus;
  std::string m_protocol;
  int m_serverStatusCode;
  std::vector<std::string> m_unknownHeaders;

public:
  CHttpResponse()
  : m_serverStatusCode(0)
  { }

  std::string getRanges() const {
    return(m_acceptRanges);
  }

  std::string getProtocol() const {
    return(m_protocol);
  }

  int getStatusCode() const {
    return(m_serverStatusCode);
  }

  std::string getStatusText() const {
    return(m_serverStatus);
  }

  std::string getETag() const {
    return(m_eTag);
  }
  std::string getLocation() const {
    return(m_location);
  }
  std::string getRetryAfter() const {
    return(m_retryAfter);
  }
  std::string getServer() const {
    return(m_server);
  }
  std::string getcontentLanguage() const {
    return(m_contentLanguage);
  }
  std::string getContentLength() const {
    return(m_contentLength);
  }
  std::string getContentLocation() const {
    return(m_contentLocation);
  }
  std::string getContentType() const {
    return(m_contentType);
  }
  std::string getCookie() const {
    return(m_setCookie);
  }
  std::string getTransferEncoding() const {
    return(m_transferEncoding);
  }
  std::string getConnection() const {
    return(m_connection);
  }
  std::string getVia() const {
    return(m_via);
  }
  CMoment getDate() const {
    return(m_date);
  }
  CMoment getExpires() const {
    return(m_expires);
  }
  CMoment getLastModified() const {
    return(m_lastModified);
  }

  const std::vector<std::string> &getUnknownHeaders() const {
    return(m_unknownHeaders);
  }










  bool isTransferChunked() const {
    return(insensitive_string("chunked")==m_transferEncoding.c_str());
  }

  bool isContenLengthAvailable() const {
    return(m_contentLength.size()!=0);
  }

  bool isResponseAvailable() const {
    return(m_serverStatusCode!=0);
  }

  void extract(const std::vector<std::string> &response, bool withHead=true);


};


}; // namespace Codebook

#endif
