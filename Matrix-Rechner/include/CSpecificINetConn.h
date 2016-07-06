#ifndef __CSPECIFICINETCONN_H
#define __CSPECIFICINETCONN_H
/*
** Unter Projekteigenschaften MFC-Verwendung aktivieren
*/
#include <afxsock.h>

#include <exception>
#include <string>

namespace Codebook {

class internet_error : public std::runtime_error { 
public:
  explicit internet_error(const std::string &error)
  : runtime_error(error)
  { }
};

class CSpecificINetConn {
private:
  static WSADATA m_wsaData;
  CSocket *m_socket;
  CSocketFile *m_sfile;
  CArchive *m_archive;
  bool m_available;

public:
  bool isAvailable() {
    return(m_available);
  }
  static void initialize();
  CSpecificINetConn(const std::string &host, unsigned int port);
  virtual ~CSpecificINetConn();
  int receive(void *buffer, int bsize);
  int send(const void *buffer, int bsize);

};

}; // namespace Codebook

#endif
