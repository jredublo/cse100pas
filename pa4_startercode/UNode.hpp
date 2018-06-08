//FILE: UNode.hpp

#include <iostream>
#include <vector>


using namespace std;



class UNode {
public:
    string name;
    int size; 
    UNode* parent;

    // Constructor
    UNode(string value)
    :name(value),
    parent(nullptr),
    size(1) {}

    ~UNode() {} //TODO

    string getName() const {
        return name;
    }

    UNode* getParent() {
        return parent;    
    }

    void setParent(UNode* par) {
        parent = par;
    }

    // returns the sentinel node of the calling object
    UNode* find() {
        vector<UNode*> storage;
        UNode* current = this;
        // while the parent still exists, we are not at the sentinel node
        while (current->parent) {
            storage.push_back(current);
            current = current->parent;
        }
        //current should now be pointing to the sentinel node
        for (unsigned int i = 0; i < storage.size(); i++) {
            // pointing all the intermediate nodes to the sentinel node
            UNode* c = storage.at(i);
            c->parent = current;
        }        
        return current;
    }
   

    /** Operator overloading */
    bool operator<(const UNode& other) const {
        return name < other.getName();
    }
    bool operator==(const UNode& other) const {
        if (name == other.getName()) return true;
        else return false;
    }

};
