
/* FILE:    ActorNode.hpp
 * AUTHOR:  Madalynn Norton, Jessica Redublo
 * DATE:    June 8, 2018
 * DESCR:   Holds the function declarations for ActorNode to be used
 *              later by ActorEdge and ActorGraph.
 */

#include <vector>
#include <iostream>
#include "ActorEdge.hpp"

using namespace std;


    /* ---------------------------- START CLASS ---------------------------- */


class ActorNode {
private:
    string name;                    // name of actor


public:
    vector<string> movies;          // list of movies they starred in (filmography)
    int distance;                   // distance traveled (PF) or bandwidth (AC)
    ActorNode* pred;                // predecessor 
    ActorEdge moviePath;            // edge from pred to get movie later
    bool done;                      // if shortest path to this node is alrdy found


    /** Constructor */
    ActorNode(string value, bool frmP)
        : name(value),
        distance(1000000),
        pred(nullptr),
        done(false),
        moviePath(ActorEdge("", nullptr,nullptr)) {
    
        if (!frmP) {
            distance = 0;
        }

    }
   
    /** Destructor */
    ~ActorNode() {
        // delete pred;
    }

    /** Adds given string movie to the movies vector */
    void addMovie(string movie) {
        movies.push_back(movie);
    }

    /** Sets movies vector to the given vector */
    void setMovie(vector<string> newMovies) {
        movies = newMovies;
    }

    /** Returns actor name */
    string getName() const { 
        return name; 
    }

    /** Returns vector of movies */
    vector<string> getMovies() const {
        return movies;
    }


    /** Operator overloading for use by map & sets */
    bool operator<(const ActorNode& other) const {
        return name < other.getName();
    }
    bool operator==(const ActorNode& other) const {
        if (name == other.getName()) return true;
        else return false;
    }

};
