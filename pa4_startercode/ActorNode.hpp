//FILE: ActorNode.hpp


#include <vector>
#include <iostream>
#include "ActorEdge.hpp"


using namespace std;


class ActorNode {
private:
    string name;
public:
    vector<string> movies;
    int distance;
    ActorNode* pred;
    ActorEdge moviePath;        // edge from pred

  // Constructor
    ActorNode(string value)
        : name(value),
        distance(1000000),
        pred(nullptr),
        moviePath(ActorEdge("")) {
        }
    
    
   /* {
        name = value;
        distance = 1000000; //TODO  
        pred = NULL;
        moviePath = ActorEdge("");
    }*/
    
  // Adds a movie to the movies vector
    void addMovie(string movie) {
        movies.push_back(movie);
    }

  // Setters
    void setMovie(vector<string> newMovies) {
        movies = newMovies;
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
    bool operator=(const ActorNode& other) const {
        name = other.getName();
        movies = other.getMovies();
        return true;
    }*/
};
