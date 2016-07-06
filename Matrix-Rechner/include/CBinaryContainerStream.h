
#ifndef __CBINARYCONTAINERSTREAM_H
#define __CBINARYCONTAINERSTREAM_H

#include "CBinaryStream.h"


namespace Codebook {

template<typename Type>
class CBinaryOContainerStream : public CBinaryOStream {
private:
  Type &m_container;
  bool m_failed;

public:
  CBinaryOContainerStream(Type &container)
    : m_container(container), m_failed(false)
  {}

  void write(const void *mem, std::streamsize size) {
    try {
      const char *ptr=reinterpret_cast<const char*>(mem);
      for(streamsize i=0; i<size; i+=1) {
        m_container.push_back(ptr[i]);
      }
    }
    catch(...) {
      m_failed=true;
    }
  }

  bool is_open() {
    return(true);
  }

  void close()
  {}

  bool fail() {
    return(m_failed);
  }

};


template<typename Type>
class CBinaryIContainerStream : public CBinaryIStream {
private:
  const Type &m_container;
  bool m_failed;
  Type::const_iterator m_pos;


public:
  CBinaryIContainerStream(Type &container)
    : m_container(container), m_failed(false), m_pos(container.begin())
  {}

  void read(void *mem, std::streamsize size) {
    try {
      char *ptr=reinterpret_cast<char*>(mem);
      for(streamsize i=0; i<size; i+=1) {
        ptr[i]=*(m_pos++);
      }
    }
    catch(...) {
      m_failed=true;
    }
  }

  bool is_open() {
    return(true);
  }

  void close() {
  }

  bool fail() {
    return(m_failed);
  }

  bool eof() {
    return(m_pos==m_container.end());
  }


};


}; // namespace Codebook

#endif
