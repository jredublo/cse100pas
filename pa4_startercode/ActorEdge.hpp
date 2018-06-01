//FILE: ActorEdge.hpp


#include <vector>
#include <iostream>
#include <algorithm>
#include "ActorNode.hpp"

using namespace std;

class ActorEdge {
private:
    vector<string> sharedMovies;
    ActorNode partner;

public:
    // constructor
    ActorEdge(ActorNode coActor):
        partner(coActor)
    {
    }

    //adds a movie to sharedMovies
    void addSharedMovie(string movie) {
        if ((find(sharedMovies.begin(), sharedMovies.end(), movie)) 
                == sharedMovies.end()) {
            sharedMovies.push_back(movie);
        }
    }

    bool operator<(const ActorEdge& other) {
        return true;
    }

    bool operator!=(const ActorEdge& other) {
        return partner.getName() != other.getName();
    }

};
