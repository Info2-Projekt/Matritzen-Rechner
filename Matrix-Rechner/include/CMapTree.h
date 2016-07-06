#ifndef __CMAPTREE_H
#define __CMAPTREE_H

#include "CTree.h"
#include <functional>

namespace Codebook {


//*********************************************************
// map_tree_traits
//*********************************************************
template<typename KType, typename VType, typename Cmp= std::less<KType> >
class map_tree_traits {
public:
  typedef typename std::pair<KType, VType> value_type;
  typedef KType key_type;

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
    return(v.first);
  }
};

//*********************************************************
// CMapTree
//*********************************************************

template<typename KType, typename VType, typename Cmp= std::less<KType> >
class CMapTree : public CTree<map_tree_traits<KType, VType, Cmp> > {
public:
  typedef CTree<map_tree_traits<KType, VType, Cmp> > base_type;
  typedef CMapTree<KType, VType, Cmp> my_type;

  CMapTree()
  : base_type()
  {}
//*********
  template<typename Input>
  CMapTree(Input beg, Input end)
  : base_type(beg,end)
  { }
//*********
  CMapTree(const CMapTree &t)
  : base_type(t)
  { }


};




}; // namespace Codebook

#endif
