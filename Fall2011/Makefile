CC=g++
CCFLAGS=-g -c -std=c++0x -I boost_1_46_0
LDFLAGS=-pthread 
SOURCES=Environment.cpp Agent.cpp Domains.cpp MDP.cpp Episodes.cpp Main.cpp
OBJECT=$(SOURCES:.cpp=.o)
EXECUTABLE=agent

all:$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):$(OBJECT)
	$(CC) $(LDFLAGS) $(OBJECT) -o $@

.cpp.o:
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm -rf *o agent
