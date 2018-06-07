
/*
 * FILE:     ActorGraph.cpp
 * AUTHOR:   Madalynn Norton, Jessica Redublo
 * DATE:     June 8, 2018
 * DESCR:    Holds the function definitions for ActorGraph that are essential
 *              for building a graph of actors and their movies and establishing
 *              connections between actors. Used for pathfinder and widestp
 *              actorconnections.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include "ActorGraph.h"

using namespace std;

typedef set<string> StringSet;              // set of strings
typedef map<string, StringSet> UmapStrings; // map (strings, StringSet)        <movieMap>
typedef set<ActorEdge> EdgeSet;             // set of ActorEdges
typedef map<ActorNode*, EdgeSet> UmapNodes; // map (ActorNode*, EdgeSet)       <theMap>
typedef map<string, ActorNode*> MapAddr;    // map (actor_name, node address)  <addrMap>
typedef pair<ActorNode*, EdgeSet> Pear;     // pair (ActorNode*, EdgeSet) for theMap
typedef pair<string, ActorNode*> Kiwi;      // pair (string, ActorNode*)  for addrMap
typedef pair<int, ActorNode*> Peach;        // pair (int, ActorNode*)     for dijkstra pq 


   /* --------------------------- FUNCTION DEFINTIONS --------------------------- */


/** Loads from input file and calls buildMovieMap and then buildTheMap to create the graph */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges, bool frmP)
{   // cout << "Got to loadFromFile" << endl;
    
    ifstream infile(in_filename);           // Initialize the file stream
    bool have_header = false;
    while (infile) {                        // Keep reading lines until end of file
        // cout << "~~~~~~~~~~~ while starting" << endl;
        string s;
        if (!getline( infile, s )) break;   // Get the next line
        if (!have_header) {                 // Skip header
            have_header = true;
            continue;
        }
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;                    // Get next string before tab and put in next
            if (!getline( ss, next, '\t' )) break;
            record.push_back( next );
        }
        if (record.size() != 3) continue;   // Exactly 3 column
        
        string actor_name(record[0]);
        string movie_title(record[1]);
        string movie_year(record[2]);
        string wholeMovie = movie_title + movie_year;   // Merge year and title
        buildMovieMap(actor_name, wholeMovie);          // Build movieMap
        //cout << "while ending~~~~~~~~~~~~~ " << endl;
    }

    buildTheMap(use_weighted_edges, frmP);         // Build theMap (actual graph)
    
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;
}



/** Builds movieMap line by line from input and preps theMap by adding ActorNodes */
void ActorGraph::buildMovieMap(string actor_name, string wholeMovie) 
{   // cout << "Begin buildMovieMap. ActorName: " << actor_name << "  WholeMov: " << wholeMovie << endl;
    
    // Building MovieMap
    UmapStrings::iterator it = movieMap.find(wholeMovie);
    if (it != movieMap.end()) {     // movie exists: add actor to cast
        // cout << "Made it to BMM 1st if" << endl;
        StringSet existingCast = it->second;
        existingCast.insert(actor_name);
        it->second = existingCast;
    } else {                        // movie DNE: add movie and castmember
        // cout << "Made it to BMM 1st else" << endl;
        StringSet cast;
        cast.insert(actor_name);
        movieMap[wholeMovie] = cast;    // same as movieMap.insert(<wholeMovie, cast>) 
    }
    
    // Adding ActorNodes to theMap
    if (actor_name == "" || actor_name != currentActor) {   // new actor: make a node for them
        // cout << "New actor -> creating node and adding movie " << wholeMovie << endl;
        ActorNode* newActor = new ActorNode(actor_name);
        // cout << "Node created." << endl;
        newActor->addMovie(wholeMovie);
        EdgeSet emptyEdges;                             // empty set for now
        theMap.insert(Pear(newActor, emptyEdges));      // store actornode* into theMap
        addrMap.insert(Kiwi(actor_name, newActor));     // store actor name and its node* for use later
        currentActor = actor_name;
        // cout << "Reached end of if" << endl;
    } else {                                                // same actor: add movie
        // cout << "Same actor -> adding movie " << wholeMovie << " only" << endl;
        ActorNode* nodePtr = findNode(actor_name);
        nodePtr->addMovie(wholeMovie);
        currentActor = actor_name;
    }
}


/** Builds theMap(actual graph) using movieMap and calls initWeights to set edges */
void ActorGraph::buildTheMap(bool use_weighted_edges, bool frmP)
{   // cout << "Begin buildTheMap BTM" << endl;

    // Iterate through theMap
    for (UmapNodes::iterator it = theMap.begin(); it != theMap.end(); ++it) {
        // cout << "Starting theMap iterations" << endl;
        ActorNode* currentActor = it->first;
        EdgeSet currCons = it->second;
        vector<string> theirMovies = currentActor->getMovies();
        // cout << "Current actor name: " << currentActor->getName() << endl; 

        // Iterate thu all movies this actor is in
        for (unsigned int i = 0; i < theirMovies.size(); i++) {
            string mov = theirMovies.at(i);
            //cout << "CURRENT MOVIE ISSSS " << mov << endl;
            UmapStrings::iterator it2 = movieMap.find(mov); // find curr movie in movieMap
            StringSet cast = it2->second; // cast names of mov

            // Iterate thru cast
            for(StringSet::iterator it100 = cast.begin(); it100 != cast.end(); ++it100) {
                currCons = it->second;
                string currentCastMate = *it100;
                //cout << "CURRENT CASTMATE ISSSS " << currentCastMate << endl;
                
                if (currentCastMate == currentActor->getName())
                    continue; // if him or herself
                
                // check if currentActor alrdy has a currentCastMate conn. in edgeset    
                ActorEdge decoyEdge = ActorEdge(currentCastMate, nullptr, nullptr);
                EdgeSet::iterator it3 = currCons.find(decoyEdge);
                if (it3!= currCons.end()) { // found existing, add to shared movies
                    //cout << "found existing, add to shared" << endl;
                    ActorEdge existingEdge = *it3;
                    existingEdge.addSharedMovie(mov);
                    
                    //vector<string>* movInCom = existingEdge.getSharedMovies();
                    //(*it3).getSharedMovies() = movInCom;
                }
                else { // dne, make new edge with shared movie and add to set
                    //cout << "make new edge with shared movie and add to set" << endl;
                    int* yes = new int;
                    *yes = 1;
                    vector<string>* no = new vector<string>(0);
                    ActorEdge newEdge = ActorEdge(currentCastMate, yes, no);
                    //cout << "YES ADDRESS IS: " << yes << endl;
                    //cout << "NO ADDRESS IS: " << no << endl;
                    EdgeSet newSet = currCons;
                    //cout << "NEWSET ADDRESS IS: " << &newSet << endl;
                    newEdge.addSharedMovie(mov);
                    //cout << "HREEER" << endl;
                    newSet.insert(newEdge);  // TODO
                    //cout << "HI" << endl;
                    it->second = newSet;
                    //cout << "IT->2ND ADDRESS IS: " << &(it->second) << endl;
                }

            }

        }
        if (use_weighted_edges) 
            initWeight(currentActor->getName(), frmP);
    }

}

/** COMPARATOR class to make a min heap and sort nodes */
class myComp
{
public:
    int operator() (pair<int,ActorNode*> p1, pair<int,ActorNode*> p2) {
      if (p1.first != p2.first)
        return p1.first > p2.first;
      else
        return p1.second->getName() > p2.second->getName();
    }
};
class myComp2 
{
public:
    int operator() (string s1, string s2) {
        int len1 = s1.size();
        int len2 = s2.size();
        string year1 = s1.substr(len1-4,len1-1);
        string year2 = s2.substr(len2-4,len2-1);
        int y1 = stoi(year1);
        int y2 = stoi(year2);

        if (y1 != y2)
            return y1 < y2;
        else
            return s1 < s2;
    }
};

class myCompA // for organizing priorityq in actorconnections
{
public:
    int operator() (pair<int,ActorNode*> p1, pair<int,ActorNode*> p2) {
      if (p1.first != p2.first)
        return p1.first < p2.first;
      else
        return p1.second->getName() < p2.second->getName();
    }
};


class myComp2A // for organizing priority q in initWeights
{
public:
    int operator() (string s1, string s2) {
        int len1 = s1.size();
        int len2 = s2.size();
        string year1 = s1.substr(len1-4,len1-1);
        string year2 = s2.substr(len2-4,len2-1);
        int y1 = stoi(year1);
        int y2 = stoi(year2);

        if (y1 != y2)
            return y1 < y2;
        else
            return s1 < s2;
    }
};




/** InitWeight initializes all the weights of the graph based on earliest movie */
void ActorGraph::initWeight(string actor_name, bool frmP) {
    //cout << "entering initWeight" << endl;
    ActorNode* nodePtr = findNode(actor_name);
    UmapNodes::iterator it = theMap.find(nodePtr);
    EdgeSet eggs = it->second;
    for (EdgeSet::iterator it = eggs.begin(); it != eggs.end(); ++it) {
        //cout << "entering init for loop" << endl;
        ActorEdge ed =  *it;
        //cout << "EDGE ADDRESS " << &ed << endl;
        string newest = "";
        if (frmP) {
            priority_queue<string, vector<string>, myComp2> pq;
            vector<string>* shared = ed.getSharedMovies();
            //cout << "SHARED SIZE: " << shared->size() << endl;
            for (unsigned int i = 0; i < shared->size(); i++) {
                pq.push(shared->at(i));
            }
            //cout << "leaving init 2nd for loop" << endl;
            newest = pq.top();
            string ogFirst = shared->at(0);
            shared->push_back(ogFirst);
            shared->at(0) = newest;         // swapping so that newest mov is at pos 0
        }
        else {
            priority_queue<string, vector<string>, myComp2A> pq;
            vector<string>* shared = ed.getSharedMovies();
            //cout << "SHARED SIZE: " << shared->size() << endl;
            for (unsigned int i = 0; i < shared->size(); i++) {
                pq.push(shared->at(i));
            }
            //cout << "leaving init 2nd for loop" << endl;
            newest = pq.top();
            string ogFirst = shared->at(0);
            shared->push_back(ogFirst);
            shared->at(0) = newest;         // swapping so that newest mov is at pos 0
        }

        //cout << "NEWEST SHARED MOVIE:  " << newest << endl;
        int len = newest.size();
        string yr = newest.substr(len-4,len-1);
        int year = stoi(yr);
        //(*it).getWait() = 1 + (2015-year);
        int wait = 1 + (2015 - year);
        //cout << "YUHHHHHHHHHHH" << ((*it).weight) << endl;
        *((*it).weight) = wait;
    }
    //cout << "leaving init 1st for loop" << endl;
}



/** PathfinderFNC to find the shortest path b/n two actors */
vector<string> ActorGraph::pathFinder(string actor1, string actor2) {
    // empty priority queue
    //cout << "got to pathfinder" << endl;
    priority_queue<Peach,vector<Peach>,myComp> pq;
    ActorNode* actor1ptr = findNode(actor1);
    actor1ptr->pred = actor1ptr;
    actor1ptr->distance = 0;
    pq.push(Peach(0,actor1ptr));            // push first actor to the queue
    
    //cout << "about to go into pathfinder while loop" << endl;
    // DIJKSTRA
    while(!pq.empty()) {
        //cout << "got into pathfinder while loop" << endl;
        Peach popped = pq.top();
        pq.pop();
            
        ActorNode* u = popped.second;
        EdgeSet edges = getEdges(u->getName());
        for (EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it) {
            //cout << "got into pathfinder for loop" << endl;
            // v is the current neighbor (for each neighbor...)
            ActorNode* v = findNode((*it).getPartner());
            //cout << v->getName() << "'s distance is: " << v->distance << endl;
            //cout << u->getName() << "'s distance is: " << u->distance << endl;
            //cout << "WEIGHT: " << *((*it).weight) << endl;
            if ((v->distance) > (u->distance + *((*it).weight))) {
                v->distance = u->distance + *((*it).weight);
                v->pred = u;
                //cout << v->getName() << "'s pred is: " << u->getName() << endl;
                v->moviePath = (*it);
                pq.push(Peach(v->distance, v));

            }
        }
    }
    //cout << "got out of pathfinder while loop" << endl;

    // PATH WAS FOUND -> find path by going backwards and push to vector
    string currentName = actor2;
    vector<string> path; 
    path.push_back(actor2);
    
    while (currentName != actor1) {
        //cout << "got into 2nd while loop of PF" << endl;
        ActorNode* realNode = findNode(currentName);
        ActorEdge wayToPred = realNode->moviePath;
        //cout << "wayToPred partner: " << wayToPred.getPartner() << endl;
        string temp = (wayToPred.getSharedMovies())->at(0);
        //cout << "didnt get here" << endl;
        path.push_back(temp); // pushes first shared movie
        ActorNode* predNode = realNode->pred;
        path.push_back(predNode->getName());                 // pushes name
        currentName = predNode->getName();
    }
    
    //cout << "got out of 2nd PF while" << endl;
    reverse(path.begin(),path.end());   // reverse path


    for(unsigned int x = 1; x < path.size(); x = x + 2) {   
        //cout << "got into 2nd PF for loop" << endl;
        string needToSplit = path.at(x);
        //cout << "~~~~~need2split: " << needToSplit << endl;
        int len = needToSplit.length();
        string title = needToSplit.substr(0, len-4);
        string year = needToSplit.substr(len-4,len-1);
        string finalthing = title + "#@" + year;
        path.at(x) = finalthing;
    }

    //cout << "returning from PF " << endl;

    return path;
}

// Returns the first year they were connected using graph structure
int ActorGraph::actorConnectGraph(string actor1, string actor2) {

    // empty priority queue
    //cout << "got to pathfinder" << endl;
    priority_queue<Peach,vector<Peach>,myComp> pq;
    ActorNode* actor1ptr = findNode(actor1);
    actor1ptr->pred = actor1ptr;
    actor1ptr->distance = 0;
    pq.push(Peach(0,actor1ptr));            // push first actor to the queue
    
    //cout << "about to go into pathfinder while loop" << endl;
    // DIJKSTRA
    while(!pq.empty()) {
        //cout << "got into pathfinder while loop" << endl;
        Peach popped = pq.top();
        pq.pop();
            
        ActorNode* u = popped.second;
        EdgeSet edges = getEdges(u->getName());
        for (EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it) {
            cout << "got into AC for loop" << endl;
            // v is the current neighbor (for each neighbor...)
            ActorNode* v = findNode((*it).getPartner());
            //cout << v->getName() << "'s distance is: " << v->distance << endl;
            //cout << u->getName() << "'s distance is: " << u->distance << endl;
            cout << "WEIGHT: " << *((*it).weight) << endl;
            int c = min(u->distance, *((*it).weight));
            cout << "jfhsjkdh" << endl;
            if (c < (v->distance)) {
                cout << "got into if" << endl;
                v->distance = c;
                v->pred = u;
                cout << v->getName() << "'s pred is: " << u->getName() << endl;
                v->moviePath = (*it);
                pq.push(Peach(v->distance, v));

            }
            cout << "going to else in for" << endl;
        }
    }
    cout << "got out of pathfinder while loop" << endl;

    // PATH WAS FOUND -> find path by going backwards and push to vector
    string currentName = actor2;
    vector<string> path; 
    path.push_back(actor2);
    
    while (currentName != actor1) { // 
        //cout << "got into 2nd while loop of PF" << endl;
        ActorNode* realNode = findNode(currentName);
        ActorEdge wayToPred = realNode->moviePath;
        //cout << "wayToPred partner: " << wayToPred.getPartner() << endl;
        string temp = (wayToPred.getSharedMovies())->at(0);
        //cout << "didnt get here" << endl;
        path.push_back(temp); // pushes first shared movie
        ActorNode* predNode = realNode->pred;
        path.push_back(predNode->getName());                 // pushes name
        currentName = predNode->getName();
    }
    
    //cout << "got out of 2nd PF while" << endl;
    reverse(path.begin(),path.end());   // reverse path

    // adding #@ in between movie entries
    // iterate through movie entries and find lowest year
    int lowestYr = 10000;
    for(unsigned int x = 1; x < path.size(); x = x + 2) {   
        //cout << "got into 2nd PF for loop" << endl;
        string needToSplit = path.at(x);
        //cout << "~~~~~need2split: " << needToSplit << endl;
        int len = needToSplit.length();
        string year = needToSplit.substr(len-4,len-1);
        int stoiyr = stoi(year);
    
        if (stoiyr < lowestYr)
            lowestYr = stoiyr;
    
    }

    //cout << "returning from PF " << endl;

    return lowestYr;
}










// Returns the first year that they were connected using union find
int ActorGraph::actorConnectUF(string actor1, string actor2) {



    return -1;
}





