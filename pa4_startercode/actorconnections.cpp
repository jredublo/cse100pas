
/* FILE:        actorconnections.cpp
 * AUTHORS:     Madalynn Norton  Jessica Redublo
 * DUE DATE:    June 8, 2018
 * DESCR:       Holds main method for taking in user input 
 *                  and calling the appropriate actorconnections
 *                  depeneding on ufind or widestp.
 */

#include <vector>
#include <iostream>
#include "ActorGraph.h"
#include "USet.hpp"
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>

using namespace std;

typedef set<string> StringSet;                      // set of strings
typedef map<string, StringSet> UmapStrings;         // map (strings, StringSet) <movieMap>
typedef set<ActorEdge> EdgeSet;                     // set of ActorEdges
typedef map<ActorNode*, EdgeSet> UmapNodes;         // map (ActorNodes, EdgeSet) <theMap>


    /* ----------------------------- MAIN FUNCTION ----------------------------- */


int main(int argc, char** argv) 
{
    if (argc != 5) {
        cerr << "ERROR! Requires 4 arguments for actorconnections!" << endl;
        return -1;
    }

    string firstIn = argv[1];                   // all actors/movies
    string secondIn = argv[2];                  // names of actor pairs
    string thirdIn = argv[3];                   // output file
    string fourthIn = argv[4];                  // widestp or ufind

    ActorGraph* obj = new ActorGraph();  // TODO
    USet disjointSet;

    if (fourthIn == "widestp") {                // create graph with weighted edges
        obj->loadFromFile(argv[1], true, false);
    } else {                                    // using sets to get earliest year
        disjointSet.initialize(argv[1]);
    }

    // Open ofstream to outfile and add header
    ofstream out(thirdIn);
    cout << "Actor1\tActor2\tYear" << endl;
    out << "Actor1\tActor2\tYear" << endl;

    ifstream in3(secondIn);                     // read in pairs line by line
    bool have_header = false;
    while (in3) {
        // cout << "Here #1" << endl;
        string s;                               // get next line
        if (!getline( in3, s)) break;
        if (!have_header) {                     // skipping header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;
        while(ss) {
            // cout << "Here #2" << endl;
            string next;                        // gets string until tab char
            if (!getline(ss, next, '\t')) break;
            record.push_back(next);
        }
        if (record.size() != 2) { continue; }   // exactly 2 cols only
    
        // cout << "Here #3" << endl;
        string first_actor(record[0]);
        string last_actor(record[1]);
        
        // Calling actorconnections for this pair
        int result = 0;
        if (fourthIn == "ufind")    // ufind
            result = disjointSet.actorConnectUF(first_actor, last_actor);
        else {                      // widestp
            result = obj->actorConnectGraph(first_actor, last_actor);
        }

        // Pass final result into ofstream along with actor names        
        cout << first_actor << "\t" << last_actor << "\t" << result << endl;
        out << first_actor << "\t" << last_actor << "\t" << result << endl;
    }
    out.close(); // close ofstream
}



