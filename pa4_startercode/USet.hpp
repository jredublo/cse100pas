// TODO TODO we deleted the year class, changed movie class so that it is the 
// year then title and then the set would sort acc to the strings bla bla

// File: USet.hpp

#include <queue>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <iostream>
#include "UNode.hpp"
#include <map>
#include <utility>

using namespace std;


/** MOVIE: Class to hold movie and their cast */
class Movie {
public: 
    string yrtitle;                 // yeartitle of movie
    set<UNode*> cast;               // castlist of movie    change to a set?

    Movie(string val):              // Constructor
        yrtitle(val) {}

    void addToCast (UNode* catmember) {
        cast.insert(catmember);
    }

    bool operator<(const Movie& other) const {
        return yrtitle < other.yrtitle;
    }
    bool operator==(const Movie& other) const {
        if (yrtitle==other.yrtitle) return true;
        else return false;
    }
};

class compa {
public:
    int operator() (Movie* m1, Movie* m2) {
        return m1->yrtitle < m2->yrtitle;
    }
};


/** Class to create the disjoint sets */
class USet {
private:
    // Members
    set<UNode*> sentinels;      // all disjoint sets repr. by their sentinels
    map<string, UNode*> sentAddrs;      // strings and their node's addresses
    set<Movie*,compa> movies;         // set of movies that are sorted by year then alphab

public:
    /** Constructor */
    USet() {}


    /** Destructor TODO */
    ~USet() {
        // delete(years);
        // delete(sentinels);   // need to iterate thru vector and set?
    }


    /** Returns set of sentinels */
    set<UNode*> getSentinels() {
        return sentinels;
    }


    /** Returns size of graph associated by given sentinel */
    int getSize(UNode* sen) {
        return sen->size;
    }


    /** Loads input file of all actors+movies+years and adds all to set */
    void initialize(const char* in_file) {
        ifstream infile(in_file);
        bool have_header = false;
        while (infile) {                    // read in line by line
            string s;
            if (!getline( infile,s )) break;
            if (!have_header) {             // skip header
                have_header = true;
                continue;
            }
            istringstream ss( s );
            vector<string> record;
            while (ss) {
                string next;
                if (!getline( ss, next, '\t' )) break;
                record.push_back( next );            
            }
            if (record.size() != 3) continue;

            string name(record[0]);     // record holds name, title, year
            string title(record[1]);
            string year(record[2]);
            string YRtitle = year + title;

            UNode* act;
            
            // check for duplicates
            auto letsGo = sentAddrs.find(name);
            if (letsGo == sentAddrs.end()) {  // no duplicate
                act = new UNode(name);
                sentinels.insert(act);
                sentAddrs.insert(pair<string, UNode*>(name,act));
            } else {
                act = letsGo->second;
            }

            //check to see if the movie already exists before adding to the cast
            set<Movie*>::iterator moveIt;
            bool found = false;
            Movie* movObj;
            for (moveIt = movies.begin(); moveIt != movies.end(); ++moveIt) {
                if ((*moveIt)->yrtitle == YRtitle) {
                    movObj = *moveIt;
                    movObj->cast.insert(act);
                    found = true;
                }
            }
            if (!found) {
                //create a new Movie and add to its cast
                movObj = new Movie(YRtitle);
                //add actor to cast
                (movObj->cast).insert(act);
                movies.insert(movObj);
            }
        }
    }



    /** Finds and returns the sentinel node of the calling object */
    UNode* find( UNode* thisNode ) {
        //cout << "this unode: " << thisNode << endl;
        //cout << "this nodes addr: " << &this << endl;
        vector<UNode*> storage;
        UNode* current = thisNode;
        // while the parent still exists, we are not at the sentinel node
        while (current->parent != nullptr) {
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



    /** Returns true if two nodes are connected (sentinels are the same) */
    bool isConnected(UNode* first, UNode* second) {
        //cout << "Here!" << endl;
        //cout << "IN ISCONNECTED, FIND(FIRST)= " << find(first) << endl;
        //cout << "IN ISCONNECTED, FIND(2ND)= " << find(second) << endl;
        if (find(first) == find(second))
            return true;
        return false;
    }
    

    /** Connect graphs of the passed nodes, union by size */
    UNode* unionSize( UNode* first, UNode* second ) {
        // uses find to get sentinels of first and second
        UNode* sent1 = find(first);
        UNode* sent2 = find(second);
        UNode* finalSent;
        //cout << "Sentinel node of first in union size: " << sent1 << endl;
        //cout << "Sentinel node of 2nd in union size: " << sent2 << endl;
        if (sent1==sent2) {
            return sent1;
        }
        
        if (sent1->size > sent2->size) {
            sent2->parent = sent1;
            sent1->size = sent1->size + sent2->size;
            finalSent = sent1;
        }
        else {
            sent1->parent = sent2;
            sent2->size = sent2->size + sent1->size;
            finalSent = sent2;
        }
        //cout << "finalSent returned from unionSize is: " << finalSent << endl;
        return finalSent;
    }


    /** Returns the earliest year two actors are connected
     *    Assume initialize is called RIGHT BEFORE THIS. (all nodes
     *    are sentinels and disconnected)
     */
    int actorConnectUF(string firstStr, string secondStr) {
        // getting nodes
        //cout << "Calling actorconnect on " <<firstStr << " & " << secondStr << endl;
        map<string,UNode*>::iterator lastIt = sentAddrs.find(firstStr);
        UNode* first = lastIt->second;
        lastIt = sentAddrs.find(secondStr);
        UNode* second = lastIt->second;

        set<Movie*>::iterator it = movies.begin();
        Movie* mv = *(it);
        string yr = mv->yrtitle;
        int finalyr = stoi(yr.substr(0,4));

        priority_queue<UNode*> pq;

        for (it = movies.begin(); it != movies.end(); ++it) {
            Movie* currMov = *it;
            string curry = currMov->yrtitle;
            //cout << "current yrtitle: " << curry << endl;
            //cout << "changing finyr to " << stoi(curry.substr(0,4)) << endl;
            finalyr = stoi(curry.substr(0,4));
            // push the whole cast to the pq
            set<UNode*>& filmcast = currMov->cast;
            set<UNode*>::iterator iter;
            for (iter = filmcast.begin(); iter != filmcast.end(); ++iter) {
                //cout << "filmcast size is: " << filmcast.size() << endl;
                //cout << "currMov->cast size is: " << currMov->cast.size() << endl;
                UNode* friendo = *iter;
                pq.push(friendo);
                //cout << "pushing " << friendo->name << " at addr " << friendo << endl; 
            }
            // union EVERYTHING in the pq (which may include things from prev movies)
            while (pq.size() > 1) {
                //cout << "Went into while" << endl;
                UNode* pop1 = pq.top();
                pq.pop();
                //cout << "here" << endl;
                UNode* pop2 = pq.top();
                pq.pop();
                
                //cout << "Unioning " << pop1->name << " and " << pop2->name << endl;
                UNode* newSent;
                newSent = unionSize(pop1,pop2);
                pq.push(newSent);
            }
            //cout << "Out of while" << endl;
            UNode* sentinelGOD = pq.top();
            pq.pop();
            
            // check if the actors are connected
            if (!isConnected(first,second)) {
                continue;
                //pq.push(sentinelGOD);
                //cout << "Not connected! Pushing sentinelGOD " << sentinelGOD->name << " at addr " << sentinelGOD << endl;
            }
            else {
                //cout << "Theyre connected!" << endl;
                break;
            }
        }
    
        return finalyr;
    }

};
