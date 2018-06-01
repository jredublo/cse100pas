//FILE: ActorNode.hpp


#include <vector>
#include <iostream>

using namespace std;


class ActorNode {
private:
    string name;
    vector<string> movies;
    
public:
    int distance;
    ActorNode* pred;

  // Constructor
    ActorNode(string value) {
        name = value;
        distance = 1000000; //TODO  
        pred = NULL;
    }
    
  // Adds a movie to the movies vector
    void addMovie(string movie) {
        movies.push_back(movie);
    }

  // Getters
    string getName() const { 
        return name; 
    }
    vector<string> getMovies() const {
        return movies;
    }

  // Operator Overloading
    bool operator<(const ActorNode& other) const {
        return name < other.getName();
    }
    bool operator==(const ActorNode& other) const {
        if (name == other.getName())
            return true;
        else
            return false;
    }

    /*
    bool operator!=(const ActorNode& other) {
        return name != other.getName();
    }*/
};
