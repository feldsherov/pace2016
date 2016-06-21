CXX = g++ -g
CFLAGS = -std=c++11
LFLAGS =

ifeq ($(DEBUG), 1)
DEFINES = -DDEBUG
else
DEFINES =
endif

ifeq ($(COUNT), 1)
DEFINES += -DCOUNTANS
else
DEFINES =
endif


DEFINES += -DFULLANS


OBJECTS = graph.o fsolve.o
EXECUTABLES = fsolve

all: $(OBJECTS)
	$(CXX) $(OBJECTS) -o fsolve

fsolve.o:
	$(CXX) $(CFLAGS) $(DEFINES) fsolve.cpp -c -o fsolve.o

graph.o:
	$(CXX) $(CFLAGS) $(DEFINES) graph.cpp -c -o graph.o

clean:
	rm -rf $(OBJECTS) $(EXECUTABLES)
