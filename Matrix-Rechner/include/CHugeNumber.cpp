#include "CHugeNumber.h"


#include "StringFunctions.h"
#include "CFileInfo.h"

namespace Codebook {

using namespace std;


//*****************************************************************************
//  CDigit
//*****************************************************************************

CHugeNumber::CDigit::CDigit(int w) {
  if((w<0)||(w>9))
    throw out_of_range("CHugeNumber::CDigit::CDigit()");
  m_digit=w;
}

//*********

CHugeNumber::CDigit::CDigit(char w) {
  if((w<'0')||(w>'9'))
    throw out_of_range("CHugeNumber::CDigit::CDigit()");
  m_digit=w-'0';
}

//*********

bool CHugeNumber::CDigit::operator+=(int d) {
  m_digit+=d;
/*
** Überlauf?
** => Überlauf abziehen und zurückgeben
*/
  if(m_digit>9) {
    m_digit%=10;
    return(true);
  }
  else
    return(false);
}

//*********

bool CHugeNumber::CDigit::operator+=(const CDigit &d) {
  m_digit+=d.m_digit;
/*
** Überlauf?
** => Überlauf abziehen und zurückgeben
*/
  if(m_digit>9) {
    m_digit%=10;
    return(true);
  }
  else
    return(false);
}

//*********

bool CHugeNumber::CDigit::operator-=(int d) {
/*
** Unterlauf?
** => Unterlauf hinzufügen und zurückgeben
*/
  if(m_digit>=d) {
    m_digit-=d;
    return(false);
  }
  else {
    m_digit=m_digit+10-d;
    return(true);
  }
}

//*********

bool CHugeNumber::CDigit::operator-=(const CDigit &d) {
/*
** Unterlauf?
** => Unterlauf hinzufügen und zurückgeben
*/
  if(m_digit>=d.m_digit) {
    m_digit-=d.m_digit;
    return(false);
  }
  else {
    m_digit=m_digit+10-d.m_digit;
    return(true);
  }
}

//*********

bool CHugeNumber::CDigit::operator==(const CDigit &d) const{
  return(d.m_digit==m_digit);
}

//*********

bool CHugeNumber::CDigit::operator!=(const CDigit &d) const{
  return(d.m_digit!=m_digit);
}

//*********

bool CHugeNumber::CDigit::operator<(const CDigit &d) const{
  return(m_digit<d.m_digit);
}

//*********

bool CHugeNumber::CDigit::operator>(const CDigit &d) const{
  return(m_digit>d.m_digit);
}

//*********

bool CHugeNumber::CDigit::operator<=(const CDigit &d) const{
  return(m_digit<=d.m_digit);
}

//*********

bool CHugeNumber::CDigit::operator>=(const CDigit &d) const{
  return(m_digit>=d.m_digit);
}


//*****************************************************************************
//  CHugeNumber
//*****************************************************************************

CHugeNumber::CHugeNumber(const std::string &w) {
  string str;
/*
** Ein - am Anfang der Zahl?
** => m_sign auf -1 setzen und - ignorieren
*/
  if(w[0]=='-') {
    m_sign=-1;
    str=w.substr(1);
  }
/*
** Ein + am Anfang der Zahl?
** => m_sign auf 1 setzen und + ignorieren
*/
  else if(w[0]=='+') {
    m_sign=1;
    str=w.substr(1);
  }
/*
** Kein Vorzeichen?
** => m_sign auf 1 setzen
*/
  else {
    m_sign=1;
    str=w;
  }
/*
** Im String hat erste Ziffer größte Wertigkeit, in CHugeNumber
** erste Ziffer kleinste Wertigkeit
** => String von hinten nach vorne übertragen
*/
  string::reverse_iterator i=str.rbegin();
  for(;i!=str.rend(); ++i)
    m_number.push_back(CDigit(*i));
/*
** Eventuell führende Nullen löschen
*/
  shorten(m_number);
}

//*********

CHugeNumber::CHugeNumber(long w) {
/*
** Vorzeichen ermitteln
*/
  m_sign=(w<0)?-1:1;
/*
** Vorzeichenlose Zahl in einen String umwandeln
** und mit Reverse-Iteratoren übertragen
*/
  string s=toString(labs(w));
  for(string::reverse_iterator i=s.rbegin(); i!=s.rend(); ++i)
    m_number.push_back(CDigit(*i));
}

//*********

CHugeNumber::CHugeNumber(unsigned long w) {
/*
** Unsigned-Werte sind immer positiv
*/
  m_sign=1;
/*
** Zahl in einen String umwandeln
** und mit Reverse-Iteratoren übertragen
*/
  string s=toString(w);
  for(string::reverse_iterator i=s.rbegin(); i!=s.rend(); ++i)
    m_number.push_back(CDigit(*i));
}

//*********

CHugeNumber::CHugeNumber(int w) {
/*
** Vorzeichen ermitteln
*/
  m_sign=(w<0)?-1:1;
/*
** Vorzeichenlose Zahl in einen String umwandeln
** und mit Reverse-Iteratoren übertragen
*/
  string s=toString(abs(w));
  for(string::reverse_iterator i=s.rbegin(); i!=s.rend(); ++i)
    m_number.push_back(CDigit(*i));
}

//*********

CHugeNumber::CHugeNumber(const number_type &v, short sign)
: m_number(v), m_sign(sign)
{}

//*********

CHugeNumber::CHugeNumber(unsigned long low, unsigned long high) {
  *this=high*CHugeNumber("4294967296")+low;
}

//*********

void CHugeNumber::increase(CHugeNumber::number_type &v, int p) {
  bool overflow;
  do {
/*
** Übertrag auf eine nicht exisiterende Ziffer?
** => Ziffer hinzufügen
*/
    if(p==v.size())
      v.push_back(0);
/*
** Ziffer um 1 erhöhen und Übertrag speichern
*/
    overflow=v[p++]+=1;
/*
** Übertrag vorhanden?
** => An nächste Ziffer weitergeben
*/
  } while(overflow);
}

//*********

void CHugeNumber::decrease(CHugeNumber::number_type &v, int p) {
  bool overflow;
  do {
/*
** Ziffer um 1 vermindern und Übertrag speichern
*/
    overflow=v[p++]-=1;
/*
** Übertrag vorhanden?
** => An nächste Ziffer weitergeben
*/
  } while(overflow);
/*
** Eventuell entstandende führende Nullen kürzen
*/
  shorten(v);
}

//*********

CHugeNumber::number_type CHugeNumber::add(const CHugeNumber::number_type &v1,
                                          const CHugeNumber::number_type &v2,
                                          unsigned int p) {
  number_type r=v1;

/*
** Existiert Ziffer im ersten Summanden nicht?
** => Nullen hinzufügen
*/
  while(r.size()<=p)
    r.push_back(0);

/*
** Alle Ziffern werden addiert und
** ein Übertrag bei der nächst höheren Ziffer berücksichtigt
*/
  int overflow=(r[p++]+=v2[0]);

  for(unsigned int i=1; i<v2.size(); i++) {
    if(p==r.size())
      r.push_back(0);
    overflow=(r[p++]+=(v2[i].m_digit+overflow));
  }
/*
** Zweiter Summand komplett hinzuaddiert, aber noch Übertrag
** => increase erledigt den Rest
*/
  if(overflow)
    increase(r,p);
/*
** Eventuell zuviel angehängte Nullen werden gekürzt
*/
  shorten(r);
  return(r);
}

//*********

CHugeNumber::number_type CHugeNumber::sub(const CHugeNumber::number_type &v1,
                                           const CHugeNumber::number_type &v2) {

/*
** Invariante: Erster Operand muss größer sein als zweiter
*/
  if(compare(v1,v2)>0)
    throw out_of_range("CHugeNumber::_sub()");

  number_type r=v1;
/*
** Alle Ziffern werden subtrahiert und
** ein Übertrag bei der nächst höheren Ziffer berücksichtigt
*/
  int p=0;  
  int overflow=(r[p++]-=v2[0]);

  for(unsigned int i=1; i<v2.size(); i++)
    overflow=(r[p++]-=(v2[i].m_digit+overflow));
/*
** Zweiter Operand komplett abgezogen, aber noch Übertrag
** => decrease erledigt den Rest
*/
  if(overflow)
    decrease(r,p);
/*
** Eventuell enstandene führende Nullen werden gekürzt
*/
  shorten(r);
  return(r);
}

//*********

CHugeNumber::number_type CHugeNumber::mul(const CHugeNumber::number_type &v1,
                                              const CHugeNumber::number_type &v2) {
  number_type r(1);
/*
** Multiplikation mit einstelligem Faktor?
*/
  if(v2.size()==1) {
    int dvalue=v2.front().m_digit;
    if(dvalue) {
      int addpos=0;
/*
** Bilden der Teilprodukte mit anschließendem
** versetzten Aufaddieren
*/
      for(number_type::const_iterator iter=v1.begin(); iter!=v1.end(); ++iter,++addpos)
        r=add(r,CHugeNumber(iter->m_digit*dvalue).m_number,addpos);
      }
  }
/*
** Beliebige Multiplikation
*/
  else {
    int addpos=0;
/*
** Reduktion der Multiplikation auf Multiplikation mit
** einem einstelligen Faktor.
** Anschließend werden die Teilprodukte versetzt addiert
*/
    for(number_type::const_iterator iter=v2.begin(); iter!=v2.end(); ++iter,++addpos)
      r=add(r,(v1*iter->m_digit).m_number,addpos);
  }
/*
** Eventuell führende Nullen werden gekürzt
*/
  shorten(r);
  return(r);
}

//*********

CHugeNumber::number_type CHugeNumber::div(const CHugeNumber::number_type &v1,
                                           const CHugeNumber::number_type &v2) {

  number_type r;
/*
** v2 größer v1?
** => Ergebnis = 0
*/
  if(compare(v1,v2)>0) {
    r.push_back(0);
    return(r);
  }

  number_type::const_reverse_iterator iter=v1.rbegin();
  number_type part;

  while(true) {
/*
** So lange Ziffern aus der Hauptzahl holen und
** Nullen im Ergebnis eintragen, wie part< v2 gilt
*/
    while((compare(part,v2)>0)&&(iter!=v1.rend())) {

      part.push_front(*(iter++));
      shorten(part);
      if(compare(part,v2)>0)
        r.push_front(0);
    }

/*
** Keine Ziffern mehr in v1 und part noch kleiner v2?
** => Berechnung beendet
*/
    if((iter==v1.rend())&&(compare(part,v2)>0)) {
      shorten(r);
      return(r);
    }

/*
** Bestimmen, wie oft v2 in part passt
*/
    CDigit digit(0);
    while(compare(part,v2)<=0) {
      part=sub(part,v2);
      digit+=1;
    }
/*
** Anzahl als Ziffer zum Ergebnis hinzufügen
*/
    r.push_front(digit);
  }
}

//*********

CHugeNumber::number_type CHugeNumber::mod(const CHugeNumber::number_type &v1,
                                           const CHugeNumber::number_type &v2) {

/*
** v2 größer v1?
** => Ergebnis = v1
*/
  if(compare(v1,v2)>0)
    return(v1);

  number_type::const_reverse_iterator iter=v1.rbegin();
  number_type part;

  while(true) {
/*
** So lange Ziffern aus der Hauptzahl holen und
** Nullen im Ergebnis eintragen, wie part< v2 gilt
*/
    while((compare(part,v2)>0)&&(iter!=v1.rend())) {

      part.push_front(*(iter++));
      shorten(part);
    }

/*
** Keine Ziffern mehr in v1 und part noch kleiner v2?
** => Berechnung beendet
*/
    if((iter==v1.rend())&&(compare(part,v2)>0))
      return(part);
/*
** Bestimmen, wie oft v2 in part passt
*/
    while(compare(part,v2)<=0)
      part=sub(part,v2);
  }
}

//*********

void CHugeNumber::shorten(number_type &v) {
/*
** Alle führenden Nullen löschen.
** Beim Wert 0 eine Null stehen lassen
*/
  while((v.size()>1)&&(v[v.size()-1]==0))
    v.pop_back();
}

//*********

bool CHugeNumber::isZero(const CHugeNumber &n) {
  for(CHugeNumber::number_type::const_iterator iter=n.m_number.begin(); iter!=n.m_number.end(); ++iter)
    if(iter->m_digit!=0)
      return(false);
  return(true);
}


//*********

CHugeNumber &CHugeNumber::operator+=(const CHugeNumber &n) {
/*
** Bei gleichem Vorzeichen das aktuelle Vorzeichen beibehalten
** Und Zahlen addieren
*/
  if(m_sign==n.m_sign) {
    m_number=add(m_number,n.m_number);
    return(*this);
  }
/*
** Wenn eigenes Vozeichen + und das der anderen Zahl -
** => Ermitteln, welche Zahl den größeren Absolutwert besitzt.
*/
  if(m_sign==1)
/*
** Eigener Absolutwert kleiner als der der anderen Zahl?
** => Eigene Zahl von der anderen Zahl abziehen, Vozeichen ist -
*/
    if(CHugeNumber::compare(m_number,n.m_number)>0) {
      m_number=sub(n.m_number,m_number);
      m_sign=-1;
      return(*this);
    }
/*
** Eigener Absolutwert größer als der der anderen Zahl
** Andere Zahl von der eigenen Zahl abziehen, Vozeichen beibehalten
*/
    else {
      m_number=sub(m_number,n.m_number);
      return(*this);
    }
/*
** Eigenes Vozeichen ist - und das der anderen Zahl +
** Ermitteln, welche Zahl den größeren Absolutwert besitzt.
*/
  else
/*
** Eigener Absolutwert kleiner als der der anderen Zahl?
** => Eigene Zahl von der anderen Zahl abziehen, Vozeichen ist +
*/
    if(CHugeNumber::compare(m_number,n.m_number)>0) {
      m_number=sub(n.m_number,m_number);
      m_sign=1;
      return(*this);
    }
/*
** Eigener Absolutwert größer als der der anderen Zahl
** Andere Zahl von der eigenen Zahl abziehen, Vozeichen beibehalten
*/
    else {
      m_number=sub(m_number,n.m_number);
      return(*this);
    }
}

//*********

CHugeNumber &CHugeNumber::operator-=(const CHugeNumber &n) {
  *this+=CHugeNumber(n.m_number,-n.m_sign);
  return(*this);
}

//*********

CHugeNumber &CHugeNumber::operator*=(const CHugeNumber &n) {
  m_number=mul(m_number,n.m_number);
  m_sign*=n.m_sign;
  return(*this);
}

//*********

CHugeNumber &CHugeNumber::operator/=(const CHugeNumber &n) {
  m_number=div(m_number,n.m_number);
  m_sign*=n.m_sign;
  return(*this);
}

//*********

CHugeNumber &CHugeNumber::operator%=(const CHugeNumber &n) {
  m_number=mod(m_number,n.m_number);
//  *this=*this-(*this/n*n);
  m_sign=1;
  return(*this);
}

//*********

CHugeNumber operator+(const CHugeNumber &n1, const CHugeNumber &n2) {
/*
** n1 und n2 haben das gleiche Vorzeichen?
** => Vorzeichen übernehmen und Zahlen addieren
*/
  if(n1.m_sign==n2.m_sign)
    return(CHugeNumber(CHugeNumber::add(n1.m_number,n2.m_number),n1.m_sign));
/*
** n1 positiv und n2 negativ?
** => Absolutwerte vergleichen
*/
  if(n1.m_sign==1)
/*
** Absolutwert von n1 kleiner als der von n2?
** => Differenz n2-n1 bilden, Vorzeichen ist -
*/
    if(CHugeNumber::compare(n1.m_number,n2.m_number)>0)
      return(CHugeNumber(CHugeNumber::sub(n2.m_number,n1.m_number),-1));
/*
** Absolutwert von n1 größer als der von n2
** Differenz n1-n2 bilden, Vorzeichen ist +
*/
    else
      return(CHugeNumber(CHugeNumber::sub(n1.m_number,n2.m_number),1));
/*
** n2 positiv und n1 negativ
** Absolutwerte vergleichen
*/
  else
/*
** Absolutwert von n1 kleiner als der von n2?
** => Differenz n2-n1 bilden, Vorzeichen ist +
*/
    if(CHugeNumber::compare(n1.m_number,n2.m_number)>0)
      return(CHugeNumber(CHugeNumber::sub(n2.m_number,n1.m_number),1));
/*
** Absolutwert von n1 größer als der von n2
** Differenz n1-n2 bilden, Vorzeichen ist -
*/
    else
      return(CHugeNumber(CHugeNumber::sub(n1.m_number,n2.m_number),-1));
}

//*********

CHugeNumber operator-(const CHugeNumber &n1, const CHugeNumber &n2) {
  return(n1+CHugeNumber(n2.m_number,-n2.m_sign));
}

//*********

CHugeNumber operator*(const CHugeNumber &n1, const CHugeNumber &n2) {
  return(CHugeNumber(CHugeNumber::mul(n1.m_number,n2.m_number),n1.m_sign*n2.m_sign));

}

//*********

CHugeNumber operator/(const CHugeNumber &n1, const CHugeNumber &n2) {
  return(CHugeNumber(CHugeNumber::div(n1.m_number,n2.m_number),n1.m_sign*n2.m_sign));

}

//*********

CHugeNumber CHugeNumber::operator%(const CHugeNumber &n) const{
  CHugeNumber result;
  result.m_number=mod(m_number,n.m_number);
  result.m_sign=1;
  return(result);
}

//*********

CHugeNumber CHugeNumber::operator<<(int w) const{
  CHugeNumber::number_type r(m_number.size()+w);
  CHugeNumber::number_type::iterator riter=r.begin()+w;
/*
** Ziffern um w Stellen nach links schieben und
** freiwerdende Stellen mit 0 füllen
** Vorzeichen wird beibehalten
*/
  for(CHugeNumber::number_type::const_iterator i=m_number.begin(); i!=m_number.end(); ++i, ++riter)
    *riter=*i;
  return(r);
}

//*********

CHugeNumber CHugeNumber::operator>>(int w) const{
  CHugeNumber::number_type r(m_number.size()-w);
  CHugeNumber::number_type::iterator riter=r.begin();
/*
** Ziffern um w Stellen nach rechts schieben und
** freiwerdende Stellen mit 0 füllen
** Vorzeichen wird beibehalten
*/
  for(CHugeNumber::number_type::const_iterator i=m_number.begin()+w; i!=m_number.end(); ++i, ++riter)
    *riter=*i;
  return(r);
}

//*********

CHugeNumber &CHugeNumber::operator++() {
/*
** Null?, dann Vorzeichen richtig setzen.
*/
  if(isZero(m_number))
    m_sign=1;
/*
** Je nach Vorzeichen erhöhen oder vermindern
*/
  if(m_sign==1)
    increase(m_number);
  else
    decrease(m_number);
  return(*this);
}

//*********

CHugeNumber &CHugeNumber::operator--() {
/*
** Null?, dann Vorzeichen richtig setzen.
*/
  if(isZero(m_number))
    m_sign=-1;
/*
** Je nach Vorzeichen erhöhen oder vermindern
*/
  if(m_sign==1)
    decrease(m_number);
  else
    increase(m_number);
  return(*this);
}

//*********

CHugeNumber CHugeNumber::operator++(int) {
  CHugeNumber tmp=*this;
  ++(*this);
  return(tmp);
}

//*********

CHugeNumber CHugeNumber::operator--(int) {
  CHugeNumber tmp=*this;
  --(*this);
  return(tmp);
}

//*********

ostream &operator<<(ostream &ostr, const CHugeNumber &n) {
  string str;
  if((n.m_sign==-1)&&(!CHugeNumber::isZero(n)))
    str+= "-";
  for(CHugeNumber::number_type::const_reverse_iterator i=n.m_number.rbegin(); i!=n.m_number.rend(); ++i)
    str+= static_cast<char>(i->m_digit+'0');
  ostr << str;
  return(ostr);
}

//*********

string CHugeNumber::getAsPointedNumber() const {
  string str;
  if((m_sign==-1)&&(!CHugeNumber::isZero(m_number)))
    str+="-";
  size_t i=m_number.size();
  for(number_type::const_reverse_iterator iter=m_number.rbegin(); iter!=m_number.rend(); ++iter, --i) {
    if((i!=m_number.size())&&((i%3)==0))
      str+=".";
    str+=iter->m_digit+'0';
  }
  return(str);
}

//*********

string CHugeNumber::getAsString() const {
  string str;
  if((m_sign==-1)&&(!CHugeNumber::isZero(m_number)))
    str+="-";
  for(number_type::const_reverse_iterator iter=m_number.rbegin(); iter!=m_number.rend(); ++iter)
    str+=iter->m_digit+'0';
  return(str);
}

//*********

CFileSize CHugeNumber::getAsFileSize() const {
  CHugeNumber v("4294967296"), high(*this/v), low(*this%v);
  return(CFileSize(low.getAsUnsignedLong(), high.getAsUnsignedLong()));
}

/*
** Invariante: Beide Zahlen müssen mit _shorten gekürzt sein
*/
int CHugeNumber::compare(const CHugeNumber::number_type &v1, const CHugeNumber::number_type &v2) {
/*
** Besitzt eine Zahl mehr Ziffern als die andere,
** muss sie zwangsläufig größer sein.
*/
  if(v1.size()<v2.size())
    return(1);
  if(v1.size()>v2.size())
    return(-1);
/*
** Bei gleicher Ziffernzahl müssen die einzelnen
** Ziffern verglichen werden
*/
  number_type::const_reverse_iterator iter1=v1.rbegin();
  number_type::const_reverse_iterator iter2=v2.rbegin();
  while((iter1!=v1.rend())&&(*iter1==*iter2)) {
    ++iter1;
    ++iter2;
  }
/*
** Unterscheiden sich zwei Ziffern, dann ist die Zahl
** mit der größeren Ziffer auch die größere Zahl.
*/
  if(iter1==v1.rend())
    return(0);
  if((*iter1)<(*iter2))
    return(1);
  else
    return(-1);
}

//*********

bool CHugeNumber::operator<(const CHugeNumber &n) const {
/*
** Sind die Vozzeichen der beiden Zahlen unterschiedlich?
** => Zahl mit positivem Vorzeichen ist die größere
*/
  if(m_sign!=n.m_sign)
    if(m_sign==1)
      return(false);
    else
      return(true);
  else
/*
** Sind Vorzeichen gleich, so muss über compare
** entschieden werden.
*/
    if(m_sign==1)
      return(compare(m_number,n.m_number)>0);
    else
      return(compare(m_number,n.m_number)<0);
}

//*********

bool CHugeNumber::operator==(const CHugeNumber &n) const {
/*
** Zwei Zahlen sind gleich, wenn ihr Vorzeichen
** und ihr Absolutwert gleich sind.
*/
  return((m_sign==n.m_sign)&&(compare(m_number,n.m_number)==0));
}

//*********

bool CHugeNumber::operator>(const CHugeNumber &n) const {
  return(n<*this);
}

//*********

bool CHugeNumber::operator!=(const CHugeNumber &n) const {
  return(!(*this==n));
}

//*********

bool CHugeNumber::operator>=(const CHugeNumber &n) const {
  return(!(*this<n));
}

//*********

bool CHugeNumber::operator<=(const CHugeNumber &n) const {
  return(!(n<*this));
}

//*********

unsigned long CHugeNumber::getAsUnsignedLong() const {
  unsigned long result=0;
  unsigned long bit=1;
  CHugeNumber tmp=*this;
  CHugeNumber two(2);
/*
** Laufe so lange, wie noch Bits in der Zahl gesetzt sind.
*/
  for(int i=0; (i<32)&&(!isZero(tmp)); i++) {
/*
** Ergibt Division durch zwei einen Rest?
** => Das niederwertigste Bit ist gesetzt.
*/
    CHugeNumber div=tmp/two;
    if(div*two<tmp) {
      result|=bit;
    }
/*
** Zahl durch zwei Teilen und Eergebnis um ein
** Bit nach links schieben.
*/
    tmp=div;
    bit<<=1;
  }
  return(result);
}

//*********

long CHugeNumber::getAsLong() const {
  long result=0;
  long bit=1;
  CHugeNumber tmp=*this;
  CHugeNumber two(2);
/*
** Laufe so lange, wie noch Bits in der Zahl gesetzt sind.
*/
  for(int i=0; (i<32)&&(!isZero(tmp)); i++) {
/*
** Ergibt Division durch zwei einen Rest?
** => Das niederwertigste Bit ist gesetzt.
*/
    CHugeNumber div=tmp/two;
    if(div*two<tmp) {
      result|=bit;
    }
/*
** Zahl durch zwei Teilen und Eergebnis um ein
** Bit nach links schieben.
*/
    tmp=div;
    bit<<=1;
  }
/*
** Bei negativem Vorzeichen den long-Wert
** mit -1 multiplizieren
*/
  if(m_sign==-1)
    result*=-1;
  return(result);
}



}; // namespace Codebook
