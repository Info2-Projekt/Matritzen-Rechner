
#include "CGermanPassport.h"
#include "CGermanIdCard.h"
#include "CShifter.h"
#include "StringFunctions.h"

#include <iostream>

using namespace std;

namespace Codebook {

CGermanPassport::CGermanPassport(const std::string &id) {
/*
** Wesentliche Bereiche isolieren
*/
  string serial=id.substr(0,10);
  string birth=id.substr(13,7);
  string sex=id.substr(20,1);
  string expires=id.substr(21,7);
  string check=id.substr(34,1);
/*
** Bestehen Teile nur aus Zahlen?
*/
  if((!isNumericString(serial))||(!isNumericString(birth))||(!isNumericString(expires))||(!isNumericString(check)))
    throw invalid_argument("CGermanPassport::CGermanPassport");
/*
** Gültige Geschlechtsangabe?
*/
  if((sex!="M")&&(sex!="F"))
    throw invalid_argument("CGermanPassport::CGermanPassport");
/*
** Unterste Passzeile zusammensetzen
*/
  m_id=serial+"D<<"+birth+sex+expires+"<<<<<<<<<<<<<<<"+check;

  if(id.size()!=44)
    throw invalid_argument("CGermanPassport::CGermanPassport");
  m_id=id;
}

//*********

CGermanPassport::CGermanPassport(const std::string &serial,
                                 const std::string &birth,
                                 const std::string &sex,
                                 const std::string &expires,
                                 const std::string &check) {
/*
** Längen der einzelnen Teilbereiche überprüfen
*/
  if((serial.size()!=10)||(birth.size()!=7)||(expires.size()!=7)||(check.size()!=1))
    throw invalid_argument("CGermanPassport::CGermanPassport");
/*
** Bestehen Teile nur aus Zahlen?
*/
  if((!isNumericString(serial))||(!isNumericString(birth))||(!isNumericString(expires))||(!isNumericString(check)))
    throw invalid_argument("CGermanPassport::CGermanPassport");
/*
** Gültige Geschlechtsangabe?
*/
  if((sex!="M")&&(sex!="F"))
    throw invalid_argument("CGermanPassport::CGermanPassport");
/*
** Unterste Passzeile zusammensetzen
*/
  m_id=serial+"D<<"+birth+sex+expires+"<<<<<<<<<<<<<<<"+check;
  if(m_id.size()!=44)
    throw invalid_argument("CGermanPassport::CGermanPassport");
}


//*********

bool CGermanPassport::isIdValid() {
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
  return(CGermanIdCard::checkChecksum(serial)&&
         CGermanIdCard::checkChecksum(birth)&&
         CGermanIdCard::checkChecksum(expiration)&&
         CGermanIdCard::checkChecksum(compID));
}

//*********

CDate CGermanPassport::getAge() {
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

bool CGermanPassport::isOfAge() {
  return(CDate::calcAge(getAge())>=18);
}

//*********

CDate CGermanPassport::getExpirationDate() {
/*
** Erst einmal davon ausgehen, dass der Pass
** in diesem Jahrhundert abläuft
*/
  CDate date(atoi(m_id.substr(25,2).c_str()),
             atoi(m_id.substr(23,2).c_str()),
             atoi(m_id.substr(21,2).c_str())+2000);
/*
** Ist der Pass noch länger als 10 Jahre gültig?
** => Pass muss bereits im letzten Jahrhundert abgelaufen sein
*/
  CDate today;
  if(date.getYear()-today.getYear()>10)
    date.setYear(date.getYear()-100);
  return(date);
}

//*********

bool CGermanPassport::isExpired() {
  CDate today;
  return(getExpirationDate()<today);
}

//*********

bool CGermanPassport::isMale() {
  return(m_id.substr(20,1)=="M");
}

//*********

bool CGermanPassport::isFemale() {
  return(m_id.substr(20,1)=="F");
}

}; // namespace Codebook

