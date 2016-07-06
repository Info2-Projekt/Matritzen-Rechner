
#ifndef __CBINARYSTREAM_H
#define __CBINARYSTREAM_H


#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>


namespace Codebook {

class CBinaryOStream {

public:

  virtual void write(const void *mem, std::streamsize size)=0;
  virtual bool is_open()=0;
  virtual void close()=0;
  virtual bool fail()=0;

  CBinaryOStream &operator<<(char v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(short v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(int v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(long v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(unsigned short v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(unsigned int v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(unsigned long v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(float v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(double v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(long double v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(bool v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(const void *v) {
    write(&v, sizeof(v));
    return(*this);
  }

  CBinaryOStream &operator<<(const char *v) {
    size_t len=strlen(v);
    *this<< len;
    write(v, static_cast<std::streamsize>(len));
    return(*this);
  }

  CBinaryOStream &operator<<(const std::string &v) {
    size_t len=v.size();
    *this<<len;
    write(v.c_str(), static_cast<std::streamsize>(len));
    return(*this);
  }

  template<typename TypeA, typename TypeB>
  CBinaryOStream &operator<<(const std::pair<TypeA,TypeB> &v) {
    *this << v.first << v.second;
    return(*this);
  }

  template<typename Type>
  CBinaryOStream &operator<<(const std::vector<Type> &v) {
    *this<<v.size();
    for(std::vector<Type>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
      *this<<*iter;
    return(*this);
  }

  template<typename Type>
  CBinaryOStream &operator<<(const std::deque<Type> &v) {
    *this<<v.size();
    for(std::deque<Type>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
      *this<<*iter;
    return(*this);
  }

  template<typename Type>
  CBinaryOStream &operator<<(const std::list<Type> &v) {
    *this<<v.size();
    for(std::list<Type>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
      *this<<*iter;
    return(*this);
  }

  template<typename Type>
  CBinaryOStream &operator<<(const std::set<Type> &v) {
    *this<<v.size();
    for(set<Type>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
      *this<<*iter;
    return(*this);
  }

  template<typename Type>
  CBinaryOStream &operator<<(const std::multiset<Type> &v) {
    *this<<v.size();
    for(multiset<Type>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
      *this<<*iter;
    return(*this);
  }

  template<typename KType, typename DType>
  CBinaryOStream &operator<<(const std::map<KType, DType> &v) {
    *this<<v.size();
    for(map<KType, DType>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
      *this<<*iter;
    return(*this);
  }



};

class CBinaryIStream {
public:
  virtual void read(void *mem, std::streamsize size)=0;
  virtual bool is_open()=0;
  virtual void close()=0;
  virtual bool fail()=0;
  virtual bool eof()=0;

  CBinaryIStream &operator>>(char &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(short &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(int &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(long &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(unsigned short &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(unsigned int &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(unsigned long &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(float &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(double &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(long double &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(bool &v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(const void *v) {
    read(&v, sizeof(v));
    return(*this);
  }

  CBinaryIStream &operator>>(char *v) {
    size_t len;
    *this>>len;
    read(v, static_cast<std::streamsize>(len));
    v[len]=0;
    return(*this);
  }

  CBinaryIStream &operator>>(std::string &v) {
    size_t len;
    *this>>len;
    char *buff=new char[len+1];
    read(buff, static_cast<std::streamsize>(len));
    buff[len]=0;
    v=buff;
    delete(buff);
    return(*this);
  }

  template<typename TypeA, typename TypeB>
  CBinaryIStream &operator>>(std::pair<TypeA,TypeB> &v) {
    *this >> v.first >> v.second;
    return(*this);
  }

  template<typename Type>
  CBinaryIStream &operator>>(std::vector<Type> &v) {
    v.clear();
    size_t size;
    *this>>size;
    Type input;
    for(size_t i=0; i<size; i++) {
      *this>>input;
      v.push_back(input);
    }
    return(*this);
  }

  template<typename Type>
  CBinaryIStream &operator>>(std::deque<Type> &v) {
    v.clear();
    size_t size;
    *this>>size;
    Type input;
    for(size_t i=0; i<size; i++) {
      *this>>input;
      v.push_back(input);
    }
    return(*this);
  }

  template<typename Type>
  CBinaryIStream &operator>>(std::list<Type> &v) {
    v.clear();
    size_t size;
    *this>>size;
    Type input;
    for(size_t i=0; i<size; i++) {
      *this>>input;
      v.push_back(input);
    }
    return(*this);
  }

  template<typename Type>
  CBinaryIStream &operator>>(std::set<Type> &v) {
    v.clear();
    size_t size;
    *this>>size;
    Type input;
    for(size_t i=0; i<size; i++) {
      *this>>input;
      v.insert(input);
    }
    return(*this);
  }

  template<typename Type>
  CBinaryIStream &operator>>(std::multiset<Type> &v) {
    v.clear();
    size_t size;
    *this>>size;
    Type input;
    for(size_t i=0; i<size; i++) {
      *this>>input;
      v.insert(input);
    }
    return(*this);
  }

  template<typename KType, typename DType>
  CBinaryIStream &operator>>(std::map<KType, DType> &v) {
    v.clear();
    size_t size;
    *this>>size;
    pair<KType, DType> input;
    for(size_t i=0; i<size; i++) {
      *this>>input;
      v.insert(input);
    }
    return(*this);
  }


};


}; // namespace Codebook

#endif
