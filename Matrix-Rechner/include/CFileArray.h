
#ifndef __CFILEARRAY_H
#define __CFILEARRAY_H

#include "CMemory.h"
#include "CFile.h"
#include <iterator>

namespace Codebook {

class CFileArray
{
  private:
    CMemory<char> m_mem;
    CFile m_file;
    std::streampos m_pufferSize;
    bool m_changed;
    std::streampos m_size;
    std::streampos m_blockPos;
    std::streampos m_blockSize;
    char &CharRef(std::streampos);
    char &CharRef(std::streampos) const;
    void readBlock(void);
    void writeBlock(void);


  public:
    static std::streampos npos;

    class CProxy;
    friend class CFileArray::CProxy;

    class CProxy {
      friend class CFileArray;
      private:
        std::streampos m_index;
        CFileArray &m_fileArray;
        CProxy(CFileArray &d, std::streampos i)
          : m_fileArray(d), m_index(i)
        {}
      public:
        operator char() const {
          return(m_fileArray.CharRef(m_index));
        }

        CProxy &operator=(const CProxy &p){
          m_fileArray.CharRef(m_index)=p.m_fileArray.CharRef(p.m_index);
          m_fileArray.m_changed=true;
          return(*this);
        }
        CProxy &operator=(char c){
          m_fileArray.CharRef(m_index)=c;
          m_fileArray.m_changed=true;
          return(*this);
        }
        friend std::ostream &operator<<(std::ostream &ostr, const CProxy &p) {
          ostr << (static_cast<char>(p));
          return(ostr);
        }
      };


    class iterator;
    friend class CFileArray::iterator;
    class iterator : public std::iterator<std::random_access_iterator_tag, char> {
    private:
      friend class CFileArray;

      std::streampos m_index;
      CFileArray &m_fileArray;
    public:
      iterator(CFileArray &d, std::streampos i=0)
        : m_fileArray(d), m_index(i)
      {}

      iterator(iterator &i)
        : m_fileArray(i.m_fileArray), m_index(i.m_index)
      {}

      iterator &operator=(iterator &i) {
        m_fileArray=i.m_fileArray;
        m_index=i.m_index;
        return(*this);
      }

      iterator &operator++() {
        if(m_index<m_fileArray.size())
          m_index+=1;
        return(*this);
      }
      iterator &operator--() {
        if(m_index>0)
          m_index-=1;
        return(*this);
      }

      iterator operator++(int) {
        iterator tmp=*this;
        if(m_index<m_fileArray.size())
          m_index+=1;
        return(tmp);
      }
      iterator operator--(int) {
        iterator tmp=*this;
        if(m_index>0)
          m_index-=1;
        return(tmp);
      }

      iterator operator+(std::streampos o) {
        std::streampos tmp=m_index+o;
        if(tmp>m_fileArray.m_size)
          tmp=m_fileArray.m_size;
        return(iterator(m_fileArray,tmp));
      }
      iterator operator-(std::streampos o) {
        std::streampos tmp;
        if(m_index<o)
          tmp=0;
        else
          tmp=m_index-o;
        return(iterator(m_fileArray,tmp));
      }

      iterator operator+(iterator &i) {
        if(&m_fileArray!=&i.m_fileArray)
          throw CFileArray::file_error("CFileArray::iterator::operator+()");
        std::streampos tmp=m_index+i.m_index;
        if(tmp>m_fileArray.m_size)
          tmp=m_fileArray.m_size;
        return(iterator(m_fileArray,tmp));
      }
      iterator operator-(iterator &i) {
        if(&m_fileArray!=&i.m_fileArray)
          throw CFileArray::file_error("CFileArray::iterator::operator-()");
        std::streampos tmp;
        if(m_index<i.m_index)
          tmp=0;
        else
          tmp=m_index-i.m_index;
        return(iterator(m_fileArray,tmp));
      }

      iterator &operator+=(std::streampos o) {
        m_index+=o;
        if(m_index>m_fileArray.m_size)
          m_index=m_fileArray.m_size;
        return(*this);
      }
      iterator &operator-=(std::streampos o) {
        if(m_index<o)
          m_index=0;
        else
          m_index-=o;
        return(*this);
      }

      iterator &operator+=(iterator &i) {
        if(&m_fileArray!=&i.m_fileArray)
          throw CFileArray::file_error("CFileArray::iterator::operator+=()");
        m_index+=i.m_index;
        if(m_index>m_fileArray.m_size)
          m_index=m_fileArray.m_size;
        return(*this);
      }
      iterator &operator-=(iterator &i) {
        if(&m_fileArray!=&i.m_fileArray)
          throw CFileArray::file_error("CFileArray::iterator::operator-=()");
        if(m_index<i.m_index)
          m_index=0;
        else
          m_index-=i.m_index;
        return(*this);
      }

      CProxy operator*() {
        return(m_fileArray[m_index]);
      }

      CProxy operator->() {
        return(m_fileArray[m_index]);
      }

      CProxy operator[](std::streampos p) {
        return(m_fileArray[p]);
      }

      bool operator!=(iterator &i) {
        return((m_index!=i.m_index)&&(&m_fileArray==&i.m_fileArray));
      }
      bool operator==(iterator &i) {
        return((m_index==i.m_index)&&(&m_fileArray==&i.m_fileArray));
      }
      bool operator<(iterator &i) {
        return((m_index<i.m_index)&&(&m_fileArray==&i.m_fileArray));
      }
      bool operator>(iterator &i) {
        return((m_index>i.m_index)&&(&m_fileArray==&i.m_fileArray));
      }
      bool operator<=(iterator &i) {
        return((m_index<=i.m_index)&&(&m_fileArray==&i.m_fileArray));
      }
      bool operator>=(iterator &i) {
        return((m_index>=i.m_index)&&(&m_fileArray==&i.m_fileArray));
      }

    };


/*
    class iterator : public std::iterator<std::bidirectional_iterator_tag, char> {
      private
    };
*/
    class file_error {
			public:
        const std::string name;
        file_error(const std::string &n) :name(n) {}
		};

    CFileArray(const std::string &name, bool create=true, unsigned long puffsize=50000);
    ~CFileArray();
    CProxy operator[](unsigned long);
    std::streampos size(void) const{
      return(m_size);
    }
    iterator begin() {
      return(iterator(*this,0));
    }

    iterator end() {
      return(iterator(*this,m_size));
    }


};

}; // namespace Codebook

#endif
