//MAIN FILE: pathfinder.cpp

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
typedef set<string> StringSet;                // uset of strings
typedef map<string, StringSet> UmapStrings;   // umap of (strings, StringSet) for movieMap
typedef set<ActorEdge> EdgeSet;               // uset of ActorEdges
typedef map<ActorNode*, EdgeSet> UmapNodes;    // umap of (ActorNodes, EdgeSet) for final graph


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



/* Main function: finding the shortest path between 2 actors */
int main(int argc, char** argv) {
    
    string firstIn = argv[1];   // all actors/movies
    string secondIn = argv[2];  // names of actor pairs
    string thirdIn = argv[3];   // output file
    string fourthIn = argv[4];  // widestp or ufind


    ActorGraph* obj = new ActorGraph();
    
    // true: always weighted edges, false for frmP
    if (fourthIn == "widestp") { // widestp  TODO TODO TODO TODO TODO
        obj->loadFromFile(argv[1], true, false); // create theMap (our graph)
    }
    else { // ufind
        // TODO not using our graph, make the sets???? TODO
    }

    ofstream out(thirdIn);

    // FILE HEADER
    cout << "Actor1\tActor2\tYear" << endl;

    // read in second input to start getting what pairs we want
    ifstream in3(secondIn);
    bool have_header = false;
    while (in3) {
    //cout << "Here #1" << endl;
        string s;
        // get next line
        if (!getline( in3, s)) break;
        if (!have_header) {
            // skip header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;
        while(ss) {
            //cout << "Here #2" << endl;
            string next;
            // get the next string before hitting a tab char and put into 'next'
            if (!getline(ss, next, '\t')) break;
            record.push_back(next);
        }
        if (record.size() != 2) {
            // we need 2 columns
            continue;
        }
    
        //cout << "Here #3" << endl;
        string first_actor(record[0]);
        string last_actor(record[1]);
        
        int result = 0;

        // actorConnections function for this pair
        if (fourthIn == "ufind")
            result = obj->actorConnectUF(first_actor, last_actor);
        else
            result = obj->actorConnectGraph(first_actor, last_actor);


        // OUTPUT to out stream actor1 tabbb actor 2 tabbb year
        
        cout << first_actor << "\t" << last_actor << "\t" << result << endl; // sepereate the pair paths on diff lines
        out << first_actor << "\t" << last_actor << "\t" << result << endl; // sepereate the pair paths on diff lines
    }

        out.close(); // close ofstream
}
