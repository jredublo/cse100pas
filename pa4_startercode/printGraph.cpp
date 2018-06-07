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
typedef map<ActorNode*, EdgeSet> UmapNodes;    // umap of (ActNodes,EdgeSet) for  theMap



int main (int argc, char** argv) {
   /* cout << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << argv[i] << endl;
    
    }*/
    
    
    // take in an input file, pass as an arg to loadFromFile
    ActorGraph obj;// = new ActorGraph();
    obj.loadFromFile(argv[1], false, true);
    UmapStrings movMap = obj.getMovieMap();
    
    // printing movie map
    for (UmapStrings::iterator it = movMap.begin(); it != movMap.end(); ++it) {
        string film = it->first;
        StringSet castList = it->second;
        cout << "Movie is " << film << endl;
        
        if (castList.empty()) {
            cout << "     Set is empty, no cast :( " << endl;
            
        }

        for (StringSet::iterator iter = castList.begin(); 
                iter != castList.end() ; ++iter) {
            cout << "     actor: " << (*iter) << endl;
        }
    }
    



    UmapNodes ourMap = obj.getTheMap();
    // iterate thru theMap and print out actor 
    for (UmapNodes::iterator it = ourMap.begin(); it != ourMap.end(); ++it) {
        ActorNode* something = it->first;
        cout << "Name is " << (*something).getName() << endl;
        
        if (it->second.empty()) {
            cout << "     Set is empty, no connections :( " << endl;
            
        }

        for (EdgeSet:: iterator iter = (it->second).begin(); 
                iter != (it->second).end() ; ++iter) {
            cout << "     connection: " << ((*iter).getPartner()) << endl;
            vector<string>* what = (*iter).getSharedMovies();
            for (unsigned int p = 0; p < what->size(); p++) 
                cout << "          movies: " << what->at(p) << endl;
        }
    }
}
