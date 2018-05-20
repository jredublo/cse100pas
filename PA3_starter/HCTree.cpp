
/* FILES:   HCTree.cpp
 * AUTHORS:
 */

#include <iostream>
#include <queue>
#include <bits/stdc++.h>
#include <functional>
#include <vector>

#include "HCTree.h"

using namespace std;



/** COMPARATOR class to make a min heap and sort nodes */
class myComp
{
public:
    int operator() (HCNode* p1, HCNode* p2) {
      if (p1->count != p2->count)
        return p1->count > p2->count;
      else
        return p1->symbol > p2->symbol;
    }
};




/*  ~~~~~ BEGIN the HCTree functions ~~~~~  */

/**
 * Build Function:  building the huffman tree
 */
void HCTree::build(const vector<int>& freqs) {
    
  priority_queue< HCNode*, vector<HCNode*>, myComp > q;
     
  // make single forests with counts if nonzero and add to priority queue
  for (unsigned int i = 0; i < freqs.size(); i++) {
    int currentCount = freqs.at(i);
    if (currentCount != 0) {
      cout << "INT i is " << i << ". LEAF FOR " << (char)i << ": FREQ IS " << currentCount << endl;
      HCNode* newNode = new HCNode(currentCount, (byte)i, 0, 0, 0);
      leaves.at(i) = newNode;
      q.push(newNode);
    }
  }

  cout << endl;

  // while loop taking the lowest two in priority queue and combining until we have a full tree
  while (q.size() > 1) {
    HCNode* small = q.top();      // lowest
    cout << "TOP IN Q: " << small->symbol << "; ASCII: " << (int)(small->symbol) << "; freq: " << small->count << endl;
    q.pop();
    HCNode* nextSmall = q.top();  // second lowest
    cout << "NEXT TOP IN Q: " << nextSmall->symbol << "; ASCII: " << (int)(nextSmall->symbol) << "; freq: " << nextSmall->count << endl;
    q.pop();

    // making new sum node for lowest two and add to queue
    int sumInt = (small->count) + (nextSmall->count);
    HCNode* sum = new HCNode(sumInt, nextSmall->symbol, 0, 0, 0);
    small->p = sum;       // adjusting pointers
    nextSmall->p = sum;
    sum->c0 = small;
    sum->c1 = nextSmall;
    q.push(sum);
  }

  root = q.top();
}


/**
 * Encode Function:  encoding single symbols depending on tree ('a' -> 001)
 */
void HCTree::encode(byte symbol, ofstream& out) const {
  int iSym = (int)symbol;
  HCNode* current = leaves[iSym];
  
  if (!current) {
    cout << "FAIL at null check in encode fnc" << endl;
    return;
  }

  string code = "";
  while (current != root) {
    // curr is on left branch of its parent
    if (current == current->p->c0) {
      code = code + '0';
    }
    // curr is on right branch of its parent
    else if (current == current->p->c1){
      code = code + '1';
    }
    else {
      cout << "ERROR HERE" << endl;
      return;
    }
    current = current->p;
  }
  
  // reverse string
  reverse(code.begin(), code.end());
  out << code;
  cout << "CODE FOR " << (char)symbol << " is: " << code << endl;
}



/**
 * Encode Function:  encoding single symbols depending on tree ('a' -> 001)
 */
int HCTree::decode(ifstream& in) const {
  cout << "GOT HERE DECCCC" << endl;
  char c;
  HCNode* current = root;
  
  while (in.get(c)) {   // while loop to transverse tree to get codes
    
    // null check
    if (!current) {
      cout << "decode: CURRENT IS NULL" << endl;
      return -1;
    }

    
    // IS a leaf
    if (!current->c0 && !current->c1){
      cout << "decoding: is a leaf" << endl;
      break;
    }

    // IS NOT a leaf-> we keep moving!
    else {
      if (c == '0')     // char was a zero
        current = current->c0;
      else              // char was a one
        current = current->c1;
    }
  }

  return (int)(current->symbol);

}

