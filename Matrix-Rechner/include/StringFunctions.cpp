#include "StringFunctions.h"

#include "STLFunctions.h"
#include <iostream>
#include <locale>

namespace Codebook {

using namespace std;

namespace CBLocale {
  const locale Locale("German_Germany");
  const ctype<char> &Facet=use_facet<ctype<char> >(Locale,0,true);
};


long hexToValue(string s) {
  return(strtol(s.c_str(),0,16));
}

void getString(string &s, int a) {
  char *buff=new char[a];
  cin.getline(buff,a);
  s=buff;
  delete(buff);
}

using namespace CBLocale;

char tolower(char c) {
  return(Facet.tolower(c));
}

//*********

char toupper(char c) {
  return(Facet.toupper(c));
}

//*********

bool isalnum(char c) {
  return(Facet.is(ctype_base::alnum,c));
}

//*********

bool isalpha(char c) {
  return(Facet.is(ctype_base::alpha,c));
}

//*********

bool iscntrl(char c) {
  return(Facet.is(ctype_base::cntrl,c));
}

//*********

bool isdigit(char c) {
  return(Facet.is(ctype_base::digit,c));
}

//*********

bool isgraph(char c) {
  return(Facet.is(ctype_base::graph,c));
}

//*********

bool islower(char c) {
  return(Facet.is(ctype_base::lower,c));
}

//*********

bool isprint(char c) {
  return(Facet.is(ctype_base::print,c));
}

//*********

bool isupper(char c) {
  return(Facet.is(ctype_base::upper,c));
}

//*********

bool isxdigit(char c) {
  return(Facet.is(ctype_base::xdigit,c));
}

string toUpperString(const string &org) {
  string s=org;
  size_t length=s.size();
  string::iterator iter=s.begin();
  while(length--) {
    *iter=toupper(*iter);
    ++iter;
  }
  return(s);
}

string toLowerString(const string &org) {
  string s=org;
  size_t length=s.size();
  string::iterator iter=s.begin();
  while(length--) {
    *iter=tolower(*iter);
    ++iter;
  }
  return(s);
}


string capitalize(const string &org) {
  string s=org;
  size_t length=s.size();
/*
** Erstes Zeichen auf Großbuchstaben setzen
*/
  if(length)
    s[0]=toupper(s[0]);
/*
** Alle Zeichen des Strings durchlaufen
*/
  for(size_t i=1;i<length; ++i) {
/*
** Aktuellen Zeichen ein Buchstabe und
** das vorherige Zeichen kein Buchstabe?
**  => Aktuelles Zeichen in Großbuchstaben umwandeln
*/
    if(isalpha(s[i])&&!isalpha(s[i-1]))
      s[i]=toupper(s[i]);
    else
      s[i]=tolower(s[i]);
  }
  return(s);
}

string upTo19(unsigned long val, bool one=true) {
  switch(val) {
    case 1: if(one)
              return("eins");
            else
              return("ein");
   
    case 2: return("zwei");
    case 3: return("drei");
    case 4: return("vier");
    case 5: return("fuenf");
    case 6: return("sechs");
    case 7: return("sieben");
    case 8: return("acht");
    case 9: return("neun");
    case 10: return("zehn");
    case 11: return("elf");
    case 12: return("zwoelf");
    case 17: return("siebzehn");
    default: return(upTo19(val%10)+"zehn");
  }
}

string upTo99(unsigned long val, bool one) {

  if(val<20)
      return(upTo19(val,one));

  string word;
  if(val%10)
    word=upTo19(val%10,false)+"und";

  switch(val/10) {
    case 2: word+="zwanzig"; break;
    case 3: word+="dreissig"; break;
    case 4: word+="vierzig"; break;
    case 5: word+="fuenfzig"; break;
    case 6: word+="sechzig"; break;
    case 7: word+="siebzig"; break;
    case 8: word+="achtzig"; break;
    case 9: word+="neunzig"; break;
  }
  return(word);
}

string upTo999(unsigned long val, bool one) {
  if(val<100)
    return(upTo99(val,one));

  string word=upTo19(val/100,false);
  word+="hundert";

  if(val%100)
    word+=upTo99(val%100, one);

  return(word);
}

string underMillion(unsigned long val) {
  string word;
  if(val>=1000) {
    word=upTo999(val/1000, false);
    word+="tausend";
    }

  if(val%1000)
    word+=upTo999(val%1000, true);

  return(word);
}

string underBillion(unsigned long val) {
  string word;
  if(val>=1000000) {
      unsigned long  mil=val/1000000;
      if(mil==1)
        word="einemillion";
      else {
        word=upTo999(mil, false);
        word+="millionen";
        }
      val%=1000000;
    }
  word+=underMillion(val);
  return(word);
}

string underTrillion(unsigned long val) {
  string word;
  if(val>=1000000000) {
    unsigned long mil=val/1000000000;
    if(mil==1)
      word="einemilliarde";
    else {
      word=upTo999(mil,false);
      word+="milliarden";
      }
    val%=1000000000;
  }
  word+=underBillion(val);
  return(word);
}

string toWord(unsigned long val, bool upcase) {
  string word;
  if(val==0)
    word=("null");
  else
    word=underTrillion(val);

  if(upcase)
    word[0]=toupper(word[0]);

  return(word);
}

size_t multiple_find(const string &s,
                     const string &sub,
                     int nth,
                     size_t start,
                     bool complete) {
/*
** Ungültige Werte?
*/
  if(nth<1||start<0)
    return(string::npos);
/*
** Erstes Vorkommen ab Position start finden
*/
  size_t fpos=s.find(sub,start);
  nth--;
/*
** Bis zum n-ten Vorkommen durchlaufen
*/
  while(nth--) {
/*
** Complete==true?
**  => Suchposition um eins verschieben
** complete==false?
**  => Suchposition um Länge des Teilstrings verschieben
*/
    fpos+=(complete)?sub.length():1;
    fpos=s.find(sub,fpos);

    if(fpos==string::npos)
      return(fpos);
  }
  return(fpos);
}

size_t multiple_rfind(const string &s,
                      const string &sub,
                      int nth,
                      size_t start,
                      bool complete) {
/*
** Ungültige Werte?
*/
  if(nth<1||start<0)
    return(string::npos);
/*
** Letztes Vorkommen ab Position start finden
*/
  size_t fpos=s.rfind(sub,start);
  nth--;
/*
** Bis zum n-ten Vorkommen zurück laufen
*/
  while(nth--) {
/*
** Neue Startposition < 0?
**  => Nicht gefunden
*/
    if((fpos<static_cast<size_t>((complete)?sub.length():1)))
      return(string::npos);
/*
** Complete==true?
**  => Suchposition um eins zurücksetzen
** complete==false?
**  => Suchposition um Länge des Teilstrings zurücksetzen
*/
    fpos-=(complete)?sub.length():1;
    fpos=s.rfind(sub,fpos);

    if(fpos==string::npos)
      return(fpos);
  }
  return(fpos);
}

int count(const string &s,
          const string &sub,
          size_t start,
          bool complete) {
/*
** Ungültiger Startwert?
*/
  if(start<0)
    return(0);

  int nth=0;

/*
** Solange Suchposition noch innerhalb des Strings
*/
  while(start<s.length()) {

  start=s.find(sub,start);
/*
** Teilstring gefunden?
**  => Anzahl um eins erhöhen
** Wenn nicht => Fertig.
*/
  if(start==string::npos)
    return(nth);
  else
    nth++;
/*
** Complete==true?
**  => Suchposition um eins verschieben
** complete==false?
**  => Suchposition um Länge des Teilstrings verschieben
*/
  start+=(complete)?sub.length():1;
  }
  return(nth);
}

string toRoman(unsigned int val, bool upcase) {
/*
** Statische Umwandlungstabellen setzen
*/
  static unsigned int intValues[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};
  static char *romanValues[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};

/*
** Wenn 0, dann leeren String zurückgeben
*/
  if(val==0)
    return("");
  string str;
  int pos=0;

/*
** Schritt für Schritt Werte aus intValues abziehen
** und Zeichenketten aus romanValues zum Ergebnis hinzufügen
*/
  while(val) {
    if(val>=intValues[pos]) {
      str+=romanValues[pos];
      val-=intValues[pos];
    }
    else
      pos++;
  }
/*
** Sollen Kleinbuchstaben erzeugt werden?
** => toLowerString aufrufen
*/
  if(upcase)
    return(str);
  else
    return(toLowerString(str));
}

unsigned int romanToUnsignedInt(const string &str) {
/*
** Statische Umwandlungstabellen setzen
*/
  static unsigned int intValues[]={900,400,90,40,9,4,1000,500,100,50,10,5,1};
  static char *romanValues[]={"CM","CD","XC","XL","IX","IV","M","D","C","L","X","V","I"};

/*
** Römische Zahl in Großbuchstaben umwandeln
*/
  string roman=toUpperString(str);
  size_t pos=0;
  unsigned int value=0;
/*
** Schritt für Schritt nach Stücken römischer Zahlen im String
** suchen und den Gegenwert zu value hinzu addieren
*/
  while(pos<roman.size()) {
    int x;
    for(x=0; x<13; x++)
      if(roman.substr(pos,strlen(romanValues[x]))==romanValues[x]) {
        pos+=strlen(romanValues[x]);
        value+=intValues[x];
        break;
      }
/*
** Wurde kein Stück gefunden?
** => keine gültige römische Zahl
*/
    if(x==13)
      throw invalid_argument("romanToUnsignedInt()");
  }
  return(value);
}

bool isNumericString(const string &s) {
  for(string::const_iterator iter=s.begin(); iter!=s.end(); ++iter)
    if(!isdigit(*iter))
      return(false);
  return(true);
}


vector<string> chopIntoWords(const string &org, const string &separators) {

  vector<string> words;
  string::const_iterator en, be=org.begin();

  do {
/*
** Nach dem ersten Separator-Zeichen suchen
*/
	  en=find_first_of(be,org.end(),
                     separators.begin(),separators.end());
/*
** Steht ein Wort vor dem gefunden Separtor?
*/
    if(be!=en) {
/*
** String zu Vektor hinzufügen und Wort hinein kopieren
*/
      words.push_back("");
	  for(string::const_iterator i=be; i!=en; ++i)
		words.back()+=*i;
    }
/*
** Ab dem gefundenen Separator-Zeichen alle direkt
** nachfolgenden Separator-Zeichen überspringen
*/
    be=find_first_not_of(en,org.end(),
                         separators.begin(),separators.end());
  } while(be!=org.end());

  return(words);
}


vector<insensitive_string> chopIntoWords(const insensitive_string &org, const string &separators) {

  vector<insensitive_string> words;
  insensitive_string::const_iterator en, be=org.begin();

  do {
/*
** Nach dem ersten Separator-Zeichen suchen
*/
	  en=find_first_of(be,org.end(),
                     separators.begin(),separators.end());
/*
** Steht ein Wort vor dem gefunden Separtor?
*/
    if(be!=en) {
/*
** String zu Vektor hinzufügen und Wort hinein kopieren
*/
      words.push_back("");
	  for(insensitive_string::const_iterator i=be; i!=en; ++i)
		words.back()+=*i;
    }
/*
** Ab dem gefundenen Separator-Zeichen alle direkt
** nachfolgenden Separator-Zeichen überspringen
*/
    be=find_first_not_of(en,org.end(),
                         separators.begin(),separators.end());
  } while(be!=org.end());

  return(words);
}


}; // Ende namespace Codebook
