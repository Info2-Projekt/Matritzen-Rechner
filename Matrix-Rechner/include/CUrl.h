#ifndef __CURL_H
#define __CURL_H

#include <string>
#include <vector>
#include "IString.h"

namespace Codebook {

class CUrl {
public:
  enum Scheme {UNDEFINED=0, PARTIAL, HTTP, FTP, FILE};
  typedef std::pair<std::string, std::string> parameter_type;
  typedef std::vector<parameter_type> parameter_container;
  typedef std::vector<std::string> folder_container;
private:
  std::string m_url;

  Scheme m_scheme;
  std::string m_protocol;
  std::string m_username;
  std::string m_password;
  std::string m_domain;
  std::string m_port;
  folder_container m_folders;
  parameter_container m_parameters;
  std::string m_fragment;
  std::string m_ftptype;

  size_t scheme(const insensitive_string &url,
                     size_t be);
  size_t user(const insensitive_string &url,
                      size_t be);
  size_t password(const insensitive_string &url,
                      size_t be);
  size_t host(const insensitive_string &url,
                      size_t be);
  size_t filehost(const insensitive_string &url,
                      size_t be);
  size_t port(const insensitive_string &url,
                      size_t be);
  size_t hostport(const insensitive_string &url,
                      size_t be);
  size_t login(const insensitive_string &url,
                      size_t be);
  size_t httppath(const insensitive_string &url,
                      size_t be);
  size_t ftppath(const insensitive_string &url,
                    size_t be);
  size_t filepath(const insensitive_string &url,
                    size_t be);
  size_t search(const insensitive_string &url,
                    size_t be);
  size_t fragment(const insensitive_string &url,
                    size_t be);
  size_t ftptype(const insensitive_string &url,
                    size_t be);
  bool divideUrl(Scheme scheme);
  void buildUrl();
  bool isUrlPartial(const std::string &url);
  CUrl combineUrls(const CUrl &base, const CUrl &rel);
  void encodeUrl();
  void decodeUrl();

public:
  class EUrlError {};
  CUrl();
  CUrl(const std::string &url, Scheme s=HTTP);
  CUrl(const std::string &url, const std::string &relurl);

  static std::string encodeString(const std::string &s);
  static std::string decodeString(const std::string &s);

  CUrl getEncodedUrl();
  CUrl getDecodedUrl();

  void showUrl() const;

  std::string getUrl() const {
    return(m_url);
  }
  std::string getProtocol() const {
    return(m_protocol);
  }
  Scheme getScheme() const {
    return(m_scheme);
  }
  std::string getUsername() const {
    return(m_username);
  }
  std::string getPassword() const {
    return(m_password);
  }
  std::string getDomain() const {
    return(m_domain);
  }
  std::string getPort() const {
    return(m_port);
  }
  std::string getFragment() const {
    return(m_fragment);
  }
  std::string getFtptype() const {
    return(m_ftptype);
  }
  void setUrl(const std::string &s) {
    m_url=s;
    buildUrl();
  }
  void setUsername(const std::string &s) {
    m_username=s;
    buildUrl();
  }
  void setPassword(const std::string &s) {
    m_password=s;
    buildUrl();
  }
  void setDomain(const std::string &s) {
    m_domain=s;
    buildUrl();
  }
  void setPort(const std::string &s) {
    m_port=s;
    buildUrl();
  }
  void setFragment(const std::string &s) {
    m_fragment=s;
    buildUrl();
  }
  void setFtptype(const std::string &s) {
    m_ftptype=s;
    buildUrl();
  }

  void addPath(const std::string &s);
  std::string deletePath();
  std::string getPath() const;

    
  const parameter_container &getParameters() const {
    return(m_parameters);
  }

  std::string getParametersAsString() const;

  bool isParamValid(const std::string &p) const;
  std::string getParamValue(const std::string &p) const;
  void setParam(const std::string &p, const std::string &v);
  void deleteParam(const std::string &p);
  void setValue(const std::string &v);


  const folder_container &getFolders() const {
    return(m_folders);
  }


};



}; // namespace Codebook

#endif

