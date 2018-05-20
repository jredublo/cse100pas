
#include "BSTInt.h"


/* Function definitions for a int-based BST class */


/** Default destructor.
    Delete every node in this BST.
*/
BSTInt::~BSTInt() {
  deleteAll(root);
}

/** Given a reference to a Data item, insert a copy of it in this BST.
 *  Return  true if the item was added to this BST
 *  as a result of this call to insert,
 *  false if an item equal to this one was already in this BST.
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=)  For the reasoning
 *  behind this, see the assignment writeup.
 */

bool BSTInt::insert(int item)
{
  int inserted = 0;

  if (!root) {
    root = new BSTNodeInt(item);
    ++isize;
    return true;
  }

  BSTNodeInt* curr = root;
  
  // two children
  while (curr->left && curr->right) {
    if (item < curr->data) {
      curr = curr->left;
    }
    else if (curr->data < item) {
      curr = curr->right;
    }
    else {
      return false;  // duplicate
    }
  }

  // Ready to insert
  BSTNodeInt* newNode = new BSTNodeInt(item);
  while (inserted == 0) {
    if (item < curr->data) {
        if (curr->left) {
            curr=curr->left;
        }
        else {
            curr->left = newNode;
            newNode->parent = curr;
            inserted = 1;
        }
    }
    else if(curr->data < item) {
        if (curr->right) {
            curr=curr->right;
        }
        else {
            curr->right = newNode;
            newNode->parent = curr;
            inserted = 1;
        }
    }
    else {
        delete newNode;
        return false; // duplicate
    }
  }

  ++isize;
  return true;
}



/** Find a Data item in the BST.
 *  Return true if the item is in the BST or false otherwise
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=).  For the reasoning
 *  behind this, see the assignment writeup.
 */
bool BSTInt::find(int item) const
{
  BSTNodeInt* curr = root;
  while (curr) {
    if (curr->data < item) {
      
      if (!curr->right) {
        return false;
      }
      
      
      curr = curr->right;
    }
    else if (item < curr->data) {

     if (!curr->left) {
        return false;
     }


      curr = curr->left;
    }
    else {
      return true;
    }
  }
  return false;
}

  
/** Return the number of items currently in the BST.
 */
unsigned int BSTInt::size() const 
{
  return isize;
}

  
/** Return the height of the BST.
    Height of tree with just root node is 0
    height of an empty tree is -1
 */
int BSTInt::height() const
{

  //if tree is empty
  if(empty()) {
    return -1;
  }
    
  else {
    int max = heightHelp(root);
    return max -1;
  }

  return 0;
}


int BSTInt::heightHelp( BSTNodeInt* n )  const  {

    if (!n) {
        return 0;
    }

    else if (!n->left && !n->right) {
        return 1;
    }

    else {
        int ltree = heightHelp(n->left);
        int rtree = heightHelp(n->right);

        int max;
        if (ltree > rtree) {
            max = ltree; 
        } 
    
        else {
            max = rtree;
        }

        return max + 1;
    }


}


/** Return true if the BST is empty, else false. 
 */
bool BSTInt::empty() const 
{
  if (!root || isize == 0) {
    return true;
  }
  return false;
}




/** do a postorder traversal, deleting nodes
 * This is a helper for the destructor
 * This method is static because it acts on BSTNodeInts
 *   and not on the BSTInt object directly.
 */
void BSTInt::deleteAll(BSTNodeInt* n)
{
  if (!n)
      return;

  else {
    if(n->left)
        deleteAll(n->left);

    if(n->right)
        deleteAll(n->right);

    delete n;
  }
}


