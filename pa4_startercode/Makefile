# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
	    CPPFLAGS += -O3
		    LDFLAGS += -O3
		else
		    CPPFLAGS += -g
			    LDFLAGS += -g
			endif

all: pathfinder printGraph actorconnections 
#FINALSUBBBall: pathfinder actorconnections

# include what ever source code *.h files pathfinder relies on (these are merely the ones that were used in the solution)





pathfinder: pathfinder.o ActorGraph.o
pathfinder.o: pathfinder.cpp ActorGraph.h
actorconnections: actorconnections.o ActorGraph.o UUNode.o
actorconnections.o: actorconnections.cpp ActorGraph.h USet.hpp

ActorGraph.o: ActorEdge.o ActorNode.o ActorGraph.h
ActorEdge.o: ActorEdge.hpp
ActorNode.o: ActorNode.hpp ActorEdge.o
USet.o: UNode.o USet.hpp
UNode.o: UNode.hpp

printGraph: printGraph.o ActorGraph.o
printGraph.o: ActorEdge.hpp ActorNode.hpp ActorGraph.h




#pathfinder: pathfinder.o ActorGraph.o
#pathfinder.o: pathfinder.cpp ActorGraph.h
#ActorGraph.o: ActorEdge.o ActorNode.o ActorGraph.h
#ActorEdge.o: ActorEdge.hpp
#ActorNode.o: ActorNode.hpp ActorEdge.o

#USet.o: UNode.o USet.hpp
#UNode.o: UNode.hpp

#printGraph: printGraph.o ActorGraph.o
#printGraph.o: ActorEdge.hpp ActorNode.hpp ActorGraph.h

#actorconnections: actorconnections.o ActorGraph.o USet.o
#actorconnections.o: actorconnections.cpp ActorGraph.h USet.hpp



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~for final submission

#actorconnections: ActorGraph.o

# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers

#ActorGraph.o: UnionFind.hpp Movie.h ActorNode.h ActorEdge.h ActorGraph.h
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

clean:
		rm -f pathfinder *.o core*


