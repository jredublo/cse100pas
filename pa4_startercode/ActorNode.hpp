//FILE: ActorNode.hpp


#include <vector>
#include <iostream>

using namespace std;


class ActorNode {
private:
    string name;
    vector<string> movies;

public:
    // constructor
    ActorNode(string value) {
        name = value;
    }
    
    // adds a movie to the movies vector
    void addMovie(string movie) {
        movies.push_back(movie);
    }

    string getName() const {
        return name;
    }

    vector<string> getMovies() const {
        return movies;
    }

    bool operator<(const ActorNode& one, const ActorNode& other) {
        return true;
    }

    bool operator!=(const ActorNode& other) {
        return name != other.getName();
    }
};
