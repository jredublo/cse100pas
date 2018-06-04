//FILE: ActorEdge.hpp


#include <vector>
#include <iostream>
#include <algorithm>
// #include "ActorNode.hpp"

using namespace std;

class ActorEdge {
private:
    vector<string> sharedMovies;    // movies that link the original actornode to partner
    string partner;                 // name of partner
    bool visited;

public:
    // Constructor
    ActorEdge(string coActor)
        : partner(coActor),
        visited(false) {}

    // Adds a movie to sharedMovies if not present yet
    void addSharedMovie(string movie) {
        if ((find(sharedMovies.begin(), sharedMovies.end(), movie)) 
                == sharedMovies.end()) {
            sharedMovies.push_back(movie);
        }
    }

    // Getters
    vector<string> getSharedMovies() const {
        return sharedMovies;
    }
    string /*ActorNode TODO?*/ getPartner() const {
        return partner;
    }


    // Overloading Operators
    bool operator<(const ActorEdge& other) const {
        return partner < other.getPartner();
        // return partner.getName() < other.getPartner().getName();
    }
    
    bool operator==(const ActorEdge& other) const {
        if (partner == other.getPartner()) {
            return true;
        }
        else 
            return false;
        }

};
