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
typedef map<ActorNode, EdgeSet> UmapNodes;    // umap of (ActorNodes, EdgeSet) for final graph


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


// Main function: finding the shortest path between 2 actors
int main(int argc, char** argv) {
    priority_queue<ActorNode, vector<ActorNode>, myComp> q;
    string firstIn = argv[1];   // all actors/movies
    string secondIn = argv[2];  // for final sub TODO weighted
    string thirdIn = argv[3];   // pairs we want
    string fourthIn = argv[4];  // out file

    ActorGraph* obj = new ActorGraph();
    obj->loadFromFile(argv[1], false); // create theMap (our graph)
    UmapNodes ourMap = obj.getTheMap();

    // read in second input to start getting what pairs we want
    ifstream in3(thirdIn);
    bool have_header = false;
    while (in3) {
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
            string next;
            // get the next string before hitting a tab char and put into 'next'
            if (!getline(ss, next, '\t')) break;
            record.push_back(next);
        }
        if (record.size() != 2) {
            // we need 2 columns
            continue;
        }
    
        string first_actor(record[0]);
        string last_actor(record[1]);
        
        // pathfinder function for this pair
        // output the best path in the outfile
    }

    // find our first actor

    UmapNodes:iterator iter = 


}
