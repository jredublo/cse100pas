
/* FILE:    ActorEdge.hpp
 * AUTHOR:  Madalynn Norton, Jessica Redublo
 * DATE:    June8, 2018
 * DESCR:   Holds the function declarations and def'ns for 
 *              ActorEdge that is essential to building the
 *              ActorGraph.
 */

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;


      /* ----------------------------- START CLASS ---------------------------- */


class ActorEdge {
private:
    // Members
    vector<string>* sharedMovies;           // movies that link og node to partner node
    string partner;                         // name of partner (castmate)
    bool visited;                           // visited already

public:
    int* weight;
    

    /** Constructor */
    ActorEdge( string coActor, int* newInt, vector<string>* newVect )
        : partner(coActor),
        weight(newInt),
        sharedMovies(newVect){}

    /** Destructor */
    ~ActorEdge(){
        //delete sharedMovies;
        //delete weight;
    }

    /** Adds given movie string to sharedMovies vector if it doesn't exist */
    void addSharedMovie(string movie) {
        if ((find(sharedMovies->begin(), sharedMovies->end(), movie)) 
                == sharedMovies->end()) {
            sharedMovies->push_back(movie);
        }
    }

    /** Returns sharedMovies vector */
    vector<string>* getSharedMovies() const {
        return sharedMovies;
    }

    /** Returns name string of partner */
    string getPartner() const {
        return partner;
    }

    /** Returns pointer to the weight of the edge */
    int* getWait() const{
        return weight;
    }

    /** Set the value stored in the weight pointer */
    void setWait(int wait) {
        *weight = wait;
    }

    /** Operator overloading for use by the maps and sets */
    bool operator<(const ActorEdge& other) const {
        return partner < other.getPartner();
    }
    bool operator==(const ActorEdge& other) const {
        if (partner == other.getPartner()) { return true; }
        else { return false; }
    }

};
