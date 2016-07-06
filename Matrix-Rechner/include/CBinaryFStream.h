
#ifndef __CBINARYFSTREAM_H
#define __CBINARYFSTREAM_H

#include "CBinaryStream.h"


namespace Codebook {

class CBinaryOFStream : public CBinaryOStream {
private:
  std::ofstream m_stream;

public:
  CBinaryOFStream(const std::string &name) {
    m_stream.open(name.c_str(),std::ios::out|std::ios::binary);
  }

  void write(const void *mem, std::streamsize size) {
    m_stream.write(reinterpret_cast<const char*>(mem), size);
  }

  bool is_open() {
    return(m_stream.is_open());
  }

  void close() {
    m_stream.close();
  }

  bool fail() {
    return(m_stream.fail());
  }

};




class CBinaryIFStream : public CBinaryIStream {
private:
  std::ifstream m_stream;

public:
  CBinaryIFStream(const std::string &name) {
    m_stream.open(name.c_str(),std::ios::in|std::ios::binary);
  }

  void read(void *mem, std::streamsize size) {
    m_stream.read(reinterpret_cast<char*>(mem), size);
  }

  bool is_open() {
    return(m_stream.is_open());
  }

  void close() {
    m_stream.close();
  }

  bool fail() {
    return(m_stream.fail());
  }

  bool eof() {
    return(m_stream.eof());
  }


};


}; // namespace Codebook

#endif
