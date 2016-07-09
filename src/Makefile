CXX = g++
CFLAGS = -std=c++11
LFLAGS =

ifeq ($(DEBUG), 1)
DEFINES = -DDEBUG
CXX += -g -O0
else
DEFINES =
CXX += -O2


endif

ifeq ($(COUNT), 1)
DEFINES += -DCOUNTANS
else
DEFINES =
endif


DEFINES += -DFULLANS


OBJECTS = graph.o fsolve.o graph_operations.o
EXECUTABLES = fsolve fcheck

all: $(OBJECTS) test
	$(CXX) $(OBJECTS) -o fsolve

fsolve.o:
	$(CXX) $(CFLAGS) $(DEFINES) fsolve.cpp -c -o fsolve.o

graph.o:
	$(CXX) $(CFLAGS) $(DEFINES) graph.cpp -c -o graph.o

graph_operations.o:
	$(CXX) $(CFLAGS) $(DEFINES) graph_operations.cpp -c -o graph_operations.o

test:
	$(CXX) $(CFLAGS) fcheck.cpp -o fcheck

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES)
