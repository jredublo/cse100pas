/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "ActorGraph.h"
#include <queue>


using namespace std;
typedef set<string> StringSet;               // uset of strings
typedef map<string, StringSet> UmapStrings;  // umap (strings, StringSet) for movieMap
typedef set<ActorEdge> EdgeSet;              // uset of ActorEdges
typedef map<ActorNode*, EdgeSet> UmapNodes;  // umap(ActorNode*, EdgeSet) for final graph
typedef map<string, ActorNode*> MapAddr; // map of (actor_name, node address) 
typedef pair<ActorNode*, EdgeSet> Pear;     // pairs to insert into theMAP
typedef pair<string, ActorNode*> Kiwi;      // pairs to insert into addrMap
typedef pair<int, ActorNode*> Peach;


bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    //cout << "GOT TO LOADFROMFILE" << endl;
    
    // Initialize the file stream
    ifstream infile(in_filename);
    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
        //cout << "~~~~~~~~~~while starting " << endl;
        
        string s;
        // get the next line
        if (!getline( infile, s )) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        istringstream ss( s );
        vector <string> record;
        while (ss) {
            string next;
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;
            record.push_back( next );
        }
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        string movie_year(record[2]);

        string wholeMovie = movie_title + movie_year; // merge year and title
        buildMovieMap(actor_name, wholeMovie);        // construct movieMap for easy graph making
        //cout << "while ending~~~~~~~~~~~~~ " << endl;
    }
    

    
    /* Building the actual graph from movieMap */
    buildTheMap(use_weighted_edges);
    
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;
}



/** FNC: Builds movieMap(list of movies) line by line from input 
 *       and adds ActorNodes to theMap at the same time
 */
void ActorGraph::buildMovieMap(string actor_name, string wholeMovie) {
   //cout << "IN BMM. ACTORNAME IS " << actor_name << " WHOLE MOVIE IS " << wholeMovie << endl;
    
    UmapStrings::iterator it = movieMap.find(wholeMovie);
    // Building movieMap
    if (it != movieMap.end()) {                 // movie exists: add actor to cast
        //cout << "made it thru 1st if of buildmoviemap" << endl;
        StringSet existingCast = it->second;
        existingCast.insert(actor_name);
        it->second = existingCast;
    } 
    else {                                      // movie DNE: add movie and castmember
        //cout << "made it thru 1st else of buildmoviemap" << endl;
        StringSet cast;
        cast.insert(actor_name);
        movieMap[wholeMovie] = cast;    // same as movieMap.insert(<wholeMovie, cast>) 
    }
    

    /* Add ActorNodes to theMap for later*/
    if (actor_name == "" || actor_name != currentActor) {   // actor DNE: new node
        //cout << "actor dne, addng movie " << wholeMovie << endl;
        ActorNode* newActor = new ActorNode(actor_name);
        //cout << "made node" << endl;
        newActor->addMovie(wholeMovie);
        EdgeSet emptyEdges;     // empty set for now
        theMap.insert(Pear(newActor, emptyEdges));
        addrMap.insert(Kiwi(actor_name, newActor));
        currentActor = actor_name;
        //cout << "end of if statement" << endl;
    } 
    else {                            // actor exists: add movie to its vector
        //cout << "actor exists, adding movie only " << wholeMovie << endl;
        // getting the address from addrMap using name
        MapAddr::iterator addrIt;
        addrIt = addrMap.find(actor_name);    // getting ptr to actornode
        ActorNode* decoy = addrIt->second;
        // getting the actual node in theMap and changing vector!:)
        UmapNodes::iterator itt;
        itt = theMap.find(decoy);               //TODO og was just actor_name ;;
        ActorNode* realNode = itt->first;        
        realNode->addMovie(wholeMovie);

        vector<string> vek = itt->first->getMovies();
        for (unsigned int i = 0; i < vek.size(); i ++) {
            cout << actor_name << " movie: " << vek.at(i) << endl;
        }

        currentActor = actor_name;
    }

}


/** FNC: Builds theMap(actual graph) from movieMap */
void ActorGraph::buildTheMap(bool use_weighted_edges) {
    //cout << "GOT TO BUILDTHEMAP" << endl;

    // Iterate through theMap
    for (UmapNodes::iterator it = theMap.begin(); it != theMap.end(); ++it) {
        //cout << "~~~starting iter" << endl;
        ActorNode* currentActor = it->first;
        EdgeSet currCons = it->second;
        vector<string> theirMovies = currentActor->getMovies();
        //cout << "CURRENT ACTOR NAME " << currentActor->getName() << endl; 

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
                ActorEdge decoyEdge = ActorEdge(currentCastMate, nullptr);
                EdgeSet::iterator it3 = currCons.find(decoyEdge);
                if (it3!= currCons.end()) { // found existing, add to shared movies
                    //cout << "found existing, add to shared" << endl;
                    ActorEdge existingEdge = *it3;
                    existingEdge.addSharedMovie(mov);
                    vector<string> movInCom = existingEdge.getSharedMovies();
                    (*it3).getSharedMovies() = movInCom;
                }
                else { // dne, make new edge with shared movie and add to set
                    //cout << "make new edge with shared movie and add to set" << endl;
                    int* yes = new int;
                    *yes = 1;
                    ActorEdge newEdge = ActorEdge(currentCastMate, yes);
                    EdgeSet newSet = currCons;
                    newEdge.addSharedMovie(mov);
                    newSet.insert(newEdge);
                    it->second = newSet;
                }

            }

        }
        if (use_weighted_edges) 
            initWeight(currentActor->getName());
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
            return y1 > y2;
        else
            return s1 > s2;
    }
};

//initializes all the weights of the graph based on earliest movie
void ActorGraph::initWeight(string actor_name) {
    cout << "entering initWeight" << endl;
    ActorNode* nodePtr = findNode(actor_name);
    UmapNodes::iterator it = theMap.find(nodePtr);
    EdgeSet eggs = it->second;
    for (EdgeSet::iterator it = eggs.begin(); it != eggs.end(); ++it) {
        cout << "entering init for loop" << endl;
        ActorEdge ed =  *it;
        priority_queue<string, vector<string>, myComp2> pq;
        vector<string> shared = ed.getSharedMovies();
        for (unsigned int i = 0; i < shared.size(); i++) {
            cout << "entering 2nd init for loop" << endl;
            pq.push(shared.at(i));
        }
        cout << "leaving init 2nd for loop" << endl;
        string newest = pq.top();
        int len = newest.size();
        string yr = newest.substr(len-4,len-1);
        int year = stoi(yr);
        //(*it).getWait() = 1 + (2015-year);
        int wait = 1 + (2015 - year);
        //cout << "YUHHHHHHHHHHH" << *((*it).weight) << endl;
        *((*it).weight) = wait;
    }
    cout << "leaving init 1st for loop" << endl;
}

// FNC to find the shortest path b/n two actors
vector<string> ActorGraph::pathFinder(string actor1, string actor2) {
    // empty priority queue
    cout << "got to pathfinder" << endl;
    priority_queue<Peach,vector<Peach>,myComp> pq;
    ActorNode* actor1ptr = findNode(actor1);
    actor1ptr->pred = actor1ptr;
    actor1ptr->distance = 0;
    pq.push(Peach(0,actor1ptr));            // push first actor to the queue
    
    //cout << "about to go into pathfinder while loop" << endl;
   
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
            cout << v->getName() << "'s distance is: " << v->distance << endl;
            cout << u->getName() << "'s distance is: " << u->distance << endl;
            cout << "WEIGHT: " << (*it).weight << endl;
            if ((v->distance) > (u->distance + *((*it).weight))) {
                v->distance = u->distance + *((*it).weight);
                v->pred = u;
                cout << v->getName() << "'s pred is: " << u->getName() << endl;
                v->moviePath = (*it);
                pq.push(Peach(v->distance, v));

            }
        }
    }

    //cout << "got out of pathfinder while loop" << endl;

    // getting path
    string currentName = actor2;
    vector<string> path; 
    path.push_back(actor2);
    
    while (currentName != actor1) {
        //cout << "got into 2nd while loop of PF" << endl;
        ActorNode* realNode = findNode(currentName);
        ActorEdge wayToPred = realNode->moviePath;
        cout << "wayToPred partner: " << wayToPred.getPartner() << endl;
        string temp = (wayToPred.getSharedMovies()).at(0);
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

