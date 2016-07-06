
#ifndef __CMEMORY_H
#define __CMEMORY_H


namespace Codebook {

template <typename Type>
class CMemory {
  private:
    Type *m_memory;

  public:

  CMemory(unsigned long anz) {
    m_memory=new(Type[anz]);
    if(!m_memory)
      throw(bad_alloc("CMemory::CMemory"));
  }

  ~CMemory()  {
     delete[](m_memory);
  }

  operator Type*() {
    return(m_memory);
  }
};


}; // namespace Codebook

#endif
