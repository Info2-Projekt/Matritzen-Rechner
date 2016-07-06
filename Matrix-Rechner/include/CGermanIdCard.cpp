
#include "CGermanIdCard.h"
#include "CShifter.h"
#include "StringFunctions.h"
#include <iostream>

using namespace std;

namespace Codebook {

CGermanIdCard::CGermanIdCard(const std::string &id) {
/*
** Wesentliche Bereiche isolieren
*/
  string serial=id.substr(0,10);
  string birth=id.substr(13,7);
  string expires=id.substr(21,7);
  string check=id.substr(34,1);
/*
** Bestehen Teile nur aus Zahlen?
*/
  if((!isNumericString(serial))||(!isNumericString(birth))||(!isNumericString(expires))||(!isNumericString(check)))
    throw invalid_argument("CGermanIdCard::CGermanIdCard");
/*
** Unterste Ausweiszeile zusammensetzen
*/
  m_id=serial+"D<<"+birth+"<"+expires+"<<<<<<"+check;

  if(id.size()!=35)
    throw invalid_argument("CGermanIdCard::CGermanIdCard");
  m_id=id;
}

//*********

CGermanIdCard::CGermanIdCard(const std::string &serial,
                             const std::string &birth,
                             const std::string &expires,
                             const std::string &check) {
/*
** Längen der einzelnen Teilbereiche überprüfen
*/
  if((serial.size()!=10)||(birth.size()!=7)||(expires.size()!=7)||(check.size()!=1))
    throw invalid_argument("CGermanIdCard::CGermanIdCard");
/*
** Bestehen Teile nur aus Zahlen?
*/
  if((!isNumericString(serial))||(!isNumericString(birth))||(!isNumericString(expires))||(!isNumericString(check)))
    throw invalid_argument("CGermanIdCard::CGermanIdCard");
/*
** Unterste Ausweiszeile zusammensetzen
*/
  m_id=serial+"D<<"+birth+"<"+expires+"<<<<<<"+check;
  if(m_id.size()!=35)
    throw invalid_argument("CGermanIdCard::CGermanIdCard");
}

//*********

bool CGermanIdCard::checkChecksum(const string &s) {
  CShifter<int> shifter(7,3,1);

  int chk=0;
/*
** Jede Ziffer mit entsprechendem Shifter-Wert
** multiplizieren und Ergebnisse aufaddieren
** Ergebnis Modulo 10 mit Prüfziffer vergleichen
*/
  for(unsigned int i=0; i<s.size()-1; ++i)
    chk+=atoi(s.substr(i,1).c_str())*shifter++;
  return((chk%10)==atoi(s.substr(s.size()-1,1).c_str()));
}

//*********


bool CGermanIdCard::isIdValid() {
/*
** Einzelne zu prüfende Teile extrahieren
*/
  string serial=m_id.substr(0,10);
  string birth=m_id.substr(13,7);
  string expiration=m_id.substr(21,7);
/*
** Relevante Teile für gesamte Prüfziffer zusammensetzen
*/
  string compID=serial+birth+expiration+m_id.substr(34,1);
/*
** Prüfziffern überprüfen
*/
  return(checkChecksum(serial)&&
         checkChecksum(birth)&&
         checkChecksum(expiration)&&
         checkChecksum(compID));
}

//*********

CDate CGermanIdCard::getAge() {
/*
** Erst einmal davon ausgehen, dass Person in
** diesem Jahrhundert geboren wurde.
*/
  CDate date(atoi(m_id.substr(17,2).c_str()),
             atoi(m_id.substr(15,2).c_str()),
             atoi(m_id.substr(13,2).c_str())+2000);
/*
** Ist Person jünger als 10 oder noch
** gar nicht geboren?
** => Person muss im vorigen Jahrhundert geboren worden sein
*/
  if(CDate::calcAge(date)<10)
    date.setYear(date.getYear()-100);

  return(date);
}

//*********

bool CGermanIdCard::isOfAge() {
  return(CDate::calcAge(getAge())>=18);
}

//*********

CDate CGermanIdCard::getExpirationDate() {
/*
** Erst einmal davon ausgehen, dass Ausweis
** in diesem Jahrhundert abläuft
*/
  CDate date(atoi(m_id.substr(25,2).c_str()),
             atoi(m_id.substr(23,2).c_str()),
             atoi(m_id.substr(21,2).c_str())+2000);
/*
** Ist Ausweis noch länger als 10 Jahre gültig?
** => Ausweis muss bereits im letzten Jahrhundert abgelaufen sein
*/
  CDate today;
  if(date.getYear()-today.getYear()>10)
    date.setYear(date.getYear()-100);
  return(date);
}

//*********

bool CGermanIdCard::isExpired() {
  CDate today;
  return(getExpirationDate()<today);
}

}; // namespace Codebook

