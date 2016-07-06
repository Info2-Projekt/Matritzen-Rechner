
#ifndef __CMATRIX_H
#define __CMATRIX_H

#include "CSaveVector.h"
#include "CBinaryFStream.h"

namespace Codebook {



template<typename Type, typename Allocator=std::allocator<Type> >
class CMatrix;

//***************************************************************
// CMatrixVector
//***************************************************************

template<typename Type, typename Allocator=std::allocator<Type> >
class CMatrixVector : private CSaveVector<Type, Allocator> {
  friend class CMatrix;
public:
  typedef CSaveVector<Type, Allocator> base_type;
  typedef CMatrixVector<Type, Allocator> my_type;
  typedef base_type::size_type size_type;
  typedef base_type::reference reference;
  typedef base_type::const_reference const_reference;

//*********

  explicit CMatrixVector(const Allocator &a=Allocator())
    : base_type(a)
  {}

//*********

  CMatrixVector(size_type n, const Type &obj=Type(), const Allocator &a=Allocator())
    : base_type(n,obj,a)
  {}

//*********

  reference operator[](size_type idx) {
    return(base_type::operator[](idx));
  }

//*********

  const_reference operator[](size_type idx) const {
    return(base_type::operator[](idx));
  }
};


template<typename Type, typename Allocator=std::allocator<Type> >
class CMatrix {
public:
  typedef CMatrix<Type, Allocator> my_type;
  typedef std::size_t size_type;
  typedef Type& reference;
  typedef const Type& const_reference;



//***************************************************************
// row_iterator
//***************************************************************

  class row_iterator;
  friend class row_iterator;
  class row_iterator : public std::iterator<std::bidirectional_iterator_tag,Type> {
  private:
    size_type m_r;
    size_type m_c;
    CMatrix *m_matrix;
  public:
    row_iterator()
      :m_matrix(0), m_r(0), m_c(0)
    {}

//*********

    row_iterator(CMatrix &matrix, size_type r, size_type c)
      : m_matrix(&matrix), m_r(r), m_c(c)
    {}

//*********

    row_iterator &operator++() {
/*
** Ende-Position erreicht?
** Ende-Position beibehalten
*/
      if(m_r==m_matrix->m_rows)
        return(*this);
/*
** Nächstes Element der Zeile
*/
      ++m_c;
/*
** Zeile zu Ende?
** => Auf erstes Element der nächsten Zeile setzen
*/
      if(m_c==m_matrix->m_columns) {
        m_c=0;
        ++m_r;
      }
      return(*this);
    }

//*********

    row_iterator &operator--() {
/*
** Anfangs-Position erreicht?
** Anfangs-Position beibehalten
*/
      if((m_c==0)&&(m_r==0))
        return(*this);
/*
** Ist aktuelles Element erstes Element der Zeile?
** => Auf letzten Element der vorhergehenden Zeile setzen
*/
      if(m_c==0) {
        m_c=m_matrix->m_columns-1;
        --m_r;
      }
/*
** Andernfalls auf vorhergehendes Element der Zeile setzen
*/
      else
        --m_c;
      return(*this);
    }

//*********

    row_iterator operator++(int) {
      row_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    row_iterator operator--(int) {
      row_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    Type &operator*() {
      return((*m_matrix)[m_r][m_c]);
    }

//*********

    Type *operator->() {
      return(&(*m_matrix)[m_r][m_c]);
    }

//*********

    bool operator==(row_iterator &i) const {
      return((m_matrix==i.m_matrix)&&(m_c==i.m_c)&&(m_r==i.m_r));
    }

//*********

    bool operator!=(row_iterator &i) const {
      return((m_matrix!=i.m_matrix)||(m_c!=i.m_c)||(m_r!=i.m_r));
    }

  };

//***************************************************************
// column_iterator
//***************************************************************

  class column_iterator;
  friend class column_iterator;
  class column_iterator : public std::iterator<std::bidirectional_iterator_tag,Type> {
  private:
    size_type m_r;
    size_type m_c;
    CMatrix *m_matrix;
  public:
    column_iterator()
      :m_matrix(0), m_r(0), m_c(0)
    {}

//*********

    column_iterator(CMatrix &matrix, size_type r, size_type c)
      : m_matrix(&matrix), m_r(r), m_c(c)
    {}

//*********

    column_iterator &operator++() {
/*
** Ende-Position erreicht?
** Ende-Position beibehalten
*/
      if(m_c==m_matrix->m_columns)
        return(*this);
/*
** Nächstes Element der Spalte
*/
      ++m_r;
/*
** Spalte zu Ende?
** => Auf erstes Element der nächsten Spalte setzen
*/
      if(m_r==m_matrix->m_rows) {
        m_r=0;
        ++m_c;
      }
      return(*this);
    }

//*********

    column_iterator &operator--() {
/*
** Anfangs-Position erreicht?
** Anfangs-Position beibehalten
*/
      if((m_c==0)&&(m_r==0))
        return(*this);
/*
** Ist aktuelles Element erstes Element der Spalte?
** => Auf letzten Element der vorhergehenden Spalte setzen
*/
      if(m_r==0) {
        m_r=m_matrix->m_rows-1;
        --m_c;
      }
/*
** Andernfalls auf vorhergehendes Element der Spalte setzen
*/
      else
        --m_r;
      return(*this);
    }

//*********

    column_iterator operator++(int) {
      column_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    column_iterator operator--(int) {
      column_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

Type &operator*() {
  return((*m_matrix)[m_r][m_c]);
}

//*********

Type *operator->() {
  return(&(*m_matrix)[m_r][m_c]);
}

//*********

    bool operator==(column_iterator &i) const {
      return((m_matrix==i.m_matrix)&&(m_c==i.m_c)&&(m_r==i.m_r));
    }

//*********

    bool operator!=(column_iterator &i) const {
      return((m_matrix!=i.m_matrix)||(m_c!=i.m_c)||(m_r!=i.m_r));
    }

  };

//***************************************************************
// const_row_iterator
//***************************************************************

  class const_row_iterator;
  friend class const_row_iterator;
  class const_row_iterator : public std::iterator<std::bidirectional_iterator_tag,Type> {
  private:
    size_type m_r;
    size_type m_c;
    const CMatrix *m_matrix;
  public:
    const_row_iterator()
      :m_matrix(0), m_r(0), m_c(0)
    {}

//*********

    const_row_iterator(const CMatrix &matrix, size_type r, size_type c)
      : m_matrix(&matrix), m_r(r), m_c(c)
    {}

//*********

    const_row_iterator &operator++() {
      if(m_r==m_matrix->m_rows)
        return(*this);
      ++m_c;
      if(m_c==m_matrix->m_columns) {
        m_c=0;
        ++m_r;
      }
      return(*this);
    }

//*********

    const_row_iterator &operator--() {
      if((m_c==0)&&(m_r==0))
        return(*this);
      if(m_c==0) {
        m_c=m_matrix->m_columns-1;
        --m_r;
      }
      else
        --m_c;
      return(*this);
    }

//*********

    const_row_iterator operator++(int) {
      const_row_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    const_row_iterator operator--(int) {
      const_row_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    const Type &operator*() {
      return((*m_matrix)[m_r][m_c]);
    }

//*********

    const Type *operator->() {
      return(&(*m_matrix)[m_r][m_c]);
    }

//*********

    bool operator==(const_row_iterator &i) const {
      return((m_matrix==i.m_matrix)&&(m_c==i.m_c)&&(m_r==i.m_r));
    }

//*********

    bool operator!=(const_row_iterator &i) const {
      return((m_matrix!=i.m_matrix)||(m_c!=i.m_c)||(m_r!=i.m_r));
    }

  };


//***************************************************************
// const_column_iterator
//***************************************************************

  class const_column_iterator;
  friend class const_column_iterator;
  class const_column_iterator : public std::iterator<std::bidirectional_iterator_tag,Type> {
  private:
    size_type m_r;
    size_type m_c;
    const CMatrix *m_matrix;
  public:
    const_column_iterator()
      :m_matrix(0), m_r(0), m_c(0)
    {}

//*********

    const_column_iterator(const CMatrix &matrix, size_type r, size_type c)
      : m_matrix(&matrix), m_r(r), m_c(c)
    {}

//*********

    const_column_iterator &operator++() {
      if(m_c==m_matrix->m_columns)
        return(*this);
      ++m_r;
      if(m_r==m_matrix->m_rows) {
        m_r=0;
        ++m_c;
      }
      return(*this);
    }

//*********

    const_column_iterator &operator--() {
      if((m_c==0)&&(m_r==0))
        return(*this);
      if(m_r==0) {
        m_r=m_matrix->m_rows-1;
        --m_c;
      }
      else
        --m_r;
      return(*this);
    }

//*********

    const_column_iterator operator++(int) {
      const_column_iterator tmp=*this;
      ++(*this);
      return(tmp);
    }

//*********

    const_column_iterator operator--(int) {
      const_column_iterator tmp=*this;
      --(*this);
      return(tmp);
    }

//*********

    const Type &operator*() {
      return((*m_matrix)[m_r][m_c]);
    }

//*********

    const Type *operator->() {
      return(&(*m_matrix)[m_r][m_c]);
    }

//*********

    bool operator==(const_column_iterator &i) const {
      return((m_matrix==i.m_matrix)&&(m_c==i.m_c)&&(m_r==i.m_r));
    }

//*********

    bool operator!=(const_column_iterator &i) const {
      return((m_matrix!=i.m_matrix)||(m_c!=i.m_c)||(m_r!=i.m_r));
    }

  };


//***************************************************************
// CMatrix-Elemente
//***************************************************************

private:
  CMatrixVector<CMatrixVector<Type, Allocator>, Allocator> m_elements;
  size_type m_rows;
  size_type m_columns;


public:
  size_type getRows() const {
    return(m_rows);
  }

//*********

  size_type getColumns() const {
    return(m_columns);
  }

//*********

  size_type size() const {
    return(m_rows*m_columns);
  }

/*
** begin & end für row_iterator
*/

  row_iterator row_begin() {
    return(row_iterator(*this,0,0));
  }
 
//*********

  row_iterator row_end() {
    return(row_iterator(*this,m_rows,0));
  }

//*********

  row_iterator row_begin(size_type r) {
    return(row_iterator(*this,r,0));
  }
 
//*********

  row_iterator row_end(size_type r) {
    return(row_iterator(*this,r+1,0));
  }

/*
** begin & end für const_row_iterator
*/

  const_row_iterator row_begin() const {
    return(const_row_iterator(*this,0,0));
  }
 
//*********

  const_row_iterator row_end() const {
    return(const_row_iterator(*this,m_rows,0));
  }

//*********

  const_row_iterator row_begin(size_type r) const {
    return(const_row_iterator(*this,r,0));
  }
 
//*********

  const_row_iterator row_end(size_type r) const {
    return(const_row_iterator(*this,r+1,0));
  }

/*
** begin & end für column_iterator
*/

  column_iterator column_begin() {
    return(column_iterator(*this,0,0));
  }
 
//*********

  column_iterator column_end() {
    return(column_iterator(*this,0,m_columns));
  }

//*********

  column_iterator column_begin(size_type c) {
    return(column_iterator(*this,0,c));
  }
 
//*********

  column_iterator column_end(size_type c) {
    return(column_iterator(*this,0,c+1));
  }

/*
** begin & end für const_column_iterator
*/

  const_column_iterator column_begin() const {
    return(const_column_iterator(*this,0,0));
  }
 
//*********

  const_column_iterator column_end() const {
    return(const_column_iterator(*this,0,m_columns));
  }

//*********

  const_column_iterator column_begin(size_type c) const {
    return(const_column_iterator(*this,0,c));
  }
 
//*********

  const_column_iterator column_end(size_type c) const {
    return(const_column_iterator(*this,0,c+1));
  }

//*********

  CMatrix()
  : m_rows(0), m_columns(0)
  {}

//*********

  CMatrix(const CMatrix &m)
  : m_elements(m.m_elements), m_rows(m.m_rows), m_columns(m.m_columns)
  { }

//*********

  CMatrix(size_type rows, size_type columns, const Type &obj=Type()) {
    resize(rows,columns,obj);
  }

//*********

  CMatrix(const CMatrix &m, size_type rbegin, size_type cbegin, size_type rows, size_type columns) {
    resize(rows,columns);
    for(size_type r=0; r<rows; ++r)
      for(size_type c=0; c<columns; ++c)
        m_elements[r][c]=m.m_elements[r+rbegin][c+cbegin];
  }

//*********

  template<typename IType>
  CMatrix(bool column, IType beg, IType end) {
    if(column) {
      resize(distance(beg,end),1);
      copy(beg,end,column_begin());
    }
    else {
      resize(1, distance(beg,end));
      copy(beg,end,row_begin());
    }
  }

//*********

  CMatrix &operator=(const CMatrix &m) {
    if(this!=&m) {
      m_elements=m.m_elements;
      m_rows=m.m_rows;
      m_columns=m.m_columns;
    }
    return(*this);
  }

//*********

  CMatrixVector<Type, Allocator> &operator[](size_type idx) {
    return(m_elements[idx]);
  }

//*********

  const CMatrixVector<Type, Allocator> &operator[](size_type idx) const {
    return(m_elements[idx]);
  }

//*********

  Type &operator()(size_type r, size_type c) {
    return(m_elements.at(r).at(c));
  }

//*********

  const Type &operator()(size_type r, size_type c) const {
    return(m_elements.at(r).at(c));
  }

//*********

  std::string output(int width) const{
    ostringstream os;
    for(size_type y=0; y<m_rows; ++y) {
      for(size_type x=0; x<m_columns; ++x)
        os << std::setw(width) << (*this)[y][x] << " ";
      os << "\n";
    }
    return(os.str());
  }

//*********

  void resize(size_type rows, size_type columns, const Type &obj=Type()) {
/*
** Größe des Zeilenvektors anpassen
*/
    m_elements.resize(rows, CMatrixVector<Type, Allocator>(columns,obj) );
/*
** Größe der Spaltenvektoren anpassen
*/
    for(size_type r=0; r<rows; ++r)
      m_elements[r].resize(columns,obj);
    m_columns=columns;
    m_rows=rows;
  }

/*
** Stream-Operatoren
*/

  friend CBinaryOStream &operator<<(CBinaryOStream &os, const CMatrix &m){
    os << m.m_rows;
    os << m.m_columns;
    const_row_iterator iter=m.row_begin();
    while(iter!=m.row_end())
      os << *(iter++);
    return(os);
  }

  friend CBinaryIStream &operator>>(CBinaryIStream &is, CMatrix &m){
    is >> m.m_rows;
    is >> m.m_columns;
    m.resize(m.m_rows,m.m_columns);
    row_iterator iter=m.row_begin();
    while(iter!=m.row_end())
      is >> *(iter++);
    return(is);
  }

};
}; // namespace Codebook

#endif
