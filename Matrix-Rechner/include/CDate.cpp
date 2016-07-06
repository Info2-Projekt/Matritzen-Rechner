#include "CDate.h"
#include "StringFunctions.h"
#include "IString.h"
#include <time.h>
#include <sstream>
#include <iomanip>

namespace Codebook {

using namespace std;

bool CDate::isLeapyear(int year) {

  if(year<1)
    throw out_of_range("isLeapyear");

  if(year%4) return(false);
  if(year<=1582) return(true);
  if(year%100) return(true);
  if(year%400) return(false);
  return(true);
}
size_t test;
int CDate::daysPerMonth(int month, int year) {
  switch(month) {
  case 1: case 3: case 5: case 7:
  case 8: case 10: case 12:
    return(31);

  case 4: case 6: case 9: case 11:
    return(30);

  case 2: return(28+isLeapyear(year));

  default:
    throw out_of_range("daysPerMonth");
  }
}

long CDate::calcDaysPast(int day, int month, int year) {
/*
** Grenzen überprüfen
*/
  if((day<1)||(month<1)||(year<1)||
     (month>12)||(day>daysPerMonth(month, year))||
     ((year==1582)&&(month==10)&&(day>=5)&&(day<=14)))
    throw out_of_range("calcDaysPast");
/*
** Invariante: Der 1.1.0001 ist Tag 0
*/
  long daysPast=0;
  int x=1;
/*
** Tage bis zum aktuellen Jahr addieren
*/
  while(x<year)
    daysPast+=365+isLeapyear(x++);
  x=1;
/*
** Tage bis zum aktuellen Monat addieren
*/
  while(x<month)
    daysPast+=daysPerMonth(x++,year);
/*
** Tage des aktuellen Monats addieren
*/
  daysPast+=day-1;
/*
** Lag Datum nach dem 14.10.1582?
** => 10 Tage abziehen wegen Lücke
*/
  if(daysPast>577736)
    daysPast-=10;
  return(daysPast);
}

string CDate::calcZodiacSign(int day, int month) {
  switch(month) {
    case 1:
      if(day<=20)
        return("Steinbock");
      else
        return("Wassermann");

    case 2:
      if(day<=19)
        return("Wassermann");
      else
        return("Fische");

    case 3:
      if(day<=20)
        return("Fische");
      else
        return("Widder");

    case 4:
      if(day<=20)
        return("Widder");
      else
        return("Stier");

    case 5:
      if(day<=21)
        return("Stier");
      else
        return("Zwillinge");

    case 6:
      if(day<=21)
        return("Zwillinge");
      else
        return("Krebs");

    case 7:
      if(day<=22)
        return("Krebs");
      else
        return("Löwe");

    case 8:
      if(day<=23)
        return("Löwe");
      else
        return("Jungfrau");

    case 9:
      if(day<=23)
        return("Jungfrau");
      else
        return("Waage");

    case 10:
      if(day<=23)
        return("Waage");
      else
        return("Skorpion");

    case 11:
      if(day<=22)
        return("Skorpion");
      else
        return("Schütze");

    case 12:
      if(day<=21)
        return("Schütze");
      else
        return("Steinbock");

    default:
      throw(out_of_range("calcZodiacsign"));
  }
}

string CDate::calcZodiacSign() {
  return(calcZodiacSign(m_day,m_month));
}


string CDate::calcWeekday(int day, int month, int year) {
/*
** Invariante: Der 1.1.0001 ist ein Samstag 
*/
  switch((calcDaysPast(day,month,year)+5)%7) {
    case 0: return("Montag");
    case 1: return("Dienstag");
    case 2: return("Mittwoch");
    case 3: return("Donnerstag");
    case 4: return("Freitag");
    case 5: return("Samstag");
    case 6: return("Sonntag");
    default: throw out_of_range("calcWeekday");

  }
}

CDate CDate::calcEasterSunday(int year) {
  if(year<1)
    throw(out_of_range("CDate::calcEasterSunday"));
  if(year>1582) {
    int A = year % 19;
    int B = year / 100;
    int C = year % 100;
    int D = B / 4;
    int E = B % 4;
    int F = (B + 8) / 25;
    int G = (B - F + 1) / 3;
    int H = (19*A + B - D - G + 15) % 30;
    int I = C / 4;
    int K = C % 4;
    int L = (32 + 2*E + 2*I - H - K) % 7;
    int M = (A + 11*H + 22*L) / 451;
    int P = (H + L - 7*M + 114) / 31;
    int Q = (H + L - 7*M + 114) % 31;
    return(CDate(Q+1,P,year));
  }
  else {
    int A = year % 4;
    int B = year % 7;
    int C = year % 19;
    int D = (19*C + 15) % 30;
    int E = (2*A + 4*B - D + 34) % 7;
    int F = (D + E + 114) / 31;
    int G = (D + E + 114) % 31;
    return(CDate(G+1,F,year));
  }
}

/*
** Aschermittwoch
*/
CDate CDate::calcAshWednesday(int year) {
  return(calcEasterSunday(year)-46);
}

/*
** Weiberfastnacht
*/

CDate CDate::calcWeiberfastnacht(int year) {
  return(calcEasterSunday(year)-52);
}

/*
** Pfingsten
*/
CDate CDate::calcWhitsun(int year) {
  return(calcEasterSunday(year)+49);
}
/*
** Christi Himmelfahrt
*/
CDate CDate::calcAscensionDay(int year) {
  return(calcEasterSunday(year)+39);
}

/*
** Fronleichnam
*/
CDate CDate::calcCorpusChristi(int year) {
  return(calcEasterSunday(year)+60);
}

/*
** Muttertag
*/
CDate CDate::calcMothersDay(int year) {
/*
** Zweiten Sonntag im Mai ermitteln
*/
  CDate date=CDate(1,5,year);
  date=date+(6-date.getWeekday()+7);
/*
** Fällt Muttertag auf Pfingsten
** => Muttertag eine Woche vorverlegen
*/
  if(date==calcWhitsun(year))
    date=date-7;
  return(date);
}

/*
** Anzahl der Kalenderwochen im Jahr
*/
int CDate::calcCalendarWeeksPerYear(int year) {
  if((CDate(1,1,year).getWeekday()==3)||
     (CDate(31,12,year).getWeekday()==3))
    return(53);
  else
    return(52);
}

/*
** Beginn einer bestimmten Kalenderwoche
*/
CDate CDate::getBeginOfCalendarWeek(int year, int cweek) {
/*
** Wochentag des 4.1. ermitteln
*/
  CDate begin(4,1,year);
  int day=begin.getWeekday();
/*
** Datum des Montags der Woche eritteln,
** in der der 4.1. liegt
*/
  begin=begin-day;
/*
** Anfang der gewünschten KW ermitteln
*/
  begin=begin+((cweek-1)*7);
  return(begin);
}

/*
** KW eines Datums ermitteln
*/
int CDate::getCalendarWeek() const {
/*
** Montag der Woche, in der das Datum liegt, ermitteln
*/
  CDate begin=*this-getWeekday();
/*
** Tagesdifferenz zwischen KW des Datums und
** der ersten KW ermitteln
*/
  int diff=begin-getBeginOfCalendarWeek(m_year,1);
/*
** KW des Datums ermitteln
*/
  return((diff/7)+1);
}


string CDate::getMonthSmallAsHtml(int year, int month) {
/*
** Farbe der zum Monat gehörenden Tage
*/
  static const string InColor="black";
/*
** Farbe der außerhalb des Monats liegenden Tage
*/
  static const string OutColor="#888888";

  static const string TitleColor="#000066";  // Titelfarbe
  static const int Width=25;                 // Zellenbreite

  string str;

/*
** Erzeugen der Monatsüberschrift
*/
  str+="<table border=\"1\" cellspacing=\"0\">\n";
  str+="<tr>\n";
  str+="<th colspan=\"8\"><font color=\""+TitleColor+"\">";
  str+=CDate(1,month,1).getMonthName()+" "+toString(year)+"</th>\n";
  str+="</tr>\n";
/*
** Erzeugen der Wochentags-Abkürzungen
*/
  str+="<tr>\n";
  str+="<th width=\""+toString(Width)+"\">KW</th>\n";
  str+="<th width=\""+toString(Width)+"\">Mo</th>\n";
  str+="<th width=\""+toString(Width)+"\">Di</th>\n";
  str+="<th width=\""+toString(Width)+"\">Mi</th>\n";
  str+="<th width=\""+toString(Width)+"\">Do</th>\n";
  str+="<th width=\""+toString(Width)+"\">Fr</th>\n";
  str+="<th width=\""+toString(Width)+"\">Sa</th>\n";
  str+="<th width=\""+toString(Width)+"\">So</th>\n";
  str+="</tr>\n";


  CDate date(1,month,year);
  date=date-date.getWeekday();
  int calweek=date.getCalendarWeek();
/*
** Zeilenschleife
*/
  do {
/*
** Ausgabe der KW
*/
    str+="<tr>\n";
    str+="<th align=\"right\">"+toString(calweek)+"</th>\n";
/*
** Spaltenschleife => Ausgabe der Tageszelle
*/
    for(int i=0; i<7; i++,date=date+1) {
      str+="<td align=\"right\">";
      if(date.getMonth()==month)
        str+="<font color=\""+InColor+"\">";
      else
        str+="<font color=\""+OutColor+"\">";
      str+=toString(date.getDay());
      str+="</font></td>\n";
    }
    str+="</tr>\n";
    calweek++;
  } while(date.getMonth()==month);
  
  str+="</table>\n";
  return(str);
}

int CDate::calcAge() {
  return(calcAge(*this));
}

int CDate::calcAge(const CDate &d) {
/*
** Differenz von jetzigem Jahr und
** Geburtsjahr bilden
*/
  CDate today;
  int age=today.getYear()-d.getYear();
/*
** Geburtstag in diesem Jahr noch nicht erreicht?
** => Alter um eins vermindern.
*/
  if(today.getMonth()<d.getMonth())
    return(age-1);
  else if((today.getMonth()==d.getMonth())&&(today.getDay()<d.getDay()))
    return(age-1);

  return(age);
}


string CDate::getMonthDetailedAsHtml(int year, int month) {
/*
** Hintergrundfarbe der zum Monat gehörenden Tage
*/
  static const string InBg="white";
/*
** Hintergrundfarbe der außerhalb des Monats liegenden Tage
*/
  static const string OutBg="gray";

  static const string TitleColor="#000066";  // Titelfarbe
  static const int Size=100;                 // Zellenbreite

  string str;

/*
** Erzeugen der Monatsüberschrift
*/
  str+="<table border=\"1\" cellspacing=\"0\">\n";
  str+="<tr>\n";
  str+="<th colspan=\"8\"><font color=\""+TitleColor+"\">";
  str+=CDate(1,month,1).getMonthName()+" "+toString(year)+"</th>\n";
  str+="</tr>\n";
/*
** Erzeugen der Wochentag-Texte
*/
  str+="<tr>\n";
  str+="<th>KW</th>\n";
  str+="<th width=\""+toString(Size)+"\">Montag</th>\n";
  str+="<th width=\""+toString(Size)+"\">Dienstag</th>\n";
  str+="<th width=\""+toString(Size)+"\">Mittwoch</th>\n";
  str+="<th width=\""+toString(Size)+"\">Donnerstag</th>\n";
  str+="<th width=\""+toString(Size)+"\">Freitag</th>\n";
  str+="<th width=\""+toString(Size)+"\">Samstag</th>\n";
  str+="<th width=\""+toString(Size)+"\">Sonntag</th>\n";
  str+="</tr>\n";


  CDate date(1,month,year);
  date=date-date.getWeekday();
  int calweek=date.getCalendarWeek();
/*
** Zeilenschleife
*/
  do {
/*
** Ausgabe der KW
*/
    str+="<tr>\n";
    str+="<th align=\"right\" height=\""+toString(Size)+"\">"+toString(calweek)+"</th>\n";
/*
** Spaltenschleife => Ausgabe der Tageszelle
*/
    for(int i=0; i<7; i++,date=date+1) {
      str+="<td align=\"right\" valign=\"top\" ";
      if(date.getMonth()==month)
        str+="bgcolor=\""+InBg+"\">";
      else
        str+="bgcolor=\""+OutBg+"\">";
      str+=toString(date.getDay());
      str+="</td>\n";
    }
    str+="</tr>\n";
    calweek++;
  } while(date.getMonth()==month);
  
  str+="</table>\n";
  return(str);
}


string CDate::getCalendarWeekAsHtml(int year, int cweek,
                                    const CTime &begin,
                                    const CTime &end,
                                    int intervall,
                                    int width,
                                    int height) {

  string str;

  str+="<table border=\"1\" cellspacing=\"0\">\n";
/*
** Erzeugen der Wochentag-Texte
*/
  str+="<tr>\n";
  str+="<th>"+toString(year)+"<br />KW "+toString(cweek)+"</th>\n";
  CDate date=getBeginOfCalendarWeek(year, cweek);
  str+="<th width=\""+toString(width)+"\">Mo, "+date.getDDMM()+"</th>\n";
  str+="<th width=\""+toString(width)+"\">Di, "+(date+1).getDDMM()+"</th>\n";
  str+="<th width=\""+toString(width)+"\">Mi, "+(date+2).getDDMM()+"</th>\n";
  str+="<th width=\""+toString(width)+"\">Do, "+(date+3).getDDMM()+"</th>\n";
  str+="<th width=\""+toString(width)+"\">Fr, "+(date+4).getDDMM()+"</th>\n";
  str+="<th width=\""+toString(width)+"\">Sa, "+(date+5).getDDMM()+"</th>\n";
  str+="<th width=\""+toString(width)+"\">So, "+(date+6).getDDMM()+"</th>\n";

  CTime time=begin;

/*
** Zeilenschleife
*/
  do {
/*
** Ausgabe der Uhrzeit
*/
    str+="<tr>\n";
    str+="<th align=\"right\" height=\""+toString(height)+"\">"+time.getHHMM()+"</th>\n";
/*
** Spaltenschleife
*/
    for(int i=0; i<7; i++,date=date+1) {
      str+="<td>&nbsp;</td>\n";
    }
    str+="</tr>\n";
    time=time+intervall*60*1000;
  } while(time<end);
  
  str+="</table>\n";
  return(str);
}




/* Invariante aller Funktionen:
** Der 1.1.0001 ist Tag 0
*/

// Standard-Konstruktor liefert aktuelles Datum

CDate::CDate() {
/*
** Aktuelles Datum und Uhrzeit holen
*/
  time_t t;
  time(&t);
/*
** Datum und Uhrzeit in tm-Struktur umwandeln
*/
  tm *ts;
  ts=localtime(&t);
/*
** Benötigte Daten aus Struktur auslesen
*/
  m_day=ts->tm_mday;
  m_month=ts->tm_mon+1;
  m_year=ts->tm_year+1900;

  checkRanges();
  calcDaysPast();
}


CDate::CDate(int day, int month, int year)
: m_day(day), m_month(month), m_year(year) {

  checkRanges();
  calcDaysPast();
}

CDate::CDate(long days)
: m_daysPast(days){
  if(days<0)
    throw out_of_range("CDate::CDate(long)");

  calcDate();
  checkRanges();

}

CDate::CDate(const string &date) {
/*
** Abkürzungen für Monate festlegen
*/
  static const char* const months[]={"Jan","Feb", "Mar", "Apr", "May", "Jun", 
                                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
/*
** Datum in EInzelteile zerlegen
** Es müssen genau drei Einzelteile sein
*/
  vector<string> components=chopIntoWords(date," .-");
  if(components.size()!=3)
    throw invalid_argument("CDate::CDate(string)");
/*
** Tag umwandeln
*/
  m_day=atol(components[0].c_str());
/*
** Jahr umwandeln.
** Bei zweistelligem Jahr die ersten beiden Ziffern
** des aktuellen Jahres nehmen
*/
  if(components[2].size()==4)
    m_year=atol(components[2].c_str());
  else if(components[2].size()==2)
    m_year=atol(components[2].c_str())+(CDate().getYear()/100*100);
  else
    throw invalid_argument("CDate::CDate(string)");
/*
** Datumskomponenten durch Punkte getrennt?
** => Monat ist als numerischer Wert angegeben
*/
  if(date.find(".")!=string::npos) {
    m_month=atol(components[1].c_str());
  }
/*
** Datumskomponenten durch Leerzeichen getrennt?
** => Monat ist als Abkürzung angegeben
*/
  else if((date.find(" ")!=string::npos)||(date.find("-")!=string::npos)) {
    insensitive_string m=components[1].c_str();
    for(int i=0; (i<12)&&(m!=months[i]); i++){}
    m_month=i+1;
  }
  else
    throw invalid_argument("CDate::CDate(string)");

  checkRanges();
  calcDaysPast();
}


void CDate::checkRanges() {
  if((m_year<1)||(m_year>32767))
    throw out_of_range("CDate::checkRanges");
  if((m_month<1)||(m_month>12))
    throw out_of_range("CDate::checkRanges");
  if((m_day<1)||(m_day>daysPerMonth(m_month,m_year)))
    throw out_of_range("CDate::checkRanges");

  if((m_year==1582)&&(m_month==10)&&(m_day>4)&&(m_day<15))
    throw out_of_range("CDate::checkRanges");
}


void CDate::calcDaysPast() {
  m_daysPast=0;
  int x=1;
  while(x<m_year)
    m_daysPast+=365+isLeapyear(x++);
  x=1;
  while(x<m_month)
    m_daysPast+=daysPerMonth(x++,m_year);
  m_daysPast+=m_day-1;

  if(m_daysPast>577736)
    m_daysPast-=10;
}

void CDate::calcDate() {
  long days=m_daysPast;
/*
** Datum nach 4.10.1582?
** => 10 Tage der Datumslücke aufaddieren
*/
  if(days>577736)
    days+=10;

/*
** Jahre von den Tagen abziehen, so lange
** noch ganze Jahre übrig sind
*/
  m_year=1;
  while(((days>=365)&&(!isLeapyear(m_year)))||
        ((days>=366)&&(isLeapyear(m_year)))) {
    days-=365+isLeapyear(m_year);
    m_year++;
  }
/*
** Monate von den Tagen abziehen, so lange
** noch ganze Monate übrig sind
*/
  m_month=1;
  while(days>=daysPerMonth(m_month,m_year)) {
    days-=daysPerMonth(m_month,m_year);
    m_month++;
  }
/*
** Übrig gebliebene Tag in m_day speichern
*/
  m_day=1+days;

  checkRanges();
}


string CDate::getDDMM() const {

  stringstream str;

  str.fill('0');
  str << setw(2) << m_day << ".";
  str << setw(2) << m_month << ".";
  return(str.str());

}

string CDate::getDDMMYYYY() const {

  stringstream str;

  str.fill('0');
  str << setw(2) << m_day << ".";
  str << setw(2) << m_month << ".";
  str << setw(4) << m_year;
  return(str.str());

}

int CDate::getWeekday() const{
/*
** Invariante: Der 1.1.0001 ist ein Samstag,
** Die Woche beginnt mit Montag
*/
  return((m_daysPast+5)%7);
}

string CDate::getWeekdayName() const{
/*
** Invariante: Der 1.1.0001 ist ein Samstag 
** Die Woche beginnt mit Montag
*/
  switch(getWeekday()) {
    case 0: return("Montag");
    case 1: return("Dienstag");
    case 2: return("Mittwoch");
    case 3: return("Donnerstag");
    case 4: return("Freitag");
    case 5: return("Samstag");
    case 6: return("Sonntag");
    default: throw out_of_range("CDate::getWeekdayName");

  }
}

string CDate::getMonthName() const{
  switch(m_month) {
    case 1: return("Januar");
    case 2: return("Februar");
    case 3: return("Maerz");
    case 4: return("April");
    case 5: return("Mai");
    case 6: return("Juni");
    case 7: return("Juli");
    case 8: return("August");
    case 9: return("September");
    case 10: return("Oktober");
    case 11: return("November");
    case 12: return("Dezember");
    default: throw out_of_range("getWeekdayName");

  }
}


CDate CDate::operator+(long days) const {
  return(CDate(m_daysPast+days));
}

CDate CDate::operator-(long days) const {
  return(CDate(m_daysPast-days));
}

long CDate::operator-(const CDate &d) const {
  return(labs(m_daysPast-d.m_daysPast));
}


bool CDate::operator<(const CDate &d) const {
  return(m_daysPast<d.m_daysPast);
}

bool CDate::operator==(const CDate &d) const {
  return(m_daysPast==d.m_daysPast);
}

CBinaryOStream &operator<<(CBinaryOStream &os, const CDate &d) {
  os << d.m_daysPast;
  return(os);
}

CBinaryIStream &operator>>(CBinaryIStream &is, CDate &d) {
  is >> d.m_daysPast;
  d.calcDate();
  d.checkRanges();
  return(is);
}



}; // namespace Codebook
