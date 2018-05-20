#include "BSTInt.h"
#include "BST.hpp"
#include "BSTIterator.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>

using namespace std;

/**
 * A test driver for the BST int class and class template.
 * PA1 CSE 100 2016
 * Author: Christine Alvarado and
 *    Jessica Redublo
 *    Madalynn Norton
 */
int main() {

    /* Create an STL vector of some ints */
    /*test*/
    vector<int> v;
    v.push_back(3);
    v.push_back(4);
    v.push_back(1);
    v.push_back(100);
    v.push_back(-33);

    /* Create an instance of BST holding int */
    BSTInt b;           // original
    BSTInt chain;       // chain
    BSTInt chain2;      // chain 2
    BSTInt simple;      // 3 4 3
    BSTInt root;        // only root...
    BSTInt tricky;      // more complicated tree for testing height()

    // Testing On Empty Tree
    if(b.find(3)) {     // test find()
        cout << "find() on empty tree: Fail 3"  << endl;
        return -1;
    }
    else
        cout << "Empty tree is correctly empty." << endl;
    
    if(!b.empty()) {    // test empty()
        cout << "empty() on empty tree: Fail" << endl;
        return -1;
    }
    else
        cout << "empty() method works on empty tree" << endl;

    // Testing height() on edge cases    
    if(b.height() == -1)    // test height() on empty tree
        cout << "height() works on empty tree" << endl;
    else {
        cout << "height() incorrect, value returned: " << b.height() << endl;
        return -1;
    }

    root.insert(1);
    if(root.height() == 0) //test height() on root only tree
        cout << "height() works on root only tree" << endl;
    else{
        cout << "height() incorrect, value returned: " << root.height() << endl;
        return -1;
    }

    // Filling Tree and Using insert()
    for(int item : v) {
        if(!b.insert(item)) {
            cout << "Incorrect bool return value when inserting " << item 
                 << endl;
            return -1;
        }
        else
            cout << "Successful insertion of " << item << endl;
    }


    if (b.size() == 5) {
        cout << "Correct size 5" << endl;
    }
    else {
        cout << "Size is incorrect after duplicate: " << b.size() << endl;
        return -1;
    }


    // Adding to chain
    chain.insert(3);
    chain.insert(1);
    chain.insert(2);
    // test find()
    if ( chain.find(1) && chain.find(2) && chain.find(3) ) {
        cout << "Chain is correct." << endl;
    }
    else {
        cout << "Chain is incorrect." << endl;
        return -1;
    }


    // Adding to chain2
    chain2.insert(3);
    chain2.insert(100);
    chain2.insert(400);
    chain2.insert(50);
    // test find()
    if ( chain2.find(3) && chain2.find(100) && chain2.find(400) && chain2.find(50) ) {
        cout << "Chain2 IT WORKS" << endl;
    }
    else {
        cout << "Chain2 fix it fam" << endl;
        return -1;
    }
    
    // adding to simple
    simple.insert(3);
    simple.insert(4);
    simple.insert(3);
    if (simple.height() == 1)
        cout << "height() correct for duplicates" << endl;
    else{
        cout << "height() incorrect for duplicates, incorrect val: " << simple.height() << endl;
        return -1;
    }


    if (chain2.height() == 2)       // test height()
        cout << "height() is working for chain2" << endl;
    else {
        cout << "height() fail. Should be 2. Incorrect height: " << chain2.height() << endl;
        return -1;
    }

    tricky.insert(1);
    tricky.insert(10);
    tricky.insert(3);
    tricky.insert(11);
    tricky.insert(15);
    tricky.insert(12);
    tricky.insert(20);
    if (tricky.height() == 4)
        cout << "height() works for tricky" << endl;
    else {
        cout << "height() doesn't work for big tree. Value returned: " << tricky.height() << endl;
        return -1;
    }

    /* Test size. */
    cout << "Size is: " << b.size() << endl;
    if(b.size() != v.size()) {
        cout << "... which is incorrect." << endl;
        return -1;
    }

    /* Test find return value. */
    // Test the items that are already in the tree
    for(int item : v) {
        if(!b.find(item)) {
            cout << "Incorrect return value when finding " << item << endl;
            return -1;
        }
    }

    // Testing find on value that should not be there
    if(!b.find(7)) {
        cout << "Didn't find value thats not there" << endl;
    }

    if(b.find(4)) {
        cout << "found 4 in the tree" << endl;
    }
    else {
        cout << "didnt find 4" << endl;
    }

    b.insert(70);
    if (b.find(100)) {
        cout << "100 is here" << endl;
    }
    else 
        cout << "100 is overwritten" << endl;

    if (b.empty() == false) {
        cout << "empty() works on non empty tree" << endl;
    }




    /* UNCOMMENT THE LINES BELOW TO TEST THE TEMPLATE-BASED ITERATOR */
    // Test the template version of the BST  with ints 
    BST<int> btemp;
    for (int item : v) {
        // The auto type here is the pair of BSTIterator<int>, bool
        cout << "Inserting " << item << " into the int template-based BST...";
        auto p = btemp.insert(item);
        if (*(p.first) != item) {
            cout << "Wrong iterator returned.  "
                 << "Expected " << item << " but got " << *(p.first) << endl;
            return -1;
        }
        if (!p.second) {
            cout << "Wrong boolean returned.  Expected true but got " 
                 << p.second << endl;
            return -1;
        }
        cout << "success!" << endl;
              
    }
    // Now test finding the elements we just put in
    for (int item: v) {
        cout << "Finding " << item << "...." << endl;
        BSTIterator<int> foundIt = btemp.find(item);
        if (*(foundIt) != item) {
            cout << "incorrect value returned.  Expected iterator pointing to "
                 << item << " but found iterator pointing to " << *(foundIt) 
                 << endl;
            return -1;
        }
        cout << "success!" << endl;
    }
    // Test the iterator: The iterator should give an in-order traversal
    // Sort the vector, to compare with inorder iteration on the BST
    sort(v.begin(),v.end());

    cout << "traversal using iterator..." << endl;
    auto vit = v.begin();
    auto ven = v.end();

    // This is equivalent to BSTIterator<int> en = btemp.end();
    auto en = btemp.end();

    //This is equivalent to BST<int>::iterator it = btemp.begin();
    auto it = btemp.begin();
    for(; vit != ven; ++vit) {
        if(! (it != en) ) {
            cout << *it << "," << *vit 
                 << ": Early termination of BST iteration." << endl;
            return -1;

        }
        cout << *it << endl;
        if(*it != *vit) {
            cout << *it << "," << *vit 
                 << ": Incorrect inorder iteration of BST." << endl;
            return -1;
        }
        ++it;
    }

    cout << "success!" << endl;



    BST<char> charTree;
    
    if (charTree.height() != -1) {
        cout << "Height incorrect for empty tree. SHould be -1, but is " << charTree.height() << endl;
        return -1;
    }
    
    if (charTree.empty() != true) {
        cout << "Empty() incorrect for empty tree. Should be false." << endl;
        return -1;
    }

    auto j = charTree.insert('J');
    if (*(j.first) != 'J') {
        cout << "Wrong iterator returned from insert." << endl;
        return -1;
    }

    if (charTree.height() != 0) {
        cout << "Height incorrect for root only tree. Should be 0 but is " << charTree.height() << endl;
        return -1;
    }

    auto c = charTree.insert('C');
    if ( (c.first).operator*() != 'C' ) {
        cout << "Insert failed. Should be C but is " << (c.first).operator*() << endl;
        return -1;
    }
    auto a = charTree.insert('A');
    auto d =charTree.insert('D');
    auto w = charTree.insert('W');
    if ( (w.first).operator*() != 'W' ) {
        cout << "Insert failed. Should be W but is " << (w.first).operator*() << endl;
        return -1;
    }

    if (charTree.height() != 2) {
        cout << "Height incorrect. Should be 2, but is " << charTree.height() << endl;
        return -1;
    }
    if (charTree.size() != 5) {
        cout << "Size does not equal 5. Is " << charTree.size() << endl;
        return -1;
    }
     
    charTree.insert('D'); // duplicate
    if (charTree.height() != 2 || charTree.size() != 5) {
        cout << "Height or Size incorrect for duplicate insertions." << endl;
        return -1;
    }
    auto iter1 = charTree.find('C');
    if ( iter1.operator*() != 'C' ) {
        cout << "Find failed. Should be C but is " << iter1.operator*() << endl;
        return -1;
    
    }    
    auto iter2 = charTree.find('X');
    if (iter2.operator*()) {
        cout << "Found element that should not be in tree." << endl;
        return -1;
    }


    // testing successor and operator
    if ( *(j.first).operator++() != 'W' ) {
        cout << "Successor and pre increment op failed. Should be W." << endl;
        return -1;
    }
    if ( *(a.first).operator++() != 'C' ) {
        cout << "Successor and pre increment op failed. Should be C." << endl;
        return -1;
    }
    if ( *(w.first).operator++() ) {
        cout << "Successor and preincrement op failed. Should be NULL" << endl;
        return -1;
    }
    if ( *(d.first).operator++() != 'J' ) {
        cout << "Successor and pre increment op failed. Should be J." << endl;
        return -1;
    }

    *(a.first).operator++();
    if ( *(a.first).operator++() != 'J' ) {
        cout << "Double successor failed. Should be J." << endl;
        return -1;
    }
    
    // testing equality and inequality operators
    if ( (d.first).operator==((a.first)) == false ) {
        cout << "Equality operator did not work." << endl;
        return -1;
    }
    if ( (w.first).operator!=((a.first)) == false) {
        cout << "Inequality operator did not work." << endl;
        return -1;
    }
    
   // testing post increment operator
   char shouldJ = *(d.first).operator++(0);
   char shouldW = (d.first).operator*();
   if (shouldJ != 'J' || shouldW != 'W') {
      cout << "Post increment doesn't work" << endl;
      return -1;
   }
    cout << "All tests passed!" << endl;
    return 0;
}
