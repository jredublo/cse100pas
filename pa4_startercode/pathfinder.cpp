
/* FILE:        pathfinder.cpp
 * AUTHORS:     Madalynn Norton  Jessica Redublo
 * DUE DATE:    June 8, 2018
 * DESCR:       Holds main method for taking in user input and 
 *                  calling pathfinder and using either a weighted 
 *                  or an unweighted graph depending on u or w.
 */

#include <vector>
#include <iostream>
#include "ActorGraph.h"
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <map>

using namespace std;

typedef set<string> StringSet;                  // set of strings
typedef map<string, StringSet> UmapStrings;     // map (strings, StringSet)  <movieMap>
typedef set<ActorEdge> EdgeSet;                 // set of ActorEdges
typedef map<ActorNode*, EdgeSet> UmapNodes;     // map (ActorNodes, EdgeSet) <theMap>


    /* ----------------------------- MAIN FUNCTION ----------------------------- */


/** Main function: finding the shortest path between 2 actors */
int main(int argc, char** argv) 
{
    if (argc != 5) {
        cerr << "ERROR! Requires 4 arguments for pathfinder!" << endl;
        return -1;
    }    

    string firstIn = argv[1];               // all actors/movies
    string secondIn = argv[2];              // for final sub
    string thirdIn = argv[3];               // pairs we want
    string fourthIn = argv[4];              // out file

    ActorGraph* obj = new ActorGraph();
    if (secondIn == "u") {                      
        obj->loadFromFile(argv[1], false, true);    // creating unweighted graph
    }
    else {
        obj->loadFromFile(argv[1], true, true);     // creating weighted graph
    }

    // Open ofstream to outfile and add header
    ofstream out(fourthIn);
    cout << "(actor)--[movie#@year]-->(actor)--..." << endl;
    out << "(actor)--[movie#@year]-->(actor)--..." << endl;

    
    ifstream in3(thirdIn);
    bool have_header = false;
    while (in3) {                               // Read in pairs line by line
        // cout << "Here #1" << endl;
        string s;
        if (!getline( in3, s)) break;           // get next line
        if (!have_header) {                     // skip header
            have_header = true;
            continue;
        }
        istringstream ss(s);
        vector<string> record;
        while(ss) {
            // cout << "Here #2" << endl;
            string next;                        // gets string until tab -> next
            if (!getline(ss, next, '\t')) break;
            record.push_back(next);
        }
        if (record.size() != 2) { continue; }   // exactly 2 columns only
    
        // cout << "Here #3" << endl;
        string first_actor(record[0]);
        string last_actor(record[1]);
        
        // Calling pathfinder for this pair
        vector<string> resultPath = obj->pathFinder(first_actor, last_actor);
       
        // Pass path into ofstream, adding in the arrows around movies
        for (unsigned int i = 0; i < resultPath.size(); i++) {
            if (i%2 == 1) {     // movies
                out << "--[" << resultPath.at(i) << "]-->";
                cout << "--[" << resultPath.at(i) << "]-->";
            } else {            // actors
                out << "(" << resultPath.at(i) << ")";
                cout << "(" << resultPath.at(i) << ")";
            }
        }
        
        cout << endl;                           // endl to seperate paths
        out << endl;
    }
    out.close();
}



