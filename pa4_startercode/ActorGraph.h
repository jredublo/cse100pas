
/* FILE:    ActorGraph.h
 * AUTHOR:  Madalynn Norton, Jessica Redublo
 * DATE:    June 8, 2018
 * DESCR:   Holds the function declarations for ActorGraph that
 *              are essential for making pathfinder and widestp
 *              actorconnections.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "ActorNode.hpp"

using namespace std;

typedef set<string> StringSet;              // set of strings
typedef map<string, StringSet> UmapStrings; // map (strings, StringSet)       <movieMap>
typedef set<ActorEdge> EdgeSet;             // set of actorEdges
typedef map<ActorNode*, EdgeSet> UmapNodes; // map (ActorNodes, EdgeSet)      <theMap>
typedef map<string, ActorNode*> MapAddr;    // map (actor_name, node address) <addrMap>


    /* ---------------------------- START CLASS ---------------------------- */


class ActorGraph {
private:
    // Members
    UmapNodes theMap;           // our undirected graph of actor nodes and their edges
    UmapStrings movieMap;       // list of strings for movies and their casts 
    string currentActor;        // used to know if we have moved on to next actor
    MapAddr addrMap;            // map for holding the actor name and its node's address


public:
    /** Constructor */
    ActorGraph() {
        currentActor = "";
    }
    
    /** Destructor */
    /*~ActorGraph() {
        UmapNodes::iterator it;
        for (it = theMap.begin(); it != theMap.end(); ++it) {
            delete it->first;
            EdgeSet::iterator iter;
            for (iter = (it->second).begin(); iter != (it->second).end(); ++iter) {
                delete &(*iter);
            }
        }
    }*/

    /** Builds movieMap by line from in file and adds actorNodes to theMap */
    void buildMovieMap(string actor_name, string wholeMovie, bool frmP);
    
    /** Builds theMap, the final undirected graph of ActorNodes */
    void buildTheMap(bool use_weighted_edges, bool frmP);

    /** Getter for the map */
    UmapNodes getTheMap() {
        return theMap;
    }
   
    /** Getter for the movieMap */
    UmapStrings getMovieMap() {
        return movieMap;
    }

    /** Getter for the addrMap */
    MapAddr getAddrMap() {
        return addrMap;
    }

    /** Gets the address of the actor node for the given actor name */
    ActorNode* findNode( string actor_name ) {
        MapAddr::iterator it = addrMap.find(actor_name);
        return it->second;
    }

    /** Gets edges of actor node for given actor name */
    EdgeSet getEdges( string actor_name ) {
        ActorNode* nodePtr = findNode(actor_name);
        UmapNodes::iterator it = theMap.find(nodePtr);
        return it->second;
    }

    /** Loads graph from tab-delimited file of actor->movie relationships, (un)weighted */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges, bool frmP);

    /** Finds the shortest path from actor1 and actor2 */
    vector<string> pathFinder(string actor1, string actor2);

    /** Finds the widest path from actor1 and actor2 using the graph */
    int actorConnectGraph(string actor1, string actor2);
    
    /** Finds the widest path from actor1 and actor2 using sets */
    //int actorConnectUF(string actor1, string actor2);

    /** Initializes all the weights of the edges. Assumes theMap is set up already */
    void initWeight(string actor_name, bool frmP);

};

#endif // ACTORGRAPH_H
