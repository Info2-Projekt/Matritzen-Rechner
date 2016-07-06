
#ifndef __CAVLMAPTREE_H
#define __CAVLMAPTREE_H

#include "CMapTree.h"
#include "CAVLTree.h"


namespace Codebook {



template<typename KType, typename VType, typename Cmp= std::less<KType> >
class CAVLMapTree : public CAVLTree<map_tree_traits<KType, VType, Cmp> > {
public:
  typedef CAVLTree<map_tree_traits<KType, VType, Cmp> > base_type;
  typedef CAVLMapTree<KType, VType, Cmp> my_type;

  CAVLMapTree()
  : base_type()
  {}
//*********
  template<typename Input>
  CAVLMapTree(Input beg, Input end)
  : base_type(beg,end)
  { }
//*********
  CAVLMapTree(const CAVLMapTree &t)
  : base_type(t)
  { }
};


}; // namespace Codebook

#endif
