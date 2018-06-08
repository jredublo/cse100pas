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
};



/** Class to create the disjoint sets */
class USet {
private:
    // Members
    set<UNode*> sentinels;      // all disjoint sets repr. by their sentinels
    map<string, UNode*> sentAddrs;      // strings and their node's addresses
    set<Movie*> movies;         // set of movies that are sorted by year then alphab

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
            string yrtitle = year + title;

            UNode* act = new UNode(name);
            sentinels.insert(act);      // duplicates are caught by set 
            sentAddrs.insert(pair<string, UNode*>(name,act));
            Movie* movObj = new Movie(yrtitle);
            //add actor to cast
            movObj->addToCast(act);
            movies.insert(movObj);
        }
    }


    /** Returns true if two nodes are connected (sentinels are the same) */
    bool isConnected(UNode* first, UNode* second) {
        if (first->find() == second->find())
            return true;
        return false;
    }
    

    /** Connect graphs of the passed nodes, union by height/size? */
    UNode* unionSize( UNode* first, UNode* second ) {
        // uses find to get sentinels of first and second
        UNode* sent1 = first->find();
        UNode* sent2 = second->find();
        UNode* finalSent;
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
        return finalSent;
    }


    /** Returns the earliest year two actors are connected
     *    Assume initialize is called RIGHT BEFORE THIS. (all nodes
     *    are sentinels and disconnected)
     */
    int actorConnectUF(string firstStr, string secondStr) {
        // getting nodes
        map<string,UNode*>::iterator lastIt = sentAddrs.find(firstStr);
        UNode* first = lastIt->second;
        lastIt = sentAddrs.find(secondStr);
        UNode* second = lastIt->second;

        set<Movie*>::iterator it = movies.begin();
        Movie* mv = *(it);
        string yr = mv->yrtitle;
        int finalyr = stoi(yr.substr(0,3));

        priority_queue<UNode*> pq;

        for (it = movies.begin(); it != movies.end(); ++it) {
            Movie* currMov = *it;
            string curry = currMov->yrtitle;
            finalyr = stoi(curry.substr(0,3));
            // push the whole cast to the pq
            set<UNode*> filmcast = currMov->cast;
            set<UNode*>::iterator iter;
            for (iter = filmcast.begin(); iter != filmcast.end(); ++iter) {
                UNode* friendo = *iter;
                pq.push(friendo);
            }
            // union EVERYTHING in the pq (which may include things from prev movies)
            while (pq.size() > 2) {
                UNode* pop1 = pq.top();
                pq.pop();
                UNode* pop2 = pq.top();
                pq.pop();
                UNode* newSent = unionSize(pop1,pop2);
                pq.push(newSent);
            }
            UNode* sentinelGOD = pq.top();
            pq.pop();

            // check if the actors are connected
            if (!isConnected(first,second))
                pq.push(sentinelGOD);
        }
    
        return finalyr;
    }

};
