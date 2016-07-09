CXX = g++
CFLAGS = -std=c++11
LFLAGS =

ifeq ($(DEBUG), 1)
DEFINES = -DDEBUG
CFLAGS += -g -O0
else
DEFINES =
CFLAGS += -O2


endif

ifeq ($(COUNT), 1)
DEFINES += -DCOUNTANS
else
DEFINES =
endif


DEFINES += -DFULLANS


LIB_OBJECTS = graph.o  graph_operations.o
OBJECTS = $(LIB_OBJECTS) fsolve.o
EXECUTABLES = fsolve fcheck fslow

all: $(OBJECTS) fcheck fslow
	$(CXX) $(DEFINES) fsolve.o $(LIB_OBJECTS) -o fsolve

fslow: $(OBJECTS)
	$(CXX) $(CFLAGS) fslow.cpp $(LIB_OBJECTS) -o fslow

fsolve.o:
	$(CXX) $(CFLAGS) $(DEFINES) fsolve.cpp -c -o fsolve.o

graph.o:
	$(CXX) $(CFLAGS) $(DEFINES) graph.cpp -c -o graph.o

graph_operations.o:
	$(CXX) $(CFLAGS) $(DEFINES) graph_operations.cpp -c -o graph_operations.o

fcheck:
	$(CXX) $(CFLAGS) fcheck.cpp -o fcheck

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES)
