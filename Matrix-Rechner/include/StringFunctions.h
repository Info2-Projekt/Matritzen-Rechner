#ifndef __STRINGFUNCTIONS_H
#define __STRINGFUNCTIONS_H

#include "STLFunctions.h"
#include "IString.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

namespace Codebook {

long hexToValue(std::string s);
void getString(std::string &s, int a);
char tolower(char c);
char toupper(char c);
bool isalnum(char c);
bool isalpha(char c);
bool iscntrl(char c);
bool isdigit(char c);
bool isgraph(char c);
bool islower(char c);
bool isprint(char c);
bool isupper(char c);
bool isxdigit(char c);

std::string toUpperString(const std::string &org);
std::string toLowerString(const std::string &org);
bool isNumericString(const std::string &s);
//std::vector<std::string> chopIntoWords(const std::string &org, const std::string &separators=" \t\n");
std::string capitalize(const std::string &org);
std::string toWord(unsigned long val, bool upcase=true);
size_t multiple_find(const std::string &s,
                          const std::string &sub,
                          int nth,
                          size_t start=0,
                          bool complete=true);
size_t multiple_rfind(const std::string &s,
                           const std::string &sub,
                           int nth,
                           size_t start=std::string::npos,
                           bool complete=true);
int count(const std::string &s, const std::string &sub, size_t start=0, bool complete=true);
std::string toRoman(unsigned int val, bool upcase=true);
unsigned int romanToUnsignedInt(const std::string &str);

template<class Type>
std::string toString(Type val) {
  std::ostringstream o;
  o << val;
  return(o.str());
}

template<class Type>
std::string toString(Type val, int width, bool left=false, char fill=' ') {
  std::ostringstream o;
  if(left)
    o << setfill(fill) << setw(width) << std::left << val;
  else
    o << setfill(fill) << setw(width) << val;
  return(o.str());
}

template<typename Type>
std::string valueToHex(Type v, int s=2) {
  std::ostringstream ostr;
  ostr << std::setfill('0') << std::setw(s) << std::hex << v;
  return(ostr.str());
}

std::vector<std::string> chopIntoWords(const std::string &org, const std::string &separators=" \t\n");

std::vector<insensitive_string> chopIntoWords(const insensitive_string &org, const std::string &separators);

template<typename Type>
std::vector<Type> chopIntoWords(const Type &org, const std::string &separators=" \t\n") {

  std::vector<Type> words;
  Type::const_iterator en, be=org.begin();

  do {
/*
** Nach dem ersten Separator-Zeichen suchen
*/
	  en=std::find_first_of(be,org.end(),
                     separators.begin(),separators.end());
/*
** Steht ein Wort vor dem gefunden Separtor?
*/
    if(be!=en) {
/*
** String zu Vektor hinzufügen und Wort hinein kopieren
*/
      words.push_back("");
	  std::copy(be,en,std::back_inserter(words.back()));
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




}; // namespace Codebook
#endif

