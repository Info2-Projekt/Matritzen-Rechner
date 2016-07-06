
#ifndef __CAVLSETTREE_H
#define __CAVLSETTREE_H

#include "CSetTree.h"
#include "CAVLTree.h"


namespace Codebook {


template<typename VType, typename Cmp= std::less<VType> >
class CAVLSetTree : public CAVLTree<set_tree_traits<VType, Cmp> > {
public:
  typedef CAVLTree<set_tree_traits<VType, Cmp> > base_type;
  typedef CAVLSetTree<VType, Cmp> my_type;

  CAVLSetTree()
  : base_type()
  {}
//*********
  template<typename Input>
  CAVLSetTree(Input beg, Input end)
  : base_type(beg,end)
  { }
//*********
  CAVLSetTree(const CAVLSetTree &t)
  : base_type(t)
  { }
};

}; // namespace Codebook

#endif
