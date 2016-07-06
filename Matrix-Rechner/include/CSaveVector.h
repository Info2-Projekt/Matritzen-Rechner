
#ifndef __CSAVEVECTOR_H
#define __CSAVEVECTOR_H

#include <vector>

namespace Codebook {

template<typename Type, typename Allocator=std::allocator<Type> >
class CSaveVector : public std::vector<Type, Allocator> {

public:
  typedef std::vector<Type, Allocator> base_type;
  typedef CSaveVector<Type, Allocator> my_type;
  typedef base_type::size_type size_type;
  typedef base_type::reference reference;
  typedef base_type::const_reference const_reference;

  explicit CSaveVector(const Allocator &a=Allocator())
    : base_type(a)
  {}

  CSaveVector(const my_type &v)
    : base_type(v)
  {}

  CSaveVector(size_type n, const Type &obj=Type(), const Allocator &a=Allocator())
    : base_type(n,obj,a)
  {}

  template<typename Input>
  CSaveVector(Input begin, Input end, const Allocator &a=Allocator())
    : base_type(begin, end ,a)
  {}

  reference operator[](size_type offset) {
    return(at(offset));
  }

  const_reference operator[](size_type offset) const {
    return(at(offset));
  }
};


}; //  namespace Codebook

#endif
