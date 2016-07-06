
#ifndef __STLFUNCTIONS_H
#define __STLFUNCTIONS_H

#include <fstream>
#include <string>

namespace Codebook {

template<class Forward1, class Forward2> inline
Forward1 find_first_not_of(Forward1 first1, Forward1 last1,
                           Forward2 first2, Forward2 last2) {
  for (; first1 != last1; ++first1) {
    for (Forward2 mid2 = first2; mid2 != last2; ++mid2)
      if (*first1 == *mid2)
        break;
    if(mid2==last2)
      return(first1);
  }
  return (first1);
}


}; // namespace Codebook

#endif
