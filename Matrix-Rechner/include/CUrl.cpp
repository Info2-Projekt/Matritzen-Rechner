#include "CUrl.h"
#include "StringFunctions.h"

#include <iostream>

using namespace std;


namespace Codebook {

CUrl::CUrl()
: m_scheme(UNDEFINED)
{}


CUrl::CUrl(const string &url, Scheme s)
: m_url(url), m_scheme(UNDEFINED) {
divideUrl(s);
}


CUrl::CUrl(const string &url, const string &relurl)
: m_url(url), m_scheme(UNDEFINED) {
/*
** Erste URL muss eine vollständige und die zweite eine
** partielle sein.
*/
  if(isUrlPartial(url)||!isUrlPartial(relurl))
    throw(EUrlError());
/*
** Umwandeln der URLs in CUrl-Objekte und Aufruf
** von combineUrls
*/
  CUrl curl(url), crelurl(relurl,curl.m_scheme);
  *this=combineUrls(curl, crelurl);
}

bool CUrl::isUrlPartial(const std::string &url) {
/*
** Bei einer nicht-partiellen URL müssen die Zeichen
** Doppelpunkt und Slash vorhanden sein und der erste
** Doppelpunkt muss vor dem ersten Slash stehen.
*/
  size_t colonpos=url.find(":");
  size_t slashpos=url.find("/");
  if((colonpos!=string::npos)&&
    (slashpos!=string::npos)&&
     (colonpos<slashpos))
     return(false);
  else
    return(true);
}

void CUrl::addPath(const string &s) {
  m_folders.push_back(s);
  buildUrl();
}

string CUrl::deletePath() {
  std::string tmp=m_folders.back();
  m_folders.pop_back();
  buildUrl();
  return(tmp);
}

string CUrl::getPath() const {
  if(m_folders.size()==0)
    return("");

  folder_container::const_iterator iter=m_folders.begin();
  string path=*(iter++);

  while(iter!=m_folders.end())
    path+="/" + *(iter++);

  return(path);
}

void CUrl::setParam(const string &p, const string &v) {
/*
** Alle Parameterpaare durchlaufen
*/
  for(parameter_container::iterator iter=m_parameters.begin();iter!=m_parameters.end(); ++iter)
/*
** Wird Parametername gefunden?
** => Parameter bekommt neuen Wert.
*/
    if(iter->first==p) {
      iter->second=v;
      buildUrl();
      return;
    }
/*
** Parametername wurde nicht gefunden.
** => Neues Parameter-Paar wird angelegt.
*/
  m_parameters.push_back(make_pair(p,v));
  buildUrl();
}

void CUrl::deleteParam(const string &p) {
/*
** Alle Parameterpaare durchlaufen
*/
  for(parameter_container::iterator iter=m_parameters.begin();iter!=m_parameters.end(); ++iter)
/*
** Wird Parametername gefunden?
** => Parameter-Paar löschen.
*/
    if(iter->first==p) {
      m_parameters.erase(iter);
      buildUrl();
      return;
    }
}

void CUrl::setValue(const string &v) {
/*
** Alle Parameterpaare durchlaufen
*/
  for(parameter_container::iterator iter=m_parameters.begin();iter!=m_parameters.end(); ++iter)
/*
** Wird Wert gefunden?
** => Funktion beenden
*/
    if(iter->second==v)
      return;
/*
** Wert nicht gefunden.
** => Wert wird angelegt.
*/
  m_parameters.push_back(make_pair(string(),v));
  buildUrl();
}


CUrl CUrl::combineUrls(const CUrl &base, const CUrl &rel) {
  CUrl result;
  folder_container::const_iterator riter=rel.m_folders.begin();
/*
** Existieren keine Teilpfade?
** => Basis-URL mit Parametern und Fragment von partieller URL ergänzen
*/
  if((base.m_scheme==HTTP)&&(rel.m_folders.size()==0)) {
    result=base;
    result.m_parameters=rel.m_parameters;
    result.m_fragment=rel.m_fragment;
    result.buildUrl();
    return(result);
  }
/*
** Partielle URL bginnt mit // ?
** => Der gesamte Pfad der Basis-URL, selbst der Host,
**    wird gelöscht.
*/
  if(rel.m_folders.front()=="//") {
    result.m_url=base.m_protocol+rel.m_url.substr(2);
    result.divideUrl(base.m_scheme);
    return(result);
  }
/*
** Außer den Parametern und dem Fragment werden
** alle Bestandteile der Basis-URL übernommen
*/
  result=base;
  result.m_parameters=rel.m_parameters;
  result.m_fragment=rel.m_fragment;
  result.m_ftptype=rel.m_ftptype;

/*
** Beginnt URL mit / ?
** => der gesamte Pfad wird gelöscht.
*/
  if(rel.m_folders.front()=="/") {
    result.m_folders.clear();
    riter+=1;
  }
/*
** Andernfalls wird der letzte Teil des Pfades
** (Üblicherweise der Dateiname) gelöscht.
*/
  else
    result.m_folders.pop_back();

/*
** Alle Pfad-Teile (den letzten ausgenommen)
** werden durchlaufen
*/
  bool basereached=(result.m_folders.size()==0);
  for(;riter!=rel.m_folders.end(); ++riter) {
/*
** Pfadteil in partieller URL gleich .. und
** Ergebnis-Url hat noch Pfadteile?
** => Der aktuell letze Pfadteil wird gelöscht.
*/
    if((!basereached)&&(*riter=="..")) {
      result.m_folders.pop_back();
      if(result.m_folders.size()==0)
        basereached=true;
    }
/*
** Pfadteil in partieller URL gleich . ?
** => Ignorieren.
*/
    else if(*riter!=".")
      result.m_folders.push_back(*riter);
  }
/*
** Aus den Einzelteilen wird die Gesamt-URL
** aufgebaut
*/
  result.buildUrl();
  return(result);
}

size_t CUrl::scheme(const insensitive_string &url,
                  size_t be) {
/*
** Bei nicht-partieller URL Schema isolieren
** und auswerten
*/
  if(isUrlPartial(m_url)) {
    m_scheme=PARTIAL;
    return(be);
  }

  if(url.find("http://",be)==be) {
    m_scheme=HTTP;
    m_protocol=url.substr(0,7).c_str();
    return(be+7);
  }

  if(url.find("file://",be)==be) {
    m_scheme=FILE;
    m_protocol=url.substr(0,7).c_str();
    return(be+7);
  }

  if(url.find("ftp://",be)==be) {
    m_scheme=FTP;
    m_protocol=url.substr(0,6).c_str();
    return(be+6);
  }
  throw(EUrlError());
  return(be);
}

size_t CUrl::user(const insensitive_string &url,
                    size_t be) {
  size_t colonpos=url.find(":", be);
  size_t atpos=url.find("@", be);
  size_t slashpos=url.find("/", be);
  size_t en;

/*
** Existiert ein @ und steht vor einem eventuellen / ?
*/
  if((atpos!=insensitive_string::npos)&&(atpos<slashpos))
/*
** Existiert vor dem @ noch ein : ?
*/
    if((colonpos!=insensitive_string::npos)&&(colonpos<atpos))
      en=colonpos;
    else
      en=atpos;
/*
** Kein Klammeraffe vor einem / ?
** => Kein Username!
*/
  else
    return(be);
/*
** Username wird isoliert
*/
  m_username=url.substr(be,en-be).c_str();
/*
** Eventuelles Passwort wird isoliert
*/
  return(password(url, en));
}

size_t CUrl::password(const insensitive_string &url,
                    size_t be) {
/*
** Aktuelles Zeichen ein @?
** => Nur Username, kein Passwort
*/
  if(url[be]=='@')
    return(be+1);
/*
** Passwort muss hinter einem : stehen
*/
  if(url[be]!=':')
    throw(EUrlError());
/*
** Passwort muss vor einem @ stehen
*/
  size_t atpos=url.find("@", be);
  size_t slashpos=url.find("/", be);
  size_t en;

  if((atpos!=insensitive_string::npos)&&(atpos<slashpos))
    en=atpos;
  else
    throw(EUrlError());

  be++;
/*
** Passwort isolieren
*/
  m_password=url.substr(be,en-be).c_str();
  return(en+1);
}

size_t CUrl::host(const insensitive_string &url,
                    size_t be) {
/*
** Leerer Host?
*/
  if(url[be]=='/')
    return(be+1);
/*
** / oder : oder Stringende markieren Ende des Hosts
*/
  size_t slashpos=url.find("/", be);
  size_t colonpos=url.find(":", be);
  size_t en;
/*
** Existiert ein : und steht dieser vor einem / ?
** => Hostende an :-Position
*/
  if((colonpos!=insensitive_string::npos)&&(colonpos<slashpos))
    en=colonpos;
/*
** Existiert ein / ?
** => Hostende an /-Position
*/
  else if(slashpos!=insensitive_string::npos)
    en=slashpos;
/*
** Kein / ?
** => Restlicher String ist Host.
*/
  else
    en=url.length();
/*
** Host isolieren
*/
  m_domain=url.substr(be, en-be).c_str();
/*
** Eventuellen Port isolieren
*/
  return(port(url, en));

}

size_t CUrl::filehost(const insensitive_string &url,
                    size_t be) {
/*
** Leerer Host?
*/
  if(url[be]=='/')
    return(be+1);
/*
** / oder Stringende markieren Ende des Hosts
*/
  size_t slashpos=url.find("/", be);
  size_t en;
/*
** Existiert ein / ?
** => Hostende an /-Position
*/
  if(slashpos!=insensitive_string::npos)
    en=slashpos;
/*
** Kein / ?
** => Restlicher String ist Host.
*/
  else
    en=url.length();
/*
** Host isolieren
*/
  m_domain=url.substr(be, en-be).c_str();
/*
** Eventuellen Port isolieren
*/
  return(en);
}

size_t CUrl::port(const insensitive_string &url,
                    size_t be) {
/*
** Port steht hinter einem :
*/
  if(url[be]!=':')
    return(be+1);
  size_t slashpos=url.find("/", be);
  size_t en;
/*
** Existiert ein / ?
** => Port endet an /-Position
*/
  if(slashpos!=insensitive_string::npos)
    en=slashpos;
/*
** Kein / ?
** => Port endet am Stringende
*/
  else
    en=url.length();

  be++;
/*
** Port isolieren
*/
  m_port=url.substr(be, en-be).c_str();
  return(en+1);

}

size_t CUrl::hostport(const insensitive_string &url,
                    size_t be) {
  return(host(url, be));

}

size_t CUrl::login(const insensitive_string &url,
                    size_t be) {
  if(m_scheme==PARTIAL)
    return(be);
/*
** Falls vorhanden, Username und Passwort isolieren
*/
  be=user(url, be);
/*
** Host und Port isolieren
*/
  return(hostport(url, be));
}

size_t CUrl::httppath(const insensitive_string &url,
                   size_t be)  {
  if(be>=url.length())
    return(be);
/*
** Pfadteil endet mit /, ?, # oder Stringende
*/
  size_t slashpos=url.find("/", be);
  size_t questionpos=url.find("?", be);
  size_t gatepos=url.find("#", be);
/*
** Ermitteln, ob ? oder # zuerst kommt
*/
  size_t firstpos=(questionpos<=gatepos)?questionpos:gatepos;
  size_t en;
/*
** # oder ? vor Slash?
** => Pfadteil endet bei # oder ?
*/
  if((firstpos!=insensitive_string::npos)&&(firstpos<slashpos))
    en=firstpos;
/*
** Existiert ein / ?
** => Pfadteil endet bei /
*/
  else if(slashpos!=insensitive_string::npos)
    en=slashpos;
/*
** Kein / ?
** => Pfadteil endet am Stringende
*/
  else
    en=url.length();


/*
** Pfadteil isolieren
*/
  m_folders.push_back(url.substr(be, en-be).c_str());
/*
** Pfadteil endete mit ? oder # ?
** => Pfad zu Ende
*/
  if((en==questionpos)||(en==gatepos))
    return(en);
/*
** Nächsten Pfadteil isolieren
*/
  return(httppath(url, en+1));
}

size_t CUrl::ftppath(const insensitive_string &url,
                   size_t be)  {
  if(be>=url.length())
    return(be);
/*
** Pfadteil endet mit / oder ; oder Stringende
*/
  size_t slashpos=url.find("/", be);
  size_t semicolonpos=url.find(";", be);
/*
** Ermitteln, ob / oder ; zuerst kommt
*/
  size_t firstpos=(slashpos<=semicolonpos)?slashpos:semicolonpos;
  size_t en;
/*
** / oder ; ?
** => Pfadteil endet bei / oder ;
*/
  if(firstpos!=insensitive_string::npos)
    en=firstpos;
/*
** Kein / oder ; ?
** => Pfadteil endet am Stringende
*/
  else
    en=url.length();


/*
** Pfadteil isolieren
*/
  m_folders.push_back(url.substr(be, en-be).c_str());
/*
** Pfadteil endete mit ; ?
** => Pfad zu Ende
*/
  if(en==semicolonpos)
    return(en);
/*
** Nächsten Pfadteil isolieren
*/
  return(ftppath(url, en+1));
}

size_t CUrl::filepath(const insensitive_string &url,
                   size_t be)  {
  if(be>=url.length())
    return(be);
/*
** Pfadteil endet mit / oder Stringende
*/
  size_t slashpos=url.find("/", be);
  size_t en;
/*
** / vorhanden?
** => Pfadteil endet an /-Position
*/
  if(slashpos!=insensitive_string::npos)
    en=slashpos;
/*
** Kein / ?
** => Pfadteil endet am Stringende
*/
  else
    en=url.length();
/*
** Pfadteil isolieren
*/
  m_folders.push_back(url.substr(be, en-be).c_str());
/*
** Eventuell weiteren Pfadteil isolieren
*/
  return(filepath(url, en+1));
}

size_t CUrl::search(const insensitive_string &url,
                   size_t be)  {
/*
** Parameter stehen hinter einem ?
*/
  if(url[be]!='?')
    return(be);
  size_t gatepos=url.find("#", be);
  size_t en;
/*
** # vorhanden?
** => Parameter enden an #-Position
*/
  if(gatepos!=insensitive_string::npos)
    en=gatepos;
/*
** Kein # ?
** => Parameter enden am Stringende
*/
  else
    en=url.length();
/*
** Parameter isolieren
*/
  be++;
  string parameters=url.substr(be, en-be).c_str();
  size_t pbe=0;
  while(pbe<parameters.length()) {
/*
** Ende des aktuellen Parameters bestimmen
*/
    size_t pen=parameters.find("&",pbe);
    if(pen==string::npos)
      pen=parameters.length();
/*
** Aktuellen Parameter isolieren
*/
    string param=parameters.substr(pbe,pen-pbe);
/*
** Prüfen, ob es Parameter oder Wert ist
*/
    size_t equalpos=param.find("=");
    if(equalpos!=string::npos)
      m_parameters.push_back(make_pair(param.substr(0,equalpos),param.substr(equalpos+1)));
    else
      m_parameters.push_back(make_pair(string(),param));
    pbe=pen+1;
  }
  return(en);
}

bool CUrl::isParamValid(const std::string &p) const {
/*
** Alle Parameterpaare durchlaufen
*/
  for(parameter_container::const_iterator iter=m_parameters.begin();iter!=m_parameters.end(); ++iter)
/*
** Ist Parametername, oder bei leerem Parameternamen
** der Parameterwert der gesuchte?
*/
    if((iter->first==p)||((iter->first=="")&&(iter->second==p)))
      return(true);
  return(false);
}

string CUrl::getParamValue(const std::string &p) const {
/*
** Alle Parameterpaare durchlaufen
*/
  for(parameter_container::const_iterator iter=m_parameters.begin();iter!=m_parameters.end(); ++iter)
/*
** Ist Parametername, oder bei leerem Parameternamen
** der Parameterwert der gesuchte?
*/
    if((iter->first==p)||((iter->first=="")&&(iter->second==p)))
      return(iter->second);
  return("");
}


size_t CUrl::fragment(const insensitive_string &url,
                   size_t be)  {
/*
** Fragment beginnt mit #
*/
  if(url[be]!='#')
    return(be);
  size_t en=url.length();
/*
** Fragment isolieren
*/
  be++;
  m_fragment=url.substr(be, en-be).c_str();
  return(en);
}

size_t CUrl::ftptype(const insensitive_string &url,
                   size_t be)  {
/*
** ftptype beginnt mit ;
*/
  if(url[be]!=';')
    return(be);
  size_t en=url.length();
  size_t equalpos=url.find("=",be);
/*
** Existiert = ?
** => ftptype hinter = isolieren
*/
  if(equalpos!=insensitive_string::npos) {
    m_ftptype=url.substr(equalpos+1).c_str();
    return(en);
  }
  return(be);
}

void CUrl::buildUrl() {
/*
** Schema hinzufügen
*/
  m_url=m_protocol;
/*
** Bei HTTP und FTP Username und Passwort
** berücksichtigen, falls vorhanden
*/
  if((m_scheme==HTTP)||(m_scheme==FTP)) {
    if(m_username!="") {
      m_url+=m_username;
      if(m_password!="")
        m_url+=":"+m_username;
      m_url+="@";
    }
  }
/*
** Host hinzufügen
*/
  m_url+=m_domain;
/*
** Bei HTTP und FTP Port
** berücksichtigen, falls vorhanden
*/
  if((m_scheme==HTTP)||(m_scheme==FTP)) {
    if(m_port!="")
      m_url+=":"+m_port;
  }
/*
** Alle Teilpfade hinzufügen
*/
  for(folder_container::const_iterator iter=m_folders.begin(); iter!=m_folders.end(); ++iter)
    m_url+="/"+*iter;
/*
** Bei HTTP Parameter und Fragment-Bezeichner
** berücksichtigen, falls vorhanden
*/
  if(m_scheme==HTTP) {
    if(m_parameters.size()!=0)
      for(parameter_container::iterator piter=m_parameters.begin(); piter!=m_parameters.end(); ++piter) {
/*
** Vor erstem Parameter-Paar muss ein ? stehen, vor
** den restlichen ein &
*/
        if(piter==m_parameters.begin())
          m_url+="?";
        else
          m_url+="&";
/*
** Bloßer Parameter-Wert?
*/
        if(piter->first=="")
          m_url+=piter->second;
/*
** Parameter-Paar
*/
        else
          m_url+=piter->first+"="+piter->second;
      }

    if(m_fragment!="")
      m_url+="#"+m_fragment;
  }
/*
** Bei FTP den FTP-Type
** berücksichtigen, falls vorhanden
*/
  if(m_scheme==FTP) {
    if(m_ftptype!="")
      m_url+=";type="+m_ftptype;
  }
}


bool CUrl::divideUrl(Scheme s) {
  insensitive_string url=m_url.c_str();
  size_t be=0;
/*
** Schema isolieren
*/
  be=scheme(url, be);
/*
** Schema ist HTTP?
** => Login, Pfad, Parameter und Fragment isolieren
*/
  if(m_scheme==HTTP) {
    be=login(url, be);
    be=httppath(url, be);
    be=search(url, be);
    be=fragment(url, be);
  }
/*
** Schema ist FTP?
** => Pfad und Type isolieren
*/
  else if(m_scheme==FTP) {
    be=login(url, be);
    be=ftppath(url, be);
    be=ftptype(url, be);
  }
/*
** Schema ist FILE?
** => Host und Pfad isolieren
*/
  else if(m_scheme==FILE) {
    be=filehost(url, be);
    be=filepath(url, be);
  }
/*
** Partielle URL?
*/
  else if(m_scheme==PARTIAL) {
    if(be<url.length()) {
      if(url[be]!='#') {
/*
** Auftreten von // oder / am Anfang des Pfades
** isolieren
*/
        if(url.substr(be,2)=="//") {
          m_folders.push_back("//");
          be+=2;
        }
        else if(url.substr(be,1)=="/") {
          m_folders.push_back("/");
          be+=1;
        }
      }
    }

/*
** Soll partielle URL als HTTP interpretiert werden?
** => Relativen Pfad, Parameter und Fragment isolieren
*/
    if(s==HTTP) {
      be=httppath(url, be);
      be=search(url, be);
      be=fragment(url, be);
    }
/*
** Soll partielle URL als FTP interpretiert werden?
** => Relativen Pfad und Type
*/
    else if(s==FTP) {
      be=ftppath(url, be);
      be=ftptype(url, be);
    }
/*
** Soll partielle URL als FILE interpretiert werden?
** => Relativen Pfad isolieren
*/
    else if(s==FILE) {
      be=filepath(url, be);
    }
  }
  return(true);
}

void CUrl::showUrl() const{
  cout << "URL        :" << m_url << endl;
  cout << "Protokoll  :" << m_protocol << endl;
  cout << "Username   :" << m_username << endl;
  cout << "Passwort   :" << m_password << endl;
  cout << "Domaene    :" << m_domain << endl;
  cout << "Port       :" << m_port << endl;
  cout << "Ordner     :" << endl;
  for(size_t i=0; i<m_folders.size(); i++)
    cout << "            " << m_folders[i] << endl;
  cout << "Parameter  :" << endl;
  for(i=0; i<m_parameters.size(); i++)
    if(m_parameters[i].first!="")
      cout << "            " << m_parameters[i].first << "=" << m_parameters[i].second << endl;
    else
      cout << "            " << m_parameters[i].second << endl;

  cout << "Fragment   :" << m_fragment << endl;
  cout << "FTP-Type   :" << m_ftptype << endl;
}

void CUrl::encodeUrl() {
  for(folder_container::iterator fiter=m_folders.begin(); fiter!=m_folders.end(); ++fiter)
    *fiter=encodeString(*fiter);

  for(parameter_container::iterator piter=m_parameters.begin(); piter!=m_parameters.end(); ++piter) {
    piter->first=encodeString(piter->first);
    piter->second=encodeString(piter->second);
  }
}

CUrl CUrl::getEncodedUrl() {
  CUrl tmp=*this;
  tmp.encodeUrl();
  tmp.buildUrl();
  return(tmp);
}

void CUrl::decodeUrl() {
  for(folder_container::iterator fiter=m_folders.begin(); fiter!=m_folders.end(); ++fiter)
    *fiter=decodeString(*fiter);

  for(parameter_container::iterator piter=m_parameters.begin(); piter!=m_parameters.end(); ++piter) {
    piter->first=decodeString(piter->first);
    piter->second=decodeString(piter->second);
  }
}

CUrl CUrl::getDecodedUrl() {
  CUrl tmp=*this;
  tmp.decodeUrl();
  tmp.buildUrl();
  return(tmp);
}

string CUrl::encodeString(const string &s) {
  string tmp;
  for(string::const_iterator iter=s.begin(); iter!=s.end(); ++iter)
    if(((*iter>=45)&&(*iter<=46))||
       ((*iter>=48)&&(*iter<=57))||
       ((*iter>=65)&&(*iter<=90))||
       ((*iter>=97)&&(*iter<=122))||
       (*iter==42)||(*iter==95))
       tmp+=*iter;
    else
      tmp+="%"+valueToHex((static_cast<int>(*iter)+256)%256);
  return(tmp);
}

string CUrl::decodeString(const string &s) {
  string tmp;
  size_t pos=0;
  while(pos<s.length()) {
/*
** Leerzeichen ?
*/
    if(s[pos]=='+') {
      tmp+=" ";
      pos++;
    }
/*
** Normales Zeichen
*/
    else if(s[pos]!='%')
      tmp+=s[pos++];
    else {
/*
** Mit % kodiertes Zeichen
*/
      tmp+=static_cast<char>(hexToValue(s.substr(pos+1,2)));
      pos+=3;
    }
  }
  return(tmp);
}

string CUrl::getParametersAsString() const {
  string params;

  if(m_scheme==HTTP) {
  if(m_parameters.size()!=0)
    for(parameter_container::const_iterator piter=m_parameters.begin(); piter!=m_parameters.end(); ++piter) {
/*
** Die Parameterpaare mit & trennen
*/
      if(piter!=m_parameters.begin())
        params+="&";
/*
** Bloßer Parameter-Wert?
*/
      if(piter->first=="")
        params+=piter->second;
/*
** Parameter-Paar
*/
      else
        params+=piter->first+"="+piter->second;
    }
  }
  return(params);
}


}; // namespace Codebook