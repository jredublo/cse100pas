
/* FILE:    ActorGraph.h
 * Author:  Madalynn Norton, Jessica Redublo
 * Date:    
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <vector>
//#include <unordered_map>
#include <map>
//#include <unordered_set>
#include <set>
#include "ActorEdge.hpp"


using namespace std;
typedef set<string> StringSet;                // uset of strings
typedef map<string, StringSet> UmapStrings;   // umap of (strings, StringSet) for movie list  movieMap
typedef set<ActorEdge> EdgeSet;               // uset of actorEdges
typedef map<ActorNode, EdgeSet> UmapNodes;    // umap of (ActorNodes, EdgeSet) for graph      theMap


class ActorGraph {
private:
    // Members
    UmapNodes theMap;           // our undirected graph of actor nodes and their edges
    UmapStrings movieMap;       // list of strings for movies and their casts 
    string currentActor;        // used to know if we have moved on to next actor
        // int numActors, int numEdges

public:
    // Constructor
    ActorGraph(){
        currentActor = ""; // numActors = numEdges = 0;
    }

    // Builds movieMap line by line from input file and adds actorNodes to theMap
    void buildMovieMap(string actor_name, string wholeMovie);
    // Builds the final undirected graph of actor Nodes
    void buildTheMap();

    UmapNodes getTheMap() {
        return theMap;
    }
    

    /**
     * Load graph from tab-delimited file of actor->movie relationships.
     * PARAMS:
     *      in_filename: input filename
     *      use_weighted_edges: if true, edge weights are 1 + (2015 - movie_year), otherwise all edges are 1
     * RETURN: true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

};


#endif // ACTORGRAPH_H
