#ifndef __CSETTREE_H
#define __CSETTREE_H

#include "CTree.h"
#include "CMapTree.h"

namespace Codebook {


//*********************************************************
// set_tree_traits
//*********************************************************

template<typename VType, typename Cmp= std::less<VType> >
class set_tree_traits {
public:
  typedef VType value_type;
  typedef VType key_type;

//*********

  static bool eq(const key_type &k1, const key_type &k2) {
    return(k1==k2);
  }

//*********

  static bool lt(const key_type &k1, const key_type &k2) {
    return(Cmp()(k1,k2));
  }

//*********

  static bool gt(const key_type &k1, const key_type &k2) {
    return(lt(k2,k1));
  }

//*********

  static bool ne(const key_type &k1, const key_type &k2) {
    return(!eq(k1,k2));
  }

//*********

  static bool ge(const key_type &k1, const key_type &k2) {
    return(!lt(k1,k2));
  }

//*********

  static bool le(const key_type &k1, const key_type &k2) {
    return(!lt(k2,k1));
  }

//*********

  static const key_type &getKey(const value_type &v) {
    return(v);
  }
};

//*********************************************************
// CSetTree
//*********************************************************
template<typename VType, typename Cmp= std::less<VType> >
class CSetTree : public CTree<set_tree_traits<VType, Cmp> > {
public:
  typedef CTree<set_tree_traits<VType, Cmp> > base_type;
  typedef CSetTree<VType, Cmp> my_type;

  CSetTree()
  : base_type()
  {}
//*********
  template<typename Input>
  CSetTree(Input beg, Input end)
  : base_type(beg,end)
  { }
//*********
  CSetTree(const CSetTree &t)
  : base_type(t)
  { }
};




}; // namespace Codebook

#endif
