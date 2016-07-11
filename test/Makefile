CXX_FLAGS = -std=c++11 -O2

SOURCE_DIR = ../src
GTEST_DIR = ./gtest/googletest

EXECUTABLES = unittests

COMPILE_TEST = $(CXX) $(CXX_FLAGS) -isystem $(GTEST_DIR)/include -pthread $(GTEST_DIR)/make/gtest_main.a

all: gtest unittests

.PHONY: gtest
gtest:
	$(MAKE) -C $(GTEST_DIR)/make

unittests: gtest
	$(COMPILE_TEST) -I$(SOURCE_DIR) $(SOURCE_DIR)/graph_operations.o $(SOURCE_DIR)/graph.o unittests.cpp -o unittests

clean:
	rm -f $(EXECUTABLES)
	$(MAKE) -C $(GTEST_DIR)/make clean
