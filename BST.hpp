

/* COURSE:      CSE 100 : Advanced Data Structures
 * FILE:        BST.hpp
 * DESCRIPTION: Holds declarations and definitions of our binary
 *              search tree implementation.
 *              implementation
 * AUTHORS:     Madalynn Norton
 *              Jessica Redublo
 */

#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <iostream>

template<typename Data>
class BST {

/*   ------ MEMBER AND FUNCTION DECLARATIONS ------    */
public:

  /** Iterator as alias typename for BSTIterator<Data>. */
  typedef BSTIterator<Data> iterator;  // TODO TODO TODO TODO


  /** FUNCT:    bst()
   *  DESCR:    default constructor, initializes empty BST.
   *  PARAM:    none    (inclined bc trivial)
   */
  BST() : root(nullptr), isize(0) {}


  /** FUNCT:    ~bst()
   *  DESCR:    default destructor, deletes every node in BST. 
   *  PARAM:    none
   */
  ~BST();

  /** FUNCT:    insert( item )
   *  DESCR:    inserts copy of given Data item into BST.
   *  PARAM:    item, reference to Data
   *  RETURN:   pair, first elem is iterator pointing to new element in BST or
   *            duplicate in BST, second elem is true if new element inserted
   *            successfully
   */
  // should use only '<' operator when comparing Data items. (not ==,>,<=,>=)
  std::pair<iterator, bool> insert(const Data& item);


  /** FUNCT:    find( item )
   *  DESCR:    finds given Data item in BST.
   *  PARAM:    item, reference to Data
   *  RETURN:   iterator pointing to found item or past the last node if not
   *            found
   */
  // should use only '<' operator when comparing Data items. (not ==,>,<=,>=)
  iterator find(const Data& item) const;

  
  /** FUNCT:    size() 
   *  DESCR:    returns number of items currently in BST
   *  PARAM:    none
   *  RETURN:   int, the number of items in BST
   */ 
  unsigned int size() const;
  
  /** FUNCT:    height() 
   *  DESCR:    gets height of the BST (root is level 0, empty is -1)
   *  PARAM:    none
   *  RETURN:   int, the height of the BST
   */ 
   int height() const;
   int heightHelp( BSTNodeInt* n ) const;


  /** FUNCT:    empty()
   *  DESCR:    checks if BST is empty
   *  PARAM:    none
   *  RETURN:   true if BST empty, else false
   */
  bool empty() const;

  /** FUNCT:    begin()
   *  DESCR:    gets iterator at the smallest item in tree
   *  PARAM:    none
   *  RETURN:   iterator pointing to first (smallest) item in BST
   */ 
  iterator begin() const;

  /** FUNCT:    end()
   *  DESCR:    gets iterator past last item in tree
   *  PARAM:    none
   *  RETURN:   iterator pointing past last item in BST
   */
  iterator end() const;


private:

  BSTNode<Data>* root;  // Pointer to root of this BST, or 0 if empty
  unsigned int isize;   // Number of Data items stored in this BST
  static BSTNode<Data>* first(BSTNode<Data>* root); // Find first element of BST, helper for begin method
  static void deleteAll(BSTNode<Data>* n);  // Deletes nodes in postorder transversal

};


/*   ------ FUNCTION DEFINITIONS ------    */


/** Default destructor.
    Delete every node in this BST.
*/
template <typename Data>
BST<Data>::~BST() {
  deleteAll(root);
}


/** Given a reference to a Data item, insert a copy of it in this BST.
 *  Return a pair where the first element is an iterator pointing to either the newly inserted
 *  element or the element that was already in the BST, and the second element is true if the 
 *  element was newly inserted or false if it was already in the BST.
 * 
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=)  
 */
template <typename Data>
std::pair<BSTIterator<Data>, bool> BST<Data>::insert(const Data& item) {
  //std::pair<BSTIterator<Data>, bool> pair(asdfsad, false);
  int inserted = 0;

  // empty tree
  if (!root) {
    root = new BSTNodeInt(item);
    ++isize;
    BSTIterator<Data> iter;
    iter = BSTIterator<Data>(root);
    std::pair<BSTIterator<Data>, bool> pair(iter, true);
    return pair;
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
        BSTIterator<Data> iter;
        iter = BSTIterator<Data>(curr);
        std::pair<BSTIterator<Data>, bool> pair(iter, false);
        return pair;  // duplicate
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
        BSTIterator<Data> iter;
        iter = BSTIterator<Data>(curr);
        std::pair<BSTIterator<Data>, bool> pair(iter, false);
        return pair;  // duplicate
    }
  }

  ++isize;
  BSTIterator<Data> iter;
  iter = BSTIterator<Data>(newNode);
  std::pair<BSTIterator<Data>, bool> pair(iter, true);
  return pair;  // duplicate

  //return std::pair<BSTIterator<Data>, bool>(BSTIterator<Data>(0), false);
}


/** Find a Data item in the BST.
 *  Return an iterator pointing to the item, or pointing past
 *  the last node in the BST if not found.
 *  Note: This function should use only the '<' operator when comparing
 *  Data items. (should not use ==, >, <=, >=).  For the reasoning
 *  behind this, see the assignment writeup.
 */
template <typename Data>
BSTIterator<Data> BST<Data>::find(const Data& item) const
{
    BSTNodeInt* curr = root;
    while (curr) {
    if (curr->data < item) {
      
      if (!curr->right) {
        return end();
      }
      
      curr = curr->right;
    }
    else if (item < curr->data) {

     if (!curr->left) {
        return end();
     }

     curr = curr->left;
    }
    else {
      return BSTIterator<Data>(curr);
    }
  }
  
  return end();
}

  
/** Return the number of items currently in the BST.
 */ 
template <typename Data>
unsigned int BST<Data>::size() const
{
  return isize;
}

/** Return the height of the BST.
 */
template <typename Data> 
int BST<Data>::height() const
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


// HEIGHT RECURSION
int BST<Data>::heightHelp( BSTNodeInt* n )  const  {

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
template <typename Data>
bool BST<Data>::empty() const
{
  if (!root || isize == 0) {
    return true;
  }

  return false;
}

/** Return an iterator pointing to the first (smallest) item in the BST.
 */ 
template <typename Data>
BSTIterator<Data> BST<Data>::begin() const
{
  return BSTIterator<Data>(first(root));
}

/** Return an iterator pointing past the last item in the BST.
 */
template <typename Data>
BSTIterator<Data> BST<Data>::end() const
{
  return BSTIterator<Data>(nullptr);
}

/** Find the first element of the BST
 * Helper function for the begin method above.
 */ 
template <typename Data>
BSTNode<Data>* BST<Data>::first(BSTNode<Data>* root)
{
  // CHECK THIS
  if (root->left) {
    return first(root->left);
  }
  else
    return root;
}

/** do a postorder traversal, deleting nodes
 */
template <typename Data>
void BST<Data>::deleteAll(BSTNode<Data>* n)
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



#endif //BST_HPP
