
#ifndef __CHTTPCONNECTION_H
#define __CHTTPCONNECTION_H

#include <vector>
#include "CHttpRequest.h"
#include "CInternetConnection.h"
#include "CHttpResponse.h"

#include "CUrl.h"
//#include "CFileInfo.h"

namespace Codebook {

class CHttpConnection {
private:
  CInternetConnection *m_iconn;
  std::string m_lastError;
  CHttpResponse m_lastResponse;

  bool connect(const std::string &host, unsigned int port);
  bool connect(const CUrl &url);
  void disconnect();
  bool sendRequest(const CHttpRequest &request);
  bool receiveResponse(std::vector<std::string> &response);
  bool normalFileTransfer(const std::string &filename, const CHttpResponse &response);
  bool chunkedFileTransfer(const std::string &filename, const CHttpResponse &response);

public:
  CHttpConnection();
  ~CHttpConnection() {
    disconnect();
  }

  bool getHeadHeader(const CUrl &url, std::vector<std::string> &header);
  bool getHeadHeader(const CUrl &url, CHttpResponse &header);

  bool getTraceHeader(const CUrl &url, std::vector<std::string> &header, int maxforwards);
  bool getTraceHeader(const CUrl &url, CHttpResponse &header, int maxforwards);

  bool getFile(const CUrl &url, const std::string &filename);
  bool getFile(const CUrl &url, const std::string &filename, const CHttpRequest &request);

  std::string getLastError() const {
    return(m_lastError);
  }

  const CHttpResponse &getLastResponse() const {
    return(m_lastResponse);
  }
};

}; // namespace Codebook

#endif
