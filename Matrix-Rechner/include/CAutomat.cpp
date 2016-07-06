#include"CAutomat.h"
#include <iostream>

namespace Codebook {

using namespace std;

void CAutomat::build(const string &pattern) {
  m_states.clear();
/*
** Startzustand des Automaten anlegen
*/
  m_states.push_back(CState(CState::START));
/*
** Iterator auf erstes Zeichen der das Muster
** enthaltenden Zeichenkette anlegen
*/
  string::const_iterator i=pattern.begin();
/*
** Startzustand von expression als Folgezustand
** des Automaten-Startzustandes eintragen
*/
  m_states[0].m_nextstates.push_back(expression(i));
/*
** Endzustand anlegen
*/
  m_states.push_back(CState(CState::END));
}



int CAutomat::expression(string::const_iterator &i) {
/*
** Anlegen des Knotens, der die einzelnen Terme
** als Folgeknoten besitzen wird.
*/
  int bstate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
/*
** Der Vektor f�r die Aufnahme der Endzust�nde der
** Teilzust�nde
*/
  vector<int> estates;
/*
** Ersten Term umwandeln und als Folgezustand eintragen
*/
  m_states[bstate].m_nextstates.push_back(bstate+1);
  term(i);
  estates.push_back(static_cast<int>(m_states.size())-1);
/*
** Eventuell weitere Terme umwandeln und als Folgezusta�nde
** eintragen
*/
  while(*i=='|') {
    m_states[bstate].m_nextstates.push_back(term(++i));
    estates.push_back(static_cast<int>(m_states.size())-1);
  }
/*
** Endzustand f�r expression anlegen
*/
  int estate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
  m_states.back().m_nextstates.push_back(estate+1);
/*
** Den Endzustand von expression als Folgezustand der einzelnen
** Teilautomaten eintragen
*/
  for(vector<int>::iterator iter=estates.begin(); iter!=estates.end(); ++iter) {
    m_states[*iter].m_nextstates.clear();
    m_states[*iter].m_nextstates.push_back(estate);
  }
  return(bstate);
}



int CAutomat::term(string::const_iterator &i) {
/*
** factor aufrufen f�r ersten Factor
*/
  int bstate=factor(i);
/*
** So lange factor aufrufen, wie kein komplexer
** Ausdruck beendet wurde.
*/
  while((*i!=0)&&(*i!='|')&&(*i!=')')&&(*i!=']')) factor(i);
  return(bstate);
}

int CAutomat::factor(string::const_iterator &i) {
  int bstate;
/*
** F�r geklammerten Ausdruck expression aufrufen
*/
  if(*i=='(') {
    bstate=expression(++i);
    if((*(i++))!=')')
      throw(EParseError());
  }
/*
** F�r Auswahl von Zeichen set aufrufen
*/
  else if(*i=='{') {
    bstate=set(++i);
    if((*(i++))!='}')
      throw(EParseError());
  }
/*
** F�r optionalen Ausdruck option aufrufen
*/
  else if(*i=='[') {
    bstate=option(++i);
    if((*(i++))!=']')
      throw(EParseError());
  }
/*
** Ansonsten �ber character einzelnes Zeichen in Zustand umwandeln
*/
  else
    bstate=character(i);

/*
** Ist der soeben bearbeitete Faktor mit einem H�llen-Operator versehen,
** dann hull oder poshull aufrufen.
*/
  if(*i=='*') {
    i++;
    bstate=hull(bstate);
  }
  else if(*i=='+') {
    i++;
    bstate=poshull(bstate);
  }
  return(bstate);
}

int CAutomat::option(string::const_iterator &i) {
/*
** Technisch bedingten NOOP-Zustand als
** Startzustand des Teilautomaten anlegen
*/
  int bstate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
/*
** Optionalen Ausdruck in Automaten umwandeln
*/
  expression(i);
/*
** Technisch bedingten NOOP-Zustand als
** Startzustand des Teilautomaten anlegen
*/
  int estate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
/*
** Verbindungen zwischen Start- und Endzustand
** des Teilautomaten und dem optionalen Ausdruck
** herstellen
*/
  m_states[bstate].m_nextstates.push_back(estate);
  m_states[bstate].m_nextstates.push_back(bstate+1);
  m_states[estate].m_nextstates.push_back(estate+1);
  return(bstate);
}

int CAutomat::hull(int state) {
/*
** Ersten Zustand der H�lle sowie technisch
** bedingten Zustand erzeugen
*/
  int bstate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
  int estate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
/*
** Der erste Zustand der H�lle bekommt als Folgezust�nde
** den ersten Zustand des zur H�lle geh�renden
** Ausdrucks, sowie den technisch bedingen Zustand
*/
  m_states[bstate].m_nextstates.push_back(estate);
  m_states[bstate].m_nextstates.push_back(state);
/*
** Wie �blich bekommt der letzte Zustand den
** Zustand mit n�chsth�herem Index als Folgezustand
*/
  m_states[estate].m_nextstates.push_back(estate+1);
/*
** Um eine leere H�lle zu erm�glichen, muss der
** Vorg�ngerzustand des ersten Zustand des Ausdrucks
** als Folgezustand den ersten Zustand der
** H�lle bekommen
*/
  m_states[state-1].m_nextstates.clear();
  m_states[state-1].m_nextstates.push_back(bstate);
  return(bstate);
}

int CAutomat::poshull(int state) {
/*
** Ersten Zustand der H�lle sowie technisch
** bedingten Zustand erzeugen
*/
  int bstate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
  int estate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::NOOP));
/*
** Der erste Zustand der H�lle bekommt als Folgezust�nde
** den ersten Zustand des zur H�lle geh�renden
** Ausdrucks, sowie den technisch bedingen Zustand
*/
  m_states[bstate].m_nextstates.push_back(estate);
  m_states[bstate].m_nextstates.push_back(state);
/*
** Wie �blich bekommt der letzte Zustand den
** Zustand mit n�chsth�herem Index als Folgezustand
*/
  m_states[estate].m_nextstates.push_back(estate+1);
/*
** Der Folgezustand des Vorg�ngerzustandes des ersten
** Zustandes des H�llenausdrucks bekommt explizit den
** ersten Zustand des H�llenausdrucks als Folgezustand
*/
  m_states[state-1].m_nextstates.clear();
  m_states[state-1].m_nextstates.push_back(state);
  return(bstate);
}

int CAutomat::character(string::const_iterator &i) {
  int bstate=static_cast<int>(m_states.size());
/*
** Wenn es sich bei Zeichen um Joker handelt, dann
** speziellen JOKER-Zustand erzeugen.
*/
  if(*i=='?') {
    m_states.push_back(CState(CState::JOKER));
  } else {
/*
** Handelt es sich um das Zeichen ~ (Beginn eines
** <special>-Zeichens), dann �berspringen
*/
    if(*i=='~') ++i;
/*
** CHAR-Zustand f�r Zeichen erzeugen
*/
    m_states.push_back(CState(CState::CHAR));
    m_states.back().m_characters.push_back(*i);
  }
/*
** Wie �blich bekommt der letzte Zustand den
** Zustand mit n�chsth�herem Index als Folgezustand
*/
  m_states.back().m_nextstates.push_back(bstate+1);
  ++i;
  return(bstate);
}

int CAutomat::set(string::const_iterator &i) {
/*
** Der Zustand f�r die Auswahl von Zeichen
** wird erzeugt.
*/
  int bstate=static_cast<int>(m_states.size());
  m_states.push_back(CState(CState::CHAR));
/*
** Alle Zeichen innerhalb der geschweiften Klammern
** werden in den Zustand �bertragen
*/
  while(*i!='}') {
    if(*i=='~') ++i;
    m_states.back().m_characters.push_back(*i);
    i++;
  }
/*
** Wie �blich bekommt der letzte Zustand den
** Zustand mit n�chsth�herem Index als Folgezustand
*/
  m_states.back().m_nextstates.push_back(bstate+1);
  return(bstate);
}






void CAutomat::showAutomat() {
  size_t maxwidth=0;
  for(vector<CState>::iterator iter=m_states.begin(); iter!=m_states.end(); ++iter)
    if(maxwidth<iter->m_characters.size())
      maxwidth=iter->m_characters.size();

  string empty(255,' ');
  for(size_t i=0; i<m_states.size(); i++) {
    cout << setw(3) << static_cast<int>(i) << " ";
    if(m_states[i].m_type==CState::NOOP)
      cout << "NOP";
    if (m_states[i].m_type==CState::JOKER)
      cout << "JOK";
    if (m_states[i].m_type==CState::START)
      cout << "STA";
    if (m_states[i].m_type==CState::END)
      cout << "END";
    if (m_states[i].m_type==CState::CHAR) {
      cout << "CHR ";
      cout << empty.substr(0,maxwidth-m_states[i].m_characters.size());
      for(vector<char>::iterator citer=m_states[i].m_characters.begin();
          citer!=m_states[i].m_characters.end();
          ++citer)
        cout << *citer;
    }
    else
      cout << empty.substr(0,maxwidth+1);

    for(vector<int>::iterator siter=m_states[i].m_nextstates.begin();
        siter!=m_states[i].m_nextstates.end();
        ++siter)
      cout << setw(3) << *siter;
    cout << endl;
  }
}






void CAutomat::minimalize() {
/*
** Alle Zust�nde des Automaten durchlaufen
*/
  int idx=0;
  vector<CState>::iterator iter=m_states.begin();
  while(iter!=m_states.end()) {
/*
** Handelt es sich um einen NOOP-Zustand?
*/
    if((iter->m_type==CState::NOOP)) {
/*
** Alle Knoten durchlaufen
*/
      for(vector<CState>::iterator liter=m_states.begin(); liter!=m_states.end(); ++liter) {
        if(iter==liter)
          continue;
/*
** Ist ein Folgezustand der zu l�schende NOOP-Knoten?
*/
        vector<int>::iterator fpos=std::find(liter->m_nextstates.begin(), liter->m_nextstates.end(), idx);
        if(fpos!=liter->m_nextstates.end()) {
/*
** Verweis auf zu l�schenden NOOP-Knoten l�schen und
** stattdessen Folgezust�nde des zu l�schenden NOOP-
** Knotens �bernehmen
*/
          fpos=liter->m_nextstates.erase(fpos);
          copy(iter->m_nextstates.begin(),iter->m_nextstates.end(), inserter(liter->m_nextstates,fpos));
        }
/*
** Alle Folgezust�nde durchlaufen
*/
        for(vector<int>::iterator siter=liter->m_nextstates.begin(); siter!=liter->m_nextstates.end(); ++siter)
/*
** Alle Verweise auf Zust�nde mit Index > idx
** auf n�chstniederen Index setzen
*/ 
          if(*siter>idx)
            (*siter)--;

      }
/*
** NOOP-Knoten l�schen
*/
      iter=m_states.erase(iter);
    }
    else {
      iter++;
      idx++;
    }
  }
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CAutomat &a) {
  os << a.m_states;
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CAutomat &a) {
  is >> a.m_states;
  return(is);
}


}; // ende namespace Codebook