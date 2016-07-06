
#ifndef __CMATHFUNCTIONS_H
#define __CMATHFUNCTIONS_H

#include <cmath>
#include <vector>

namespace Codebook {

template<typename Type>
bool isEven(Type v) {
  return(!(v%2));
}


template<typename Type>
bool isPrime(Type v) {
/*
** Sonderfall: 1 ist keine Primzahl
*/
  if(v==1)
    return(false);
/*
** 2 ist eine Primzahl
*/
  if(v==2)
    return(true);
/*
** Durch 2 teilbar?
** => Keine Primzahl
*/
  if(!(v%2))
    return(false);
/*
** von 3 ab in Zweierschritten bis Wurzel(v) auf teilbar prüfen
*/
  Type end=static_cast<Type>(sqrt(v));
  for(Type i=3;i<=end; i+=2)
    if(!(v%i))
      return(false);
  return(true);
}

template<typename Type>
Type facul(Type x) {
  if(x<0)
    throw std::out_of_range("facul()");
  Type f=1;
  while(x>1)
    f*=x--;
  return(f);
}


template<typename Type>
Type fibonacci(Type x) {
  if(x<1)
    throw std::out_of_range("fibonacci()");
  if((x<3))
    return(1);
  return( fibonacci(x-1) + fibonacci(x-2) );
}

template<typename Type>
Type ggT(Type a, Type b) {
  Type c=a%b;
  if(!c)
    return(b);
  else
    return(ggT(b,c)); 
}

template<typename Type>
Type kgV(Type a, Type b) {
  return(a*b/ggT(a,b));
}

void getPrimeNumbers(unsigned long v, std::vector<unsigned long> &result);
unsigned long ggT(unsigned long a, unsigned long b);
unsigned long kgV(unsigned long a, unsigned long b);
unsigned long facul(unsigned long x);
unsigned long fibonacci(unsigned long x);
void factorizeIntoPrimes(unsigned long v, std::vector<unsigned long> &result);


}; // namespace Codebook

#endif
