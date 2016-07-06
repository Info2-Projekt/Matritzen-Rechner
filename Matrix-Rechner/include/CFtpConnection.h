
#ifndef __CFTPCONNECTION_H
#define __CFTPCONNECTION_H

/*
** Unter Projekteigenschaften MFC-Verwendung aktivieren
*/

#include "CInternetConnection.h"
#include "CUrl.h"
#include "CFileInfo.h"

namespace Codebook {

class CFtpConnection {
private:
  typedef std::pair<std::string, std::string> ServerReply;
  typedef std::pair<std::string, unsigned int> Address;

  static const std::string AnonymousName;
  static const std::string AnonymousPwd;

  std::string m_lastError;
  std::string m_rootDir;
  std::string m_currentDir;
  CInternetConnection *m_iconn;


  ServerReply splitReply(const std::string &line) const;
  ServerReply waitForCmdGroup(int group);
  ServerReply waitForSpecificCmd(int cmd);
  ServerReply waitForAnyCmd();
  std::string  rebuildReply(const ServerReply &reply) const;
  CFileInfo convertListEntryToFileInfo(const std::string &line);

public:
  enum FtpType {A='A', I='I', B='B'};
  CFtpConnection(const std::string &host, unsigned int port=21, bool newlogin=true, int retries=5);
  CFtpConnection(const CUrl &url, bool newlogin=true, int retries=5);
  ~CFtpConnection();
  bool login(const std::string &username="anonymous", const std::string &pwd="", const std::string &account="");
  bool login(const CUrl &url, const std::string &account="");
  bool setType(FtpType type);
//  bool setPort();
  Address setPassive();
  bool list(CFileInfo::filelist_type &filelist);
  std::string getCurrentDir();
  bool changeDir(const std::string &path);
  bool retrieveFile(const std::string &serverFile, const std::string &localFile);

  static bool retrieveFile(const CUrl &url, const std::string &localPath);

  static bool getDir(const CUrl &url, CFileInfo::filelist_type &files);
  static bool getCompleteDir(const CUrl &url, CFileInfo &fi);
  bool getCompleteDir(const std::string &dir, CFileInfo &fi);

  std::string getRootDir() const {
    return(m_rootDir);
  }
  std::string getLastError() const {
    return(m_lastError);
  }
  std::string getCurrentDir() const {
    return(m_currentDir);
  }
};

}; // namespace Codebook

#endif
