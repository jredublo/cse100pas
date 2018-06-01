/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
//#include <unordered_set>
#include <set>
//#include <unordered_map>
#include <map>
#include <vector>
#include "ActorGraph.h"

using namespace std;
typedef set<string> StringSet;                // uset of strings
typedef map<string, StringSet> UmapStrings;   // umap of (strings, StringSet) for movieMap
typedef set<ActorEdge> EdgeSet;               // uset of ActorEdges
typedef map<ActorNode, EdgeSet> UmapNodes;    // umap of (ActorNodes, EdgeSet) for final graph


bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);
    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
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

    }
    
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;

    
    /* Building the actual graph from movieMap */
    buildTheMap();
    
}



/** FNC: Builds movieMap(list of movies) line by line from input 
 *       and adds ActorNodes to theMap at the same time
 */
void ActorGraph::buildMovieMap(string actor_name, string wholeMovie) {
    UmapStrings::iterator it = movieMap.find(wholeMovie);
    // Building movieMap
    if (it != movieMap.end()) {                 // movie exists: add actor to cast
        StringSet existingCast = it->second;
        existingCast.insert(actor_name);
    } 
    else {                                      // movie DNE: add movie and castmember
        StringSet cast;
        cast.insert(actor_name);
        movieMap[wholeMovie] = cast; 
    }
    
    /* Add ActorNodes to theMap for later*/
    if (actor_name == "" || actor_name != currentActor) {   // actor DNE: new node
        ActorNode newActor = ActorNode(actor_name);
        newActor.addMovie(wholeMovie);
        EdgeSet emptyEdges; // empty set for now
        theMap[newActor] = emptyEdges;
    } 
    else {                                                  // actor exists: add movie to its vector
        UmapNodes::iterator itt;
        ActorNode decoy = ActorNode(actor_name);
        itt = theMap.find(decoy);               //TODO og was just actor_name ;; 
        ActorNode existingActor = itt->first;   // won't work, param needs to be the actorNode, not name TODO
        existingActor.addMovie(wholeMovie);
    }

}


/** FNC: Builds theMap(actual graph) from movieMap */
void ActorGraph::buildTheMap() {
    
    // we have theMap and all the actor nodes already in it
    // we need to add the actor edges to their sets
    // 1) iterate through theMap through all nodes      FOR LOOP 
    //     - get the nodes movie list                   
    //     - get the movie in movieMap and get its cast
    //     - make ActorEdges from cast
    //     - add Actor Edges to the set of edges of theMap at corresponding actornode **

    // Iterate through theMap
    for (UmapNodes::iterator it = theMap.begin(); it != theMap.end(); ++it) {
        ActorNode currentActor = it->first;
        EdgeSet currentActorsConnections = it->second;
        vector<string> theirMovies = currentActor.getMovies();
        
        // Iterate through all the movies that this actor starred in
        for (unsigned int i = 0; i < theirMovies.size(); i++) {
            string mov = theirMovies.at(i);
            UmapStrings::iterator it2 = movieMap.find(mov);
            string nowMovie = it2->first;
            StringSet cast = it2->second;

            // Iterate through the cast and create actorEdges for each cast mate
            for(StringSet::iterator it100 = cast.begin(); it100 != cast.end(); ++it100) {
                string currentCastMate = *it100;
                
                if (currentCastMate != currentActor.getName()) {  // not him/herself
                    // making sure we havent already made a connection for this one: if so, add to Shared
                    ActorNode decoy = ActorNode(currentCastMate);
                    EdgeSet::iterator it3 = currentActorsConnections.find(decoy);
                    if (it3 != it->second.end()) {      // found, add to Shared TODO
                        ActorEdge ourEdge = *it3;
                        ourEdge.addSharedMovie(mov);
                    }
                    else {                              // not found, create edge and add to shared
                        ActorNode coActor = ActorNode(currentCastMate);
                        ActorEdge connection = ActorEdge(coActor);
                        connection.addSharedMovie(mov);
                        currentActorsConnections.insert(connection); // add to set of edges
                    }
                }

            }

        }


    }

}








