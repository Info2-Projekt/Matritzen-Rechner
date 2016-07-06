#include "MathFunctions.h"
#include "PrimeNumbers.h"


#include <iostream>

namespace Codebook {

using namespace std;

void getPrimeNumbers(unsigned long v, vector<unsigned long> &result) {

  if(v<2)
    return;

  unsigned long i;
/*
** Ein entsprechend gro�es Feld reservieren
** Und Endewert bestimmen
*/
  vector<unsigned long> field(v+1,0);
  unsigned long end=static_cast<unsigned long>(sqrt(v));

/*
** Alle ungeraden Zahlen als potnezielle
** Primzahlen eintragen
*/
  for(i=3; i<=v; i+=2)
    field[i]=i;

  i=3;
  do {
/*
** Alle Vielfachen von i k�nnen keine Primzahlen sein
** => Aus der Liste austragen
*/
    for(unsigned long x=i*2; x<=v; x+=i)
      field[x]=0;
    i+=2;
/*
** N�chten Wert aus der Liste suchen oder abbrechen,
** falls Grenze erreicht
*/
    while((i<=end)&&(!field[i]))
      i+=2;
  } while(i<=end);
/*
** Alle ermittelten Primzahlen in den
** Ergebnisvektor �bertragen
*/
  result.push_back(2);
  for(i=3; i<=v; i+=2)
    if(field[i])
      result.push_back(field[i]);
}

void factorizeIntoPrimes(unsigned long v, vector<unsigned long> &result) {

  unsigned long ppos=0,n=v;

/*
** So lange laufen, wie n ungleich 1
*/
  while(n!=1) {
/*
** Ergibt die Division durch die aktuelle
** Primzahl einen Rest?
*/
    if(n%primeNumbers[ppos]) {
/*
** Wenn Rest: sollte v geteilt durch die aktuelle Primzahl
** kleiner als die aktuelle Primzahl sein, dann Abbruch
*/
      if((v/primeNumbers[ppos])<primeNumbers[ppos])
        break;
/*
** Andernfalls n�chste Primzahl nehmen
*/
      ppos++;
    }
    else {
/*
** Die Division hat keinen Rest ergeben?
** => Es wurde ein Faktor gefunden. Faktor in die L�sung
** �bernehmen und mit derselben Primzahl weitermachen
*/
      result.push_back(primeNumbers[ppos]);
      n/=primeNumbers[ppos];
    }
  }
/*
** Sollte der Rest ungleich 1 sein, dann ist er 
** der letzte Primfaktor und wird zur L�sung hinzugef�gt.
*/
  if(n!=1)
    result.push_back(n);
}







}; // namespace Codebook
