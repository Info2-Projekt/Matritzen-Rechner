#ifndef __CAUTOMAT_H
#define __CAUTOMAT_H

#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iterator>
#include "CBinaryStream.h"
#include "CFileArray.h"
#include "CCharCIPredicate.h"

namespace Codebook {

class CAutomat {
private:
  class CState {
  public:
    enum TYPES {NOOP=0, JOKER, START, END, CHAR};
    TYPES m_type;
    std::vector<char> m_characters;
    std::vector<int> m_nextstates;
    CState(TYPES t=NOOP) : m_type(t) {}

    friend CBinaryOStream &operator<<(CBinaryOStream &os, const CState &s){
      os.write(&s.m_type, sizeof(s.m_type));
      os << s.m_characters;
      os << s.m_nextstates;
      return(os);
    }
    friend CBinaryIStream &operator>>(CBinaryIStream &is, CState &s){
      is.read(&s.m_type, sizeof(s.m_type));
      is >> s.m_characters;
      is >> s.m_nextstates;
      return(is);
    }

  };

  std::vector<CState> m_states;

  void build(const std::string &pattern);
  int expression(std::string::const_iterator &i);
  int term(std::string::const_iterator &i);
  int factor(std::string::const_iterator &i);
  int option(std::string::const_iterator &i);
  int hull(int state);
  int poshull(int state);
  int character(std::string::const_iterator &i);
  int set(std::string::const_iterator &i);


//*****************************************************************************************
  template<typename Type>
  bool r_find(int state, Type iter) const{
  /*
  ** Hat der Automat seinen Endzustand erreicht?
  ** => Muster wurde erkannt.
  */
    if(m_states[state].m_type==CState::END)
      return(true);
  /*
  ** Ist das Ende der Zeichenkette erreicht, es müssen
  ** aber noch CHAR-Zustände des Automaten ausgewertet
  ** werden => Muster wurde nicht erkannt
  */
    if((*iter==0)&&(m_states[state].m_type!=CState::NOOP))
      return(false);

  /*
  ** Status auswerten
  */
    std::vector<int>::const_iterator siter;
    switch(m_states[state].m_type) {
  /*
  ** CHAR-Zustand
  */
      case CState::CHAR:
  /*
  ** Das aktuelle Zeichen der Zeichenkette wird mit
  ** den Zeichen des Zustands verglichen.
  ** Stimmt es nicht überein, dann wurde das Muster
  ** nicht erkannt.
  */
        if(std::find(m_states[state].m_characters.begin(),m_states[state].m_characters.end(),*iter)==m_states[state].m_characters.end()) {
          return(false);
        } else {
  /*
  ** Bei Übereinstimmung wird rfind
  ** rekursiv für jeden Folgezustand mit dem nächsten
  ** Zeichen der Zeichenkette aufgerufen.
  */
          for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
            if(r_find(*siter,iter+1))
              return(true);
        }
          return(false);

  /*
  ** NOOP- und START-Zustand
  */
      case CState::START:
      case CState::NOOP:
  /*
  ** rfind wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_find(*siter,iter))
            return(true);
        return(false);

  /*
  ** JOKER-Zustand (Beliebiges Zeichen)
  */
      case CState::JOKER:
  /*
  ** rfind wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_find(*siter,iter+1))
            return(true);

        return(false);

    }

    return(false);
  }

//*****************************************************************************************

  template<typename Type>
  bool r_findCI(int state, Type iter) const{
  /*
  ** Hat der Automat seinen Endzustand erreicht?
  ** => Muster wurde erkannt.
  */
    if(m_states[state].m_type==CState::END)
      return(true);
  /*
  ** Ist das Ende der Zeichenkette erreicht, es müssen
  ** aber noch CHAR-Zustände des Automaten ausgewertet
  ** werden => Muster wurde nicht erkannt
  */
    if((*iter==0)&&(m_states[state].m_type!=CState::NOOP))
      return(false);

  /*
  ** Status auswerten
  */
    std::vector<int>::const_iterator siter;
    switch(m_states[state].m_type) {
  /*
  ** CHAR-Zustand
  */
      case CState::CHAR:
  /*
  ** Das aktuelle Zeichen der Zeichenkette wird ohne Berücksichtigung
  ** der Groß-/Kleinschreibung mit den Zeichen des Zustands verglichen.
  ** Stimmt es nicht überein, dann wurde das Muster
  ** nicht erkannt.
  */
        if(std::find_if(m_states[state].m_characters.begin(),m_states[state].m_characters.end(),CCharCIPredicate(*iter))==m_states[state].m_characters.end()) {
          return(false);
        } else {
  /*
  ** Bei Übereinstimmung wird r_findCI
  ** rekursiv für jeden Folgezustand mit dem nächsten
  ** Zeichen der Zeichenkette aufgerufen.
  */
          for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
            if(r_findCI(*siter,iter+1))
              return(true);
        }
          return(false);

  /*
  ** NOOP- und START-Zustand
  */
      case CState::START:
      case CState::NOOP:
  /*
  ** r_findCI wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_findCI(*siter,iter))
            return(true);
        return(false);

  /*
  ** JOKER-Zustand (Beliebiges Zeichen)
  */
      case CState::JOKER:
  /*
  ** r_findCI wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_findCI(*siter,iter+1))
            return(true);

        return(false);

    }

    return(false);
  }

//*****************************************************************************************

  template<typename Type>
  bool r_find(int state, Type iter, Type &end) const {
  /*
  ** Hat der Automat seinen Endzustand erreicht?
  ** => Muster wurde erkannt.
  */
    if(m_states[state].m_type==CState::END) {
      end=iter;
      return(true);
    }
  /*
  ** Ist das Ende der Zeichenkette erreicht, es müssen
  ** aber noch CHAR-Zustände des Automaten ausgewertet
  ** werden => Muster wurde nicht erkannt
  */
    if((*iter==0)&&(m_states[state].m_type!=CState::NOOP))
      return(false);

  /*
  ** Status auswerten
  */
    switch(m_states[state].m_type) {
  /*
  ** CHAR-Zustand
  */
      case CState::CHAR:
  /*
  ** Das aktuelle Zeichen der Zeichenkette wird mit
  ** den Zeichen des Zustands verglichen.
  ** Stimmt es nicht überein, dann wurde das Muster
  ** nicht erkannt.
  */
        if(std::find(m_states[state].m_characters.begin(),m_states[state].m_characters.end(),*iter)==m_states[state].m_characters.end()) {
          return(false);
        } else {
  /*
  ** Bei Übereinstimmung wird rfind
  ** rekursiv für jeden Folgezustand mit dem nächsten
  ** Zeichen der Zeichenkette aufgerufen.
  */
          for(vector<int>::const_iterator siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
            if(r_find(*siter,iter+1,end))
              return(true);
        }
          return(false);

  /*
  ** NOOP- und START-Zustand
  */
      case CState::START:
      case CState::NOOP:
  /*
  ** rfind wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(vector<int>::const_iterator siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_find(*siter,iter,end))
            return(true);
        return(false);

  /*
  ** JOKER-Zustand (Beliebiges Zeichen)
  */
      case CState::JOKER:
  /*
  ** rfind wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(vector<int>::const_iterator siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_find(*siter,iter+1,end))
            return(true);

        return(false);

    }

    return(false);
  }

//*****************************************************************************************

  template<typename Type>
  bool r_match(int state, Type iter, Type &end) const {
  /*
  ** Hat die Zeichenkette ihr Ende erreicht?
  */
    if(iter==end) {
  /*
  ** Hat der Automat seinen Endzustand erreicht?
  ** => Muster wurde erkannt.
  */
      if(m_states[state].m_type==CState::END)
        return(true);
  /*
  ** Muss der Automat noch Zeichen auswerten?
  ** => Muster wurde nicht erkannt.
  */
      if(m_states[state].m_type!=CState::NOOP)
        return(false);
    }
  /*
  ** Status auswerten
  */
    std::vector<int>::const_iterator siter;
    switch(m_states[state].m_type) {
  /*
  ** CHAR-Zustand
  */
      case CState::CHAR:
  /*
  ** Das aktuelle Zeichen der Zeichenkette wird mit
  ** den Zeichen des Zustands verglichen.
  ** Stimmt es nicht überein, dann wurde das Muster
  ** nicht erkannt.
  */
        if(std::find(m_states[state].m_characters.begin(),m_states[state].m_characters.end(),*iter)==m_states[state].m_characters.end()) {
          return(false);
        } else {
  /*
  ** Bei Übereinstimmung wird rmatch
  ** rekursiv für jeden Folgezustand mit dem nächsten
  ** Zeichen der Zeichenkette aufgerufen.
  */
          for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
            if(r_match(*siter,iter+1,end))
              return(true);
        }
          return(false);

  /*
  ** NOOP- und START-Zustand
  */
      case CState::START:
      case CState::NOOP:
  /*
  ** rmatch wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_match(*siter,iter,end))
            return(true);
        return(false);

  /*
  ** JOKER-Zustand (Beliebiges Zeichen
  */
      case CState::JOKER:
  /*
  ** rmatch wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_match(*siter,iter+1, end))
            return(true);

        return(false);
    }
    return(false);
  }

//*****************************************************************************************

  template<typename Type>
  bool r_matchCI(int state, Type iter, Type &end) const {
  /*
  ** Hat die Zeichenkette ihr Ende erreicht?
  */
    if(iter==end) {
  /*
  ** Hat der Automat seinen Endzustand erreicht?
  ** => Muster wurde erkannt.
  */
      if(m_states[state].m_type==CState::END)
        return(true);
  /*
  ** Muss der Automat noch Zeichen auswerten?
  ** => Muster wurde nicht erkannt.
  */
      if(m_states[state].m_type!=CState::NOOP)
        return(false);
    }
  /*
  ** Status auswerten
  */
    std::vector<int>::const_iterator siter;
    switch(m_states[state].m_type) {
  /*
  ** CHAR-Zustand
  */
      case CState::CHAR:
  /*
  ** Das aktuelle Zeichen der Zeichenkette wird ohn Berücksichtigung
  ** der Groß-/Kleinschreibung mit den Zeichen des Zustands verglichen.
  ** Stimmt es nicht überein, dann wurde das Muster
  ** nicht erkannt.
  */
        if(std::find_if(m_states[state].m_characters.begin(),m_states[state].m_characters.end(),CCharCIPredicate(*iter))==m_states[state].m_characters.end()) {
          return(false);
        } else {
  /*
  ** Bei Übereinstimmung wird rmatch
  ** rekursiv für jeden Folgezustand mit dem nächsten
  ** Zeichen der Zeichenkette aufgerufen.
  */
          for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
            if(r_matchCI(*siter,iter+1,end))
              return(true);
        }
          return(false);

  /*
  ** NOOP- und START-Zustand
  */
      case CState::START:
      case CState::NOOP:
  /*
  ** rmatch wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_matchCI(*siter,iter,end))
            return(true);
        return(false);

  /*
  ** JOKER-Zustand (Beliebiges Zeichen
  */
      case CState::JOKER:
  /*
  ** rmatch wird rekursiv für jeden Folgezustand
  ** mit dem aktuellen Zeichen der Zeichenkette aufgerufen.
  */
        for(siter=m_states[state].m_nextstates.begin();siter!=m_states[state].m_nextstates.end();++siter)
          if(r_matchCI(*siter,iter+1, end))
            return(true);

        return(false);
    }
    return(false);
  }

//*****************************************************************************************


public:
  class EParseError {};
  CAutomat(const std::string &pattern="?*") {
    build(pattern);
    minimalize();
  }
  void rebuild(const std::string &pattern) {
    build(pattern);
    minimalize();
  }
  void showAutomat();
  void minimalize();

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CAutomat &a);
  friend CBinaryIStream &operator>>(CBinaryIStream &is, CAutomat &a);

//*****************************************************************************************

  template<typename Type>
  size_t find(Type &s, size_t spos=0) const{
    for(Type::iterator iter=s.begin()+spos; iter!=s.end(); ++iter,++spos)
      if(r_find(0, iter))
        return(spos);

    return(Type::npos);
  }

//*****************************************************************************************

  template<typename Type>
  size_t findCI(Type &s, size_t spos=0) const{
    for(Type::iterator iter=s.begin()+spos; iter!=s.end(); ++iter,++spos)
      if(r_findCI(0, iter))
        return(spos);

    return(Type::npos);
  }

//*****************************************************************************************

  template<typename Type>
  size_t rfind(Type &s, size_t spos) const{
    if (spos==Type::npos)
      spos=s.length()-1;
    for(Type::iterator iter=s.begin()+spos; iter!=s.end(); --iter,--spos)
      if(r_find(0, iter))
        return(spos);

    return(Type::npos);
  }

//*****************************************************************************************

  template<typename Type>
  size_t multiple_find(Type &s,
                       int nth,
                       size_t start=0,
                       bool complete=true) const{
  /*
  ** Ungültige Werte?
  */
    if(nth<1||start<0)
      return(Type::npos);
  /*
  ** Bis zum n-ten Vorkommen durchlaufen
  */
    while(nth--) {
      Type::iterator iter, end;
  /*
  ** Position im String suchen, an der Muster erkannt wird.
  */
      for(iter=s.begin()+start; iter!=s.end(); ++iter,++start)
        if(r_find(0, iter,end))
          break;
  /*
  ** Wurde Muster nicht gefunden?
  */
      if(iter==s.end())
        return(Type::npos);
  /*
  ** n-te Position erreicht?
  */
      if(!nth)
        return(start);
  /*
  ** Complete==true?
  **  => Suchposition um eins verschieben
  ** complete==false?
  **  => Suchposition um Länge des Teilstrings verschieben
  */
      start+=(complete)?distance(iter, end):1;
    }
    return(Type::npos);
  }

//*****************************************************************************************

  template<typename Type>
  size_t multiple_rfind(Type &s,
                        int nth,
                        size_t start,
                        bool complete) const{
  /*
  ** Ungültige Werte?
  */
    if(nth<1||start<0)
      return(Type::npos);

    if(start==Type::npos)
      start=s.length()-1;
  /*
  ** Bis zum n-ten Vorkommen durchlaufen
  */
    while(nth--) {
      Type::iterator iter, end;
  /*
  ** Position im String suchen, an der Muster erkannt wird.
  */
      for(iter=s.begin()+start; iter!=s.end(); --iter,--start)
        if(r_find(0, iter,end))
          break;
  /*
  ** Wurde Muster nicht gefunden?
  */
      if(iter==s.end())
        return(Type::npos);
  /*
  ** n-te Position erreicht?
  */
      if(!nth)
        return(start);
  /*
  ** Zukünftige Suchposition außerhalb des Strings?
  */
      if(start<(static_cast<size_t>((complete)?distance(iter, end):1)))
        return(Type::npos);
  /*
  ** Complete==true?
  **  => Suchposition um eins verschieben
  ** complete==false?
  **  => Suchposition um Länge des Teilstrings verschieben
  */
      start-=(complete)?distance(iter, end):1;
    }
    return(Type::npos);
  }

//*****************************************************************************************

  template<typename Type>
  int count(Type &s,
            size_t start=0,
            bool complete=true) const{
  /*
  ** Ungültige Werte?
  */
    if(start<0)
      return(static_cast<int>(Type::npos));

    int nth=0;
  /*
  ** Den String komplett durchlaufen
  */
    while(start<s.length()) {
      Type::iterator iter, end;
  /*
  ** Position im String suchen, an der Muster erkannt wird.
  */
      for(iter=s.begin()+start; iter!=s.end(); ++iter,++start)
        if(r_find(0, iter,end))
          break;
  /*
  ** Teilstring gefunden?
  **  => Anzahl um eins erhöhen
  ** Wenn nicht => Fertig.
  */
      if(iter==s.end())
        return(nth);
      else
        nth++;
  /*
  ** Complete==true?
  **  => Suchposition um eins verschieben
  ** complete==false?
  **  => Suchposition um Länge des Teilstrings verschieben
  */
      start+=(complete)?distance(iter, end):1;
    }
    return(nth);
  }

//*****************************************************************************************

  template<typename Type>
  bool match(Type &s, size_t spos, size_t len) const{
    if((len!=Type::npos)&&((spos+len)>s.size()))
      return(false);          // Bereich überschritten
    Type::const_iterator b=s.begin()+spos;
    Type::const_iterator e=(len!=Type::npos)?s.begin()+spos+len:s.end();
    return(r_match(0, b, e));
  }

//*****************************************************************************************

  template<typename Type>
  bool matchCI(Type &s, size_t spos, size_t len) const{
    if((len!=Type::npos)&&((spos+len)>s.size()))
      return(false);          // Bereich überschritten
    Type::const_iterator b=s.begin()+spos;
    Type::const_iterator e=(len!=Type::npos)?s.begin()+spos+len:s.end();
    return(r_matchCI(0, b, e));
  }

//*****************************************************************************************


};


}; // ende namespace Codebook

#endif