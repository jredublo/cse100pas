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



bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    cout << "GOT TO LOADFROMFILE" << endl;
    
    // Initialize the file stream
    ifstream infile(in_filename);
    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
        cout << "~~~~~~~~~~while starting " << endl;
        
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
        cout << "while ending~~~~~~~~~~~~~ " << endl;
    }
    

    
    /* Building the actual graph from movieMap */
    buildTheMap();
    
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
    cout << "IN BMM. ACTORNAME IS " << actor_name << " WHOLE MOVIE IS " << wholeMovie << endl;
    
    UmapStrings::iterator it = movieMap.find(wholeMovie);
    // Building movieMap
    if (it != movieMap.end()) {                 // movie exists: add actor to cast
        cout << "made it thru 1st if of buildmoviemap" << endl;
        StringSet existingCast = it->second;
        existingCast.insert(actor_name);
        it->second = existingCast;
    } 
    else {                                      // movie DNE: add movie and castmember
        cout << "made it thru 1st else of buildmoviemap" << endl;
        StringSet cast;
        cast.insert(actor_name);
        movieMap[wholeMovie] = cast;    // same as movieMap.insert(<wholeMovie, cast>) 
    }
    

    /* Add ActorNodes to theMap for later*/
    if (actor_name == "" || actor_name != currentActor) {   // actor DNE: new node
        cout << "actor dne, addng movie " << wholeMovie << endl;
        ActorNode* newActor = new ActorNode(actor_name);
        cout << "made node" << endl;
        newActor->addMovie(wholeMovie);
        EdgeSet emptyEdges;     // empty set for now
        theMap.insert(Pear(newActor, emptyEdges));
        addrMap.insert(Kiwi(actor_name, newActor));
        currentActor = actor_name;
        cout << "end of if statement" << endl;
    } 
    else {                            // actor exists: add movie to its vector
        cout << "actor exists, adding movie only " << wholeMovie << endl;
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
            cout << actor_name << "   movie: " << vek.at(i) << endl;
        }

        currentActor = actor_name;
    }

}


/** FNC: Builds theMap(actual graph) from movieMap */
void ActorGraph::buildTheMap() {
    cout << "GOT TO BUILDTHEMAP" << endl;


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
                ActorEdge decoyEdge = ActorEdge(currentCastMate);
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
                    ActorEdge newEdge = ActorEdge(currentCastMate);
                    EdgeSet newSet = currCons;
                    newEdge.addSharedMovie(mov);
                    newSet.insert(newEdge);
                    it->second = newSet;
                }

            }


        }


    }

}



/** COMPARATOR class to make a min heap and sort nodes */
class myComp
{
public:
    int operator() (ActorNode p1, ActorNode p2) {
      if (p1.distance != p2.distance)
        return p1.distance > p2.distance;
      else
        return p1.getName() > p2.getName();
    }
};



// FNC to find the shortest path b/n two actors
vector<string> ActorGraph::pathFinder(string actor1, string actor2) {
    priority_queue<string> q;  // TODO make it into djikstra TODO unvisited priority queue
    
    q.push(actor1);     // push first actor to the queue
   
    bool firstIter = true;

    int visitedLastTimes = 0;

    while (!q.empty()) {
        // pop first and then get children (set distance and pred) then push to q
        string currentAct = q.front();
        q.pop();
       
        // find the first actor in our map, is our start vertex
        MapAddr::iterator addrIt = addrMap.find(currentAct);
        ActorNode* decoy1 = addrIt->second;
        UmapNodes::iterator mapIt = theMap.find(decoy1);

        cout << "~~~Popping!"<< endl;
        cout << "ActorName in Pathfinder: " << currentAct << endl;
        cout << "distance if any: " << mapIt->first->distance << endl;
        
        
        if (firstIter == true) { // first iteration, set dist to 0
            
            ActorNode* firstIterNode = mapIt->first;
            EdgeSet edges = mapIt->second;
            firstIterNode->distance = 0;
            for(EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it) {
                // push first node's children, after making their pred the first node
                ActorEdge kid = *it;
                string kidName = kid.getPartner();
                MapAddr::iterator aIt = addrMap.find(kidName);
                ActorNode* kidNode = aIt->second;
                kidNode->distance = 1;
                kidNode->pred = firstIterNode;
                kidNode->moviePath = kid;
                q.push(kidName);
            }

            firstIter = false;
        }
        // normal iterations
        else {
        
            ActorNode* theNode = mapIt->first;
            EdgeSet nodeE = mapIt->second;
            for (EdgeSet::iterator itSet = nodeE.begin(); itSet != nodeE.end(); ++itSet) {
                // iterating through all edges (adj nodes)
                ActorEdge shid = *itSet;
                string shidName = shid.getPartner();
                MapAddr::iterator bIt = addrMap.find(shidName);
                ActorNode* shidNode = bIt->second;
                // if not predecessor
                if (shidNode != theNode->pred) {
                    int oldDist = shidNode->distance;
                    int ourPathDist = (theNode->distance) + 1;
                    if (ourPathDist < oldDist) { // we found a shorter way to here
                        shidNode->distance = ourPathDist;
                        shidNode->pred = theNode;
                        shidNode->moviePath = shid;
                    }
                    q.push(shidName);
                    
                }
            }
                
        }

    }
    
    string currentName = actor2;
    vector<string> path; 
    path.push_back(actor2);
    
    while (currentName != actor1) {
        MapAddr::iterator adIt = addrMap.find(currentName);
        ActorNode* decoyZ = adIt->second;

        UmapNodes::iterator iter = theMap.find(decoyZ); // it points to the real thing
        ActorNode* realNode = iter->first;
        ActorEdge wayToPred = realNode->moviePath;
        string temp = (wayToPred.getSharedMovies()).at(0);
        path.push_back(temp); // pushes first shared movie
        ActorNode* predNode = realNode->pred;
        path.push_back(predNode->getName());                 // pushes name
        currentName = predNode->getName();
    }
    
    reverse(path.begin(),path.end());   // reverse path
    
    for(unsigned int x = 1; x < path.size(); x = x + 2) {   
        string needToSplit = path.at(x);
        int len = needToSplit.length();
        string title = needToSplit.substr(0, len-5);
        string year = needToSplit.substr(len-4,len-1);
        string finalthing = title + "#@" + year;
        path.at(x) = finalthing;
    }

    

    return path;

}


/*EdgeSet ActorGraph::getEdges(string actorName) {
    ActorNode decoy1 = ActorNode(actorName);
    UmapNodes::iterator it = theMap.find(decoy1);
    return it->second;
}*/


