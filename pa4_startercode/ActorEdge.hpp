//FILE: ActorEdge.hpp


#include <vector>
#include <iostream>
#include <algorithm>
#include "ActorNode.hpp"

using namespace std;

class ActorEdge {
private:
    vector<string> sharedMovies;    // movies that link the original actornode to partner
    ActorNode partner;              // partner

public:
    // Constructor
    ActorEdge(ActorNode coActor)
        : partner(coActor) { }

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
    ActorNode getPartner() const {
        return partner;
    }


    // Overloading Operators
    bool operator<(const ActorEdge& other) const {
        return partner.getName() < other.getPartner().getName();
    }
    
    bool operator==(const ActorEdge& other) const {
        if (partner.getName() == other.getPartner().getName()) {
            return true;
        }
        else 
            return false;
        }

};
