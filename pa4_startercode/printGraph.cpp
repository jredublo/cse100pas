// file for a main func just to print our graph

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ActorGraph.h"
#include <map>
#include <set>

using namespace std;
typedef set<string> StringSet;                // uset of strings
typedef map<string, StringSet> UmapStrings;   // umap of (strings, StringSet) for movieMap
typedef set<ActorEdge> EdgeSet;               // uset of actorEdges
typedef map<ActorNode, EdgeSet> UmapNodes;    // umap of (ActNodes,EdgeSet) for  theMap



int main (int argc, char** argv) {
   /* cout << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << endl;
    
    }*/
    
    
    // take in an input file, pass as an arg to loadFromFile
    ActorGraph* obj = new ActorGraph();
    obj->loadFromFile(argv[1], false);
    /*UmapNodes ourMap = obj.getTheMap();

    // iterate thru map and print out actor
    for (UmapNodes::iterator it = ourMap.begin(); it != ourMap.end(); ++it) {
        cout << "Name is " << (it->first).getName() << endl;
        for (EdgeSet:: iterator iter = (it->second).begin(); 
                iter != (it->second).end() ; ++iter) {
            cout << "     connection: " << ((*iter).getPartner()).getName() << endl;
        }
    }*/

}