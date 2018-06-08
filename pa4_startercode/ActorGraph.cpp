
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
    //int p = 0;
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
        buildMovieMap(actor_name, wholeMovie, frmP);          // Build movieMap 
        //cout << "Calling movieMap " << p << endl;
        //p++;
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
void ActorGraph::buildMovieMap(string actor_name, string wholeMovie, bool frmP) 
// TODO need bool here
{   // cout << "Begin buildMovieMap. ActorName: " << actor_name << "  WholeMov: " << wholeMovie << endl;
    
    // Building MovieMap
    UmapStrings::iterator it = movieMap.find(wholeMovie);
    if (it != movieMap.end()) {                         // movie exists: add actor to cast
        // cout << "Made it to BMM 1st if" << endl;
        StringSet existingCast = it->second;
        existingCast.insert(actor_name);
        it->second = existingCast;
    } else {                                   // movie DNE: add movie and castmember
        // cout << "Made it to BMM 1st else" << endl;
        StringSet cast;
        cast.insert(actor_name);
        movieMap[wholeMovie] = cast;                    // movieMap.insrt(<mov, cast>) 
    }
    
    // Adding ActorNodes to theMap
    if (actor_name == "" || actor_name != currentActor) {   // new actor: make a node
        // cout << "New actor -> creating node and adding movie " << wholeMovie << endl;
        ActorNode* newActor = new ActorNode(actor_name, frmP);
        // cout << "Node created." << endl;
        newActor->addMovie(wholeMovie);
        EdgeSet emptyEdges;                                 // empty set for now
        theMap.insert(Pear(newActor, emptyEdges));          // store actornode* into theMap
        addrMap.insert(Kiwi(actor_name, newActor));         // store actor name and its node* for use later
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

    // Iterate through theMap (TODO alpha order, stop if currEj < than currAct)
    for (UmapNodes::iterator it = theMap.begin(); it != theMap.end(); ++it) {
        // cout << "Starting theMap iterations" << endl;
        ActorNode* currentActor = it->first;
        EdgeSet currCons = it->second;
        vector<string> theirMovies = currentActor->getMovies();
        // cout << "Current actor name: " << currentActor->getName() << endl; 

        // Iterate thu all movies this actor is in
        for (unsigned int i = 0; i < theirMovies.size(); i++) {
            string mov = theirMovies.at(i);
            // cout << "  Current movie: " << mov << endl;
            UmapStrings::iterator it2 = movieMap.find(mov); // find curr movie in movieMap
            StringSet cast = it2->second;                       // cast names of mov

            // Iterate thru cast
            for(StringSet::iterator it100 = cast.begin(); it100 != cast.end(); ++it100) {
                currCons = it->second;
                string currentCastMate = *it100;
                // cout << "    Current castmate: " << currentCastMate << endl;
                
                if (currentCastMate == currentActor->getName()) continue;// him or herself
                
                // Check currActor alrdy has an edge for this currentCastMate    
                ActorEdge decoyEdge = ActorEdge(currentCastMate, nullptr, nullptr);
                EdgeSet::iterator it3 = currCons.find(decoyEdge);
                if (it3!= currCons.end()) {
                    // cout << "Found existing edge, just add movie to shared" << endl;
                    ActorEdge existingEdge = *it3;
                    existingEdge.addSharedMovie(mov);
                } else {
                    // cout << "DNE, make new edge with shared movie" << endl;
                    int* yes = new int;
                    *yes = 1;                    // cout << "yes Addr: " << yes << endl;
                    vector<string>* no = new vector<string>(0); 
                    // cout << "no Addr: " << no << endl;
                    ActorEdge newEdge = ActorEdge(currentCastMate, yes, no);
                    EdgeSet newSet = currCons;          
                    // cout << "newSet Addr: " << &newSet << endl;
                    newEdge.addSharedMovie(mov);
                    // cout << "Here!" << endl;
                    newSet.insert(newEdge);             // TODO
                    //cout << "Here #2!" << endl;
                    it->second = newSet;                // cout << "it->2nd Addr: " << &(it->second) << endl;
                }
            }
        }
        
        if (use_weighted_edges) { initWeight(currentActor->getName(), frmP); }
    }
}



      /* --------------------------- COMPARATOR CLASSES --------------------------- */
/** Comparator class to sort pairs for priority queue for pathfinder */
class myComp  {
public:
    int operator() (pair<int,ActorNode*> p1, pair<int,ActorNode*> p2) {
      if (p1.first != p2.first)
        return p1.first > p2.first;
      else
        return p1.second->getName() > p2.second->getName();
    }
};
/** Comparator class to sort wholeMovie strings for priority queue for initWeights */
class myComp2  {
public:
    int operator() (string s1, string s2) {
        int len1 = s1.size();
        int len2 = s2.size();
        string year1 = s1.substr(len1-4,len1-1);
        string year2 = s2.substr(len2-4,len2-1);
        int y1 = stoi(year1);
        int y2 = stoi(year2);

        if (y1 != y2) { return y1 < y2; }
        else { return s1 < s2; }
    }
};
/** Comparator class to sort pairs for priority queue for actorconnections */
class myCompA  {
public:
    int operator() (pair<int,ActorNode*> p1, pair<int,ActorNode*> p2) {
      if (p1.first != p2.first) { return p1.first < p2.first; }
      else { return p1.second->getName() < p2.second->getName(); }
    }
};
/** Comparator class to sort wholeMovie strings for priorty queue for initWeights */
class myComp2A  {
public:
    int operator() (string s1, string s2) {
        int len1 = s1.size();
        int len2 = s2.size();
        string year1 = s1.substr(len1-4,len1-1);
        string year2 = s2.substr(len2-4,len2-1);
        int y1 = stoi(year1);
        int y2 = stoi(year2);

        if (y1 != y2) { return y1 < y2; }
        else { return s1 < s2; }
    }
};      /* --------------------------- CONTINUE FUNCTIONS --------------------------- */




/** Initializes all the weights of the graph based on earliest or newest movie */
void ActorGraph::initWeight(string actor_name, bool frmP) 
{   //cout << "Begin initWeight for actor: " << actor_name << endl;
    ActorNode* nodePtr = findNode(actor_name);
    UmapNodes::iterator it = theMap.find(nodePtr);
    EdgeSet eggs = it->second;
    for (EdgeSet::iterator it = eggs.begin(); it != eggs.end(); ++it) {
        // cout << "Entering initWeight for loop" << endl;
        ActorEdge ed =  *it;
        // cout << "Edge Address: " << &ed << endl;
        string newest = "";
        if (frmP) {                             // pathfinder --> newest movie for weight
            priority_queue<string, vector<string>, myComp2> pq;
            vector<string>* shared = ed.getSharedMovies();
            // cout << "Size of shared movies: " << shared->size() << endl;
            for (unsigned int i = 0; i < shared->size(); i++) { pq.push(shared->at(i)); }
            // cout << "Just left initWeight 2nd for loop" << endl;
            newest = pq.top();
            string ogFirst = shared->at(0);     // swapping so newest is at position 0
            shared->push_back(ogFirst);
            shared->at(0) = newest;
        } else {                                // actcon --> oldest movie for weight
            priority_queue<string, vector<string>, myComp2A> pq;
            vector<string>* shared = ed.getSharedMovies();
            // cout << "Size of shared movies: " << shared->size() < endl;
            for (unsigned int i = 0; i < shared->size(); i++) { pq.push(shared->at(i)); }
            //cout << "leaving init 2nd for loop" << endl;
            newest = pq.top();
            string ogFirst = shared->at(0);     // swapping so oldest is at position 0
            shared->push_back(ogFirst);
            shared->at(0) = newest;
        }
        // cout << "Newest(Pf)/Oldest(Ac) movie: " << newest << endl;
        int len = newest.size();
        string yr = newest.substr(len-4,len-1);
        int year = stoi(yr);
        int wait = 1 + (2015 - year);
        // cout << "Weight stored in this edge: " << ((*it).weight) << endl;
        *((*it).weight) = wait;
    }
    // cout << "Left initWeights for loop" << endl;
}



/** Finds the shortest path b/n two actors using the graph */
vector<string> ActorGraph::pathFinder(string actor1, string actor2) {
    // cout << "Begin pathfinder()" << endl;
    priority_queue<Peach,vector<Peach>,myComp> pq;  // empty priority q
    ActorNode* actor1ptr = findNode(actor1);
    actor1ptr->pred = actor1ptr;
    actor1ptr->distance = 0;
    pq.push(Peach(0,actor1ptr));                    // push 1st actor to the queue
    
    // Begin DIJKSTRA loop until priority queue is empty
    while(!pq.empty()) {
        // cout << "Beginning pathfinder while loop" << endl;
        Peach popped = pq.top();
        pq.pop();
            
        ActorNode* u = popped.second;
        EdgeSet edges = getEdges( u->getName() );
        // Get all the adjacent nodes
        for (EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it) {
            // cout << "Reached athfinder for loop" << endl;
            // v is the current neighbor (for each neighbor...)
            ActorNode* v = findNode((*it).getPartner());
            // cout << v->getName() << "'s distance is: " << v->distance << endl;
            // cout << u->getName() << "'s distance is: " << u->distance << endl;
            // cout << "Weight between them two: " << *((*it).weight) << endl;
            if ((v->distance) > (u->distance + *((*it).weight))) {
                v->distance = u->distance + *((*it).weight);
                v->pred = u;
                // cout << v->getName() << "'s pred is: " << u->getName() << endl;
                v->moviePath = (*it);
                pq.push(Peach(v->distance, v));
            }
        }
    }
    // cout << "Got out of pathfinder while loop" << endl;
    // PATH WAS FOUND -> find path by going backwards from actor2 and push to vector
    string currentName = actor2;
    vector<string> path; 
    path.push_back(actor2);
    
    while (currentName != actor1) {                 // stop if we have 1st actor
        // cout << "Reached 2nd while loop of PF" << endl;
        ActorNode* realNode = findNode(currentName);
        ActorEdge wayToPred = realNode->moviePath;
        // cout << "wayToPred partner: " << wayToPred.getPartner() << endl;
        string temp = (wayToPred.getSharedMovies())->at(0);
        // cout << "Got here?" << endl;
        path.push_back(temp);                       // push first shared movie
        ActorNode* predNode = realNode->pred;
        path.push_back(predNode->getName());        // push name
        currentName = predNode->getName();
    }
 
    // cout << "Got out of 2nd PF while" << endl;
    reverse(path.begin(),path.end());               // reverse path

    for(unsigned int x = 1; x < path.size(); x = x + 2) {   
        //cout << "Got into 2nd PF for loop to split up wholeMovie " << path.at(x) << endl;
        string needToSplit = path.at(x);
        int len = needToSplit.length();
        string title = needToSplit.substr(0, len-4);
        string year = needToSplit.substr(len-4,len-1);
        string finalthing = title + "#@" + year;
        path.at(x) = finalthing;
    }

    // cout << "About to return rom PF " << endl;
    return path;
}



/** Returns the first year two actors were connected using graph structure */
int ActorGraph::actorConnectGraph(string actor1, string actor2) 
{   // cout << "Got to actorConnections" << endl;
    priority_queue<Peach,vector<Peach>,myComp> pq;  // empty priority queue
    ActorNode* actor1ptr = findNode(actor1);
    actor1ptr->pred = actor1ptr;
    actor1ptr->distance = 1000000;
    pq.push(Peach(0,actor1ptr));                    // push first actor to the queue
    
    // DIJKSTRA
    while(!pq.empty()) {
        // cout << "Got into pathfinder while loop" << endl;
        Peach popped = pq.top();
        pq.pop();
            
        ActorNode* u = popped.second;
        EdgeSet edges = getEdges(u->getName());
        for (EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it) {
            //cout << "Got into AC for loop" << endl;
            // v is the current neighbor (for each neighbor...)
            ActorNode* v = findNode((*it).getPartner());
            //cout << v->getName() << "'s distance is: " << v->distance << endl;
            //cout << u->getName() << "'s distance is: " << u->distance << endl;
            //cout << "Weight between them: " << *((*it).weight) << endl;
            int c = min(u->distance, *((*it).weight));
            //cout << "Hello?" << endl;
            //cout << "C'S DISTANCE IS: " << c << endl;
            if (c > (v->distance)) {
                //cout << "Went inside if?" << endl;
                v->distance = c;
                v->pred = u;
                //cout << v->getName() << "'s pred is: " << u->getName() << endl;
                v->moviePath = (*it);
                pq.push(Peach(v->distance, v));
            }
            //else cout << "Going into else?" << endl;
        }
    }
    //cout << "Broke out of actorconnections while loop" << endl;

    // PATH WAS FOUND -> find path by going backwards and push to vector
    string currentName = actor2;
    vector<string> path; 
    path.push_back(actor2);
    
    while (currentName != actor1) { // 
        //cout << "Got into 2nd while loop of PF" << endl;
        ActorNode* realNode = findNode(currentName);
        ActorEdge wayToPred = realNode->moviePath;
        //cout << "wayToPred partner: " << wayToPred.getPartner() << endl;
        string temp = (wayToPred.getSharedMovies())->at(0);
        //cout << "Got here?" << endl;
        path.push_back(temp);                       // pushes first shared movie
        ActorNode* predNode = realNode->pred;
        path.push_back(predNode->getName());        // pushes name
        currentName = predNode->getName();
    }
    
    //cout << "Got out of 2nd AC while" << endl;
    reverse(path.begin(),path.end());               // reverse path
    //for (unsigned int t = 0; t < path.size(); t++)
        //cout << "Path vector:   " << path.at(t) << endl;

    // Iterate through movie entries and find lowest year
    int lowestYr = 0;
    for(unsigned int x = 1; x < path.size(); x = x + 2) {   
        //cout << "~~~Looking at wholeMovie: " << path.at(x) << endl;
        string needToSplit = path.at(x);
        int len = needToSplit.length();
        string year = needToSplit.substr(len-4,len-1);
        int stoiyr = stoi(year); 
        
        if (stoiyr > lowestYr)  { lowestYr = stoiyr; }
    }

    // cout << "About to return from AC!" << endl;

    return lowestYr;
}


