
#ifndef __CMATHMATRIX_H
#define __CMATHMATRIX_H

#include "CMatrix.h"
#include <exception>

namespace Codebook {

class invalid_dimension : public std::logic_error {
public:
  explicit invalid_dimension(const std::string& error)
  : logic_error(error)
  { }

};

template<typename Type, typename Allocator=std::allocator<Type> >
class CMathMatrix : public CMatrix<Type, Allocator> {
public:
  typedef CMatrix<Type, Allocator> base_type;
  typedef CMathMatrix<Type, Allocator> my_type;
  typedef base_type::size_type size_type;
  typedef base_type::reference reference;
  typedef base_type::const_reference const_reference;

private:

//***************************************************************
// Matrizen-Addition
//***************************************************************

  void add(CMathMatrix &res, const CMathMatrix &op) const {
/*
** Beide Matrizen haben nicht denselben Typ?
** => Ausnahme werfen
*/
    if((res.getRows()!=op.getRows())||(res.getColumns()!=op.getColumns()))
      throw invalid_dimension("CMathMatrix::add");
/*
** Die einzelnen Elemente addieren
*/
    for(size_type r=0; r<res.getRows(); ++r)
      for(size_type c=0; c<res.getColumns(); ++c)
        res(r,c)+=op(r,c);
  }

//***************************************************************
// Matrizen-Subtraktion
//***************************************************************

  void sub(CMathMatrix &res, const CMathMatrix &op) const {
/*
** Beide Matrizen haben nicht denselben Typ?
** => Ausnahme werfen
*/
    if((res.getRows()!=op.getRows())||(res.getColumns()!=op.getColumns()))
      throw invalid_dimension("CMathMatrix::sub");

/*
** Die einzelnen Elemente subtrahieren
*/
    for(size_type r=0; r<res.getRows(); ++r)
      for(size_type c=0; c<res.getColumns(); ++c)
        res(r,c)-=op(r,c);
  }

//***************************************************************
// Matrizen-Multiplikation
//***************************************************************

  void mul(CMathMatrix &res, const CMathMatrix &op1, const CMathMatrix &op2) const {
/*
** Es kann nur eine Matrix vom Typ (n,m) mit einer Matrix
** vom Typ (m,o) multipliziert werden
*/
    if(op1.getColumns()!=op2.getRows())
      throw invalid_dimension("CMathMatrix::mul");
/*
** Die Ergebnis-Matrix ist vom Typ (n,o)
*/
    res.resize(op1.getRows(), op2.getColumns());
/*
** Jedes Element e(i,k) von res ist skalares Produkt
** der i-ten Zeile von op1 mit der k-ten Spalte von op2
*/
    for(size_type r=0;r<res.getRows();++r)
      for(size_type c=0;c<res.getColumns();++c) {
        Type newElement=0;
/*
** Skalares Produkt bilden
*/
        for(size_type v=0;v<op1.getColumns();++v)
          newElement+=(op1(r,v)*op2(v,c));
        res(r,c)=newElement;
      }
  }

//*********

  void mul(CMathMatrix &res, const Type &op) const {
/*
** Jedes Element der Matrix wird mit op multipliziert
*/
    for(size_type r=0;r<res.getRows();++r)
      for(size_type c=0;c<res.getColumns();++c)
        res(r,c)*=op;
  }

public:

//***************************************************************
// Konstruktoren
//***************************************************************

  CMathMatrix()
  : base_type()
  {}

//*********

  CMathMatrix(const CMathMatrix &m)
  : base_type(m)
  {}

//*********

  CMathMatrix(size_type rows, size_type columns, const Type &obj=Type())
  : base_type(rows, columns, obj)
  {}

//*********

  CMathMatrix(const CMathMatrix &m, size_type rbegin, size_type cbegin, size_type rows, size_type columns)
  : base_type(m,rbegin,cbegin,rows,columns)
  {}

//*********

  template<typename IType>
  CMathMatrix(bool column, IType beg, IType end)
  : base_type(column, beg, end)
  {}


//***************************************************************
// Vergleichsoperatoren
//***************************************************************

  bool operator==(const CMathMatrix &m) {
    if((getColumns()!=m.getColumns())||(getRows()!=m.getRows()))
      return(false);
    for(size_type r=0; r<getRows(); ++r)
      for(size_type c=0; c<getColumns(); ++c)
        if((*this)(r,c)!=m(r,c))
          return(false);
    return(true);
  }

//*********

  bool operator!=(const CMathMatrix &m) {
    return(!((*this)==m));
  }

//***************************************************************
// Rechenoperatoren
//***************************************************************

  CMathMatrix operator+(const CMathMatrix &m) const {
    CMathMatrix res(*this);
    add(res,m);
    return(res);
  }

//*********

  CMathMatrix &operator+=(const CMathMatrix &m) {
    add(*this,m);
    return(*this);
  }

//*********

  CMathMatrix operator-(const CMathMatrix &m) const {
    CMathMatrix res(*this);
    sub(res,m);
    return(res);
  }

//*********

  CMathMatrix &operator-=(const CMathMatrix &m) {
    sub(*this,m);
    return(*this);
  }

//*********

  CMathMatrix operator*(const CMathMatrix &m) const {
    CMathMatrix res;
    mul(res,*this,m);
    return(res);
  }

//*********

  CMathMatrix &operator*=(const CMathMatrix &m) {
    CMathMatrix res;
    mul(res,*this,m);
    *this=res;
    return(*this);
  }

//*********

  CMathMatrix operator*(const Type &obj) const {
    CMathMatrix res(*this);
    mul(res,obj);
    return(res);
  }

//*********

  CMathMatrix &operator*=(const Type &obj) {
    mul(*this,obj);
    return(*this);
  }


//***************************************************************
// Prüf-Methoden
//***************************************************************

  bool isQuadratic() const {
    return(getRows()==getColumns());
  }

//*********

  bool isUnitMatrix() const {
/*
** Nur quadratische Matrizen können Einheitsmatrizen sein
*/
    if(!isQuadratic())
      return(false);
/*
** Die Elemente auf der Hauptdiagonalen müssen den Wert 1,
** alle anderen den Wert 0 haben
*/
    for(size_type r=0;r<getRows();++r)
      for(size_type c=0;c<getColumns();++c)
        if(((r==c)&&((*this)(r,c)!=1))||
           ((r!=c)&&((*this)(r,c)!=0)))
           return(false);
    return(true);
  }

//*********

  bool isUnitMatrix(const Type &eps) const {
/*
** Nur quadratische Matrizen können Einheitsmatrizen sein
*/
    if(!isQuadratic())
      return(false);
/*
** Die Elemente auf der Hauptdiagonalen müssen den Wert 1+/-eps,
** alle anderen den Wert 0+/-eps haben
*/
    for(size_type r=0;r<getRows();++r)
      for(size_type c=0;c<getColumns();++c)
        if(((r==c)&&(fabs((*this)(r,c)-1)>=eps))||
           ((r!=c)&&(fabs((*this)(r,c))>=eps)))
           return(false);
    return(true);
  }

//*********

  bool isRegular() const {
/*
** Nur eine quadratische Matrix kann regulär sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::isRegular");

    return(calcDeterminant()!=0);
  }

//*********

  bool isRegular(const Type &eps) const {
/*
** Nur eine quadratische Matrix kann regulär sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::isRegular");

    return(fabs(calcDeterminant())>eps);
  }

//*********

  bool isSingular() const {
/*
** Nur eine quadratische Matrix kann singulär sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::isSingular");

    return(calcDeterminant()==0);
  }

//*********

  bool isSingular(const Type &eps) const {
/*
** Nur eine quadratische Matrix kann singulär sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::isSingular");

    return(fabs(calcDeterminant())<=eps);
  }

//*********

  bool isSymmetric() const {
/*
** Nur eine quadratische Matrix kann symmetrisch sein
*/
    if(!isQuadratic())
      return(false);

    for(size_type r=0;r<getRows();r++)
      for(size_type c=r+1;c<getColumns();c++)
/*
** e(r,c)==e(c,r)?
*/
        if((*this)(r,c)!=(*this)(c,r))
          return(false);

    return(true);
  }

//*********

  bool isSymmetric(const Type &eps) const {
/*
** Nur eine quadratische Matrix kann symmetrisch sein
*/
    if(!isQuadratic())
      return(false);

    for(size_type r=0;r<getRows();r++)
      for(size_type c=r+1;c<getColumns();c++)
/*
** Unterscheiden sich e(r,c) und e(c,r) um mehr als eps?
*/
        if(fabs((*this)(r,c)-(*this)(c,r))>eps)
          return(false);

    return(true);
  }

//*********

  bool isSkewSymmetric() const {
/*
** Nur eine quadratische Matrix kann schiefsymmetrisch sein
*/
    if(!isQuadratic())
      return(false);

    for(size_type r=0;r<getRows();r++)
      for(size_type c=r;c<getColumns();c++)
/*
** Ist Hauptdiagonal-Element ungleich 0?
*/
        if(((c==r)&&((*this)(r,c)!=0))||((c!=r)&&
/*
** Unterscheiden sich e(r,c) und e(c,r) auch im Absolutwert?
*/
           (-(*this)(r,c)!=(*this)(c,r) )))
          return(false);

    return(true);
  }

//*********

  bool isSkewSymmetric(const Type &eps) const {
/*
** Nur eine quadratische Matrix kann schiefsymmetrisch sein
*/
    if(!isQuadratic())
      return(false);

    for(size_type r=0;r<getRows();r++)
      for(size_type c=r;c<getColumns();c++)
/*
** Ist Hauptdiagonal-Element größer eps?
*/
        if(((c==r)&&(fabs((*this)(r,c))>eps))||((c!=r)&&
/*
** Unterscheiden sich die Absolutwerte von e(r,c)
** und e(c,r) um mehr als eps?
*/
           (fabs((*this)(r,c)+(*this)(c,r))>eps )))
          return(false);

    return(true);
  }

//*********

  bool isUpperTriangularMatrix() const {
/*
** Nur eine quadratische Matrix kann eine obere Dreiecksmatrix sein
*/
    if(!isQuadratic())
      return(false);

    for(size_type c=0;c<getColumns();c++)
      for(size_type r=c+1;r<getRows();r++)
        if((*this)(r,c)!=0)
          return(false);

    return(true);
  }

//*********

  bool isUpperTriangularMatrix(const Type &eps) const {
/*
** Nur eine quadratische Matrix kann eine obere Dreiecksmatrix sein
*/
    if(!isQuadratic())
      return(false);

    for(size_type c=0;c<getColumns();c++)
      for(size_type r=c+1;r<getRows();r++)
        if(fabs((*this)(r,c))>eps)
          return(false);

    return(true);
  }


//***************************************************************
// Öffentliche Berechnungs-Methoden
//***************************************************************

  void convertToUnitMatrix() {
/*
** Nur eine quadratische Matrix kann in
** eine Einheitsmatrix umgewandelt werden
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::convertToE");
/*
** Die Elemente auf der Hauptdiagonalen werden auf 1,
** alle anderen auf 0 gesetzt
*/
    for(size_type r=0;r<getRows();++r)
      for(size_type c=0;c<getColumns();++c)
        if(r==c)
          (*this)(r,c)=1;
        else
          (*this)(r,c)=0;
  }

//*********

  void transpose() {
    if(isQuadratic()) {
/*
** Ist Matrix quadratisch?
** => die elemente e(r,c) und e(c,r) werden vertauscht
*/
      for(size_type r=0;r<getRows();r++)
        for(size_type c=r+1;c<getColumns();c++)
          swap((*this)(c,r),(*this)(r,c));
    }
    else {
/*
** Ist Matrix nicht quadratisch?
** => Eine neue Matrix für das Ergebnis wird angelegt
*/
      CMathMatrix res(getColumns(), getRows());
/*
** Das Element e(c,r) der transponierten Matrix bekommt
** den Wert von Element e(r,c) der originalen Matrix
*/
      for(size_type r=0;r<getRows();r++)
        for(size_type c=0;c<getColumns();c++)
          res(c,r)=(*this)(r,c);
      *this=res;
    }
  }

//*********

  CMathMatrix getTransposedMatrix() const {
    CMathMatrix res(getColumns(), getRows());
/*
** Das Element e(c,r) der transponierten Matrix bekommt
** den Wert von Element e(r,c) der originalen Matrix
*/
    for(size_type r=0;r<getRows();r++)
      for(size_type c=0;c<getColumns();c++)
        res(c,r)=(*this)(r,c);
    return(res);
  }

//*********

  CMathMatrix getSubdeterminant(size_type drow, size_type dcolumn) const {
/*
** Determinanten müssen quadratisch sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::getSubdeterminant");
/*
** Die Subdeterminante hat genau eine Zeile und Spalte
** weniger als das Original
*/
    CMathMatrix res(getRows()-1, getColumns()-1);
    for(int dr=0, sr=0; dr<res.getRows(); dr++, sr++) {
/*
** Zu streichende Zeile erreicht?
** => überspringen
*/
      if(sr==drow)
        sr++;
      for(int dc=0,sc=0;dc<res.getColumns(); dc++, sc++) {
/*
** Zu streichende Spalte erreicht?
** => überspringen
*/
        if(sc==dcolumn)
          sc++;
        res(dr,dc)=(*this)(sr,sc);
      }
    }
    return(res);
  }

//*********

  Type calcAdjoint(size_type drow, size_type dcolumn) const {
/*
** Determinanten müssen quadratisch sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::calcAdjoint");

/*
** Vorzeichen der Adjunkten berücksichtigen
*/
    if((drow+dcolumn)&1)
      return(-getSubdeterminant(drow, dcolumn).calcDeterminant());
    else
      return(getSubdeterminant(drow, dcolumn).calcDeterminant());
  }

//*********

  Type calcDeterminant(void) const {
/*
** Determinanten müssen quadratisch sein
*/
    if(!isQuadratic())
      throw invalid_dimension("CMathMatrix::calcDeterminant");
/*
** Determinante zweireihig?
** => Wert = e(0,0)*e(1,1) - e(1,0)*e(0,1)
*/
    if(getRows()==2)
      return(((*this)(0,0) * (*this)(1,1))-
             ((*this)(0,1) * (*this)(1,0)));
/*
** Determinante mehr als zweireihig?
** => Werte der ersten Spalte mit ihrer Adjunkten
** multiplizieren und aufaddieren
*/
    else {
      Type res=0;
      for(size_type r=0;r<getRows(); r++)
        res+=(*this)(r,0)*calcAdjoint(r,0);

      return(res);
    }
  }

//*********

  CMathMatrix getInverseMatrix() const {
/*
** Inverse Matrizen nur bei regulären quadratischen Matrizen bestimmbar
*/
    if((!isQuadratic())||(!isRegular()))
      throw invalid_dimension("CMathMatrix::getInverseMatrix");
/*
** Matrix zur Aufnahme der Berechnungen anlegen
*/
    CMathMatrix res(getRows(), getColumns());
/*
** Jedes Element wird durch die entsprechende Adjunkte ersetzt.
** (Vertauschte Indizes!)
*/
    for(size_type r=0; r<getRows(); r++)
      for(size_type c=0; c<getColumns(); c++)
        res(r,c)=calcAdjoint(c,r);
/*
** Die Matrix wird mit 1/det multipliziert
*/
    res*=(1/calcDeterminant());
    return (res);
  }

//*********

  void swapRows(size_type r1, size_type r2) {
    swap((*this)[r1], (*this)[r2]);
  }

//*********


  static std::pair<CMathMatrix,CMathMatrix> gaussElimination(const std::pair<CMathMatrix,CMathMatrix> &p) {
/*
** Koeffizientenmatrix muss quadratisch sein und die gleiche
** Zeilenanzahl haben wie der Ergebnisvektor
*/
    if(!p.first.isQuadratic()||(p.first.getRows()!=p.second.getRows()))
      throw invalid_dimension("CMathMatrix::gaussElimination");

    CMathMatrix k=p.first, e=p.second;

/*
** Das betragsgroesste Element der zu elimierenden Spalte
** suchen.
*/
    for(size_type step=0; step<k.getRows()-1; step++) {
      size_type posofmaxval=step;
      for(size_type r=step+1; r<k.getRows(); r++)
        if(fabs(k(posofmaxval,step))<fabs(k(r,step)))
          posofmaxval=r;
/*
** Die aktuelle Zeile und die Zeile mit dem
** betragsgroessten Element vertauschen.
*/
      if(posofmaxval!=step) {
        k.swapRows(step,posofmaxval);
        e.swapRows(step,posofmaxval);
      }
/*
** Die aktuelle Spalte wird eliminiert
*/
      for(size_type transgl=step+1;transgl<k.getRows();transgl++) {
        Type factor=-k(transgl,step)/k(step,step);

        e(transgl,0)=e(transgl,0)+(e(step,0)*factor);
        for(size_type c=step; c<k.getRows(); c++) {
          k(transgl,c)=k(transgl,c)+(k(step,c)*factor);
        }
      }
    }
/*
** Umgewandelte Koeffizientenmatrix und angepassten
** Ergebnisvektor zurückliefern
*/
    return(make_pair(k,e));
  }


  static CMathMatrix calcSolutionVector(const std::pair<CMathMatrix,CMathMatrix> &p, const Type &eps=0) {
/*
** Koeffizientenmatrix muss eine obere Dreiecksmatrix sein
** Zeilenanzahl von Koeffizientenmatrix und Ergebnisvektor
** müssen übereinstimmen
*/
    if((!p.first.isUpperTriangularMatrix(eps))||(p.first.getRows()!=p.second.getRows()))
      throw invalid_dimension("CMathMatrix::calcSolutionVector");

    CMathMatrix k=p.first, e=p.second;
    size_type n=k.getRows();

    cout << "n:" << n << endl;
    size_type r=n-1;
/*
** Das Gleichungssystem wird zeilenweise aufgelöst
*/
    while(true) {
      for(size_type c=n-1;c>r;c--) {
        k(r,c)=k(r,c)*e(c,0);
        e(r,0)=e(r,0)-k(r,c);
        k(r,c)=0;
      }
      e(r,0)=e(r,0)/k(r,r);
      k(r,r)=1;

      if(r==0)
        break;
      r--;
    }
/*
** Ergebnisvektor zurückliefern
*/
    return(e);
  }


};








}; // namespace Codebook

#endif
