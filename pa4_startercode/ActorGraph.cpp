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
#include <set>
#include <vector>
#include "ActorGraph.h"

using namespace std;

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

        /* Building movieMap */
        map<string, set<string>>::iterator it = movieMap.find(wholeMovie);
        //movie exists
        if (it != movieMap.end()) {
            it->second.insert(actor_name);
        }
        //movie does not exist
        else {
            set<string> cast;
            cast.insert(actor_name);
            movieMap[wholeMovie] = cast; 
        }
       
        /* Add actor nodes to theMap for later*/
        if (actor_name == "" || actor_name != currentActor) { // new actor: new node
            ActorNode newActor = ActorNode(actor_name);
            newActor.addMovie(wholeMovie);
            theMap[newActor] = set<ActorEdge>(); // initialize coactors set to null for now
        }
        else {      // actor already made: add movie to its vector
            map<ActorNode, set<ActorNode>>::iterator itt;
            itt = theMap.find(actor_name);
            itt->first.addMovie(wholeMovie);
        }

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




// Builds theMap(actual graph) from movieMap
void buildTheMap() {
    
    // we have theMap and all the actor nodes already in it
    // we need to add the actor edges to their sets
    // 1) iterate through theMap through all nodes      FOR LOOP 
    //     - get the nodes movie list                   
    //     - get the movie in movieMap and get its cast
    //     - make ActorEdges from cast
    //     - add Actor Edges to the set of edges of theMap at corresponding actornode **

    // Iterate through theMap
    for (map<ActorNode,set<ActorNode>>::iterator it=theMap.begin(); 
            it!=theMap.end(); ++it) {
        // it->first is actor node   it->second is that actorNode's set of coactors
        vector<string> theirMovies = it->first.getMovies();
        
        // get all the movies that this actor starred in
        for (unsigned int i = 0; i < theirMovies.size(); i++) {
            string mov = theirMovies.at(i);
            map<string,set<string>>::iterator it2 = movieMap.find(mov);
            // it2->first is movie   it2->second is cast
            set<string> cast = it2->second;
            set<string>::iterator it100;

            // make actorEdges for each cast mate
            for(it100=cast.begin(); it100 != cast.end(); ++it100) {
                string currentCastMate = *it100;
                // if its not him/herself 
                if (currentCastMate != it->first.getName()) {
                    set<ActorEdge>::iterator it3 = it->second.find(currentCastMate);
                    if (it3 != it->second.end()) {
                        // found already exists: add to shared Movies //TODO
                        (*it3).addSharedMovies(mov);
                    }
                    else {
                        // does not exist yet: create edge and add to shared movies
                        ActorNode coActor = ActorNode(currentCastMate);
                        ActorEdge connection = ActorEdge(coActor);
                        connection.addSharedMovie(mov);
                    }

                }
            }
        }
    }

}








