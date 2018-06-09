
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


all: clean pathfinder printGraph actorconnections 
#FINALSUBBBall: pathfinder actorconnections


#pathfinder: pathfinder.o ActorGraph.o
#	echo 1
#	$(g++ -std=c++11 -o USet.o USet.hpp)
#	$(g++ -std=c++11 -o UNode.o UNode.hpp)
#pathfinder.o: pathfinder.cpp ActorGraph.h
#	echo 2
#actorconnections: actorconnections.o ActorGraph.o USet.o
#	echo 3
#actorconnections.o: actorconnections.cpp ActorGraph.h USet.hpp
#	echo 4
#	
#USet: USet.o USet.hpp UNode.o
#	echo 5
#ActorGraph.o: ActorEdge.o ActorNode.o ActorGraph.h
#	echo 6
#ActorEdge.o: ActorEdge.hpp
#	echo 7
#ActorNode.o: ActorNode.hpp ActorEdge.o
#	echo 8
#USet.o: UNode.o USet.hpp
#	echo 9
#Movie.o: USet.hpp
#UNode.o: UNode.hpp USet.hpp
#
#
#printGraph: printGraph.o ActorGraph.o
#printGraph.o: ActorEdge.hpp ActorNode.hpp ActorGraph.h




pathfinder: pathfinder.o ActorGraph.o
pathfinder.o: pathfinder.cpp ActorGraph.h

ActorGraph.o: ActorEdge.o ActorNode.o ActorGraph.h
ActorEdge.o: ActorEdge.hpp
ActorNode.o: ActorNode.hpp ActorEdge.o

printGraph: printGraph.o ActorGraph.o
printGraph.o: ActorEdge.hpp ActorNode.hpp ActorGraph.h

actorconnections: actorconnections.o ActorGraph.o $(USet)
actorconnections.o: actorconnections.cpp ActorGraph.h USet.hpp

USet: USet.hpp
	$(CC) $(CXXFLAGS) -o USet.o USet.hpp $(UNode)
UNode: UNode.hpp
	$(CC) $(CXXFLAGS) -o UNode.o UNode.hpp


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~for final submission

#actorconnections: ActorGraph.o

# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers

#ActorGraph.o: UnionFind.hpp Movie.h ActorNode.h ActorEdge.h ActorGraph.h
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

clean:
		rm -f pathfinder *.o core*


