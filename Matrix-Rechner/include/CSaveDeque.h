
#ifndef __CSAVEDEQUE_H
#define __CSAVEDEQUE_H

#include <deque>

namespace Codebook {

template<typename Type, typename Allocator=std::allocator<Type> >
class CSaveDeque : public std::deque<Type, Allocator> {

public:
  typedef std::deque<Type, Allocator> base_type;
  typedef CSaveDeque<Type, Allocator> my_type;
  typedef base_type::size_type size_type;
  typedef base_type::reference reference;
  typedef base_type::const_reference const_reference;

  explicit CSaveDeque(const Allocator &a=Allocator())
    : base_type(a)
  {}

  CSaveDeque(const my_type &v)
    : base_type(v)
  {}

  CSaveDeque(size_type n, const Type &obj=Type(), const Allocator &a=Allocator())
    : base_type(n,obj,a)
  {}

  template<typename Input>
  CSaveDeque(Input begin, Input end, const Allocator &a=Allocator())
    : base_type(begin, end ,a)
  {}

  reference operator[](size_type offset) {
    return(at(offset));
  }

  const_reference operator[](size_type offset) const {
    return(at(offset));
  }
};


}; // namespace Codebook

#endif
