#ifndef __CSHIFTER_H
#define __CSHIFTER_H

#include <vector>

namespace Codebook {

template<typename Type>
class CShifter {
private:
  std::vector<Type> m_elements;
  long m_pos;

  CShifter() {}

public:
  CShifter(const Type &a, const Type &b)
  : m_pos(0) {
    m_elements.push_back(a);
    m_elements.push_back(b);
  }

//*********

  CShifter(const Type &a, const Type &b, const Type &c)
  : m_pos(0) {
    m_elements.push_back(a);
    m_elements.push_back(b);
    m_elements.push_back(c);
  }

//*********

  CShifter(const CShifter &s)
  : m_pos(s.m_pos), m_elements(s.m_elements)
  {}

//*********
/*
  template<typename CType>
  CShifter(const CType &c)
  : m_pos(0) {
    CType::const_iterator iter=c.begin();
    while(iter!=c.end())
      m_elements.push_back(*iter++);
//
// Nicht mehr als ein Objekt enthalten*
// => Ausnahme werfen
//
    if(m_elements.size()<2)
      throw std::invalid_argument("CShifter::CShifter()");
  }
*/
//*********

  void toBegin() {
    m_pos=0;
  }

//*********

  void toEnd() {
    m_pos=m_elements.size()-1;
  }

//*********

  CShifter &operator+=(long offset) {
    m_pos+=offset;
    if(m_pos>=m_elements.size())
      m_pos%=m_elements.size();
    return(*this);
  }

//*********

  CShifter &operator-=(long offset) {
    m_pos-=offset;
    if(m_pos<0)
      m_pos=m_elements.size()-1+((m_pos+1)%static_cast<long>(m_elements.size()));
    return(*this);
  }

//*********

  CShifter operator++(int) {
    CShifter tmp=*this;
    ++m_pos;
    if(m_pos>=m_elements.size())
      m_pos=0;
    return(tmp);
  }

//*********

  CShifter &operator++() {
    ++m_pos;
    if(m_pos>=m_elements.size())
      m_pos=0;
    return(*this);
  }

//*********

  CShifter operator--(int) {
    CShifter tmp=*this;
    if(m_pos==0)
      m_pos=m_elements.size()-1;
    else
      --m_pos;
    return(tmp);
  }

//*********

  CShifter &operator--() {
    if(m_pos==0)
      m_pos=m_elements.size()-1;
    else
      --m_pos;
    return(*this);
  }

//*********

  Type &operator*() {
    return(m_elements[m_pos]);
  }

//*********

  Type *operator->() {
    return(&m_elements[m_pos]);
  }

//*********

  operator Type() {
    return(m_elements[m_pos]);
  }

//*********

  class iterator;
  friend class iterator;
  class iterator : public std::iterator<std::bidirectional_iterator_tag,Type> {
  private:
    long m_pos;
    CShifter *m_shifter;
  public:
    iterator()
      :m_shifter(0), m_pos(0)
    {}

//*********

    iterator(CShifter &shifter, long pos)
      : m_shifter(&shifter), m_pos(pos)
    {}

//*********

    iterator &operator++() {
      ++m_pos;
      if(m_pos>=m_shifter->m_elements.size())
        m_pos=0;
      return(*this);
    }

//*********

    iterator &operator--() {
      if(m_pos==0)
        m_pos=m_shifter->m_elements.size()-1;
      else
        --m_pos;
      return(*this);
    }

//*********

    iterator operator++(int) {
      iterator tmp=*this;
      ++m_pos;
      if(m_pos>=m_shifter->m_elements.size())
        m_pos=0;
      return(tmp);
    }

//*********

    iterator operator--(int) {
      iterator tmp=*this;
      if(m_pos==0)
        m_pos=m_shifter->m_elements.size()-1;
      else
        --m_pos;
      return(tmp);
    }

//*********

    Type &operator*() {
      return(m_shifter->m_elements[m_pos]);
    }

//*********

    Type *operator->() {
      return(&m_shifter->m_elements[m_pos]);
    }

//*********

    bool operator==(iterator &i) {
      return((m_shifter==i.m_shifter)&&(m_pos==i.m_pos));
    }

//*********

    bool operator!=(iterator &i) {
      return((m_shifter!=i.m_shifter)||(m_pos!=i.m_pos));
    }

  };

//*********

  iterator begin() {
    return(iterator(*this,0));
  }

};


}; // namespace Codebook

#endif
