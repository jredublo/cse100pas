// FILE: HCNODE.cpp

#include "HCNode.h"
#include <iostream>

using namespace std;


class myComparator
{
public:
    int operator() (HCNode* p1, HCNode* p2) {
      if (p1->count != p2->count)
        return p1->count > p2->count;
      else
        return p1->symbol > p2->symbol;
    }
};

/**Less-than comparison, so HCNodes will work in std::priority_queue
 * We want small counts to have high priority.
 * And we want to break ties deterministically.
 */

bool HCNode::operator<(const HCNode& other) {
  //if counts are different, just compare counts
  if (count != other.count) {
    if (count < other.count)
      return true;
    else
      return false;
  }
  // counts are equal, use symbol to break tie
  else {
    if (symbol < other.symbol) {
      return true;
    }
    else {
      return false;
    }
  
  }
}


