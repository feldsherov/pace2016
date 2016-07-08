#ifndef __GRAPH_OPEARTIONS_H__
#define __GRAPH_OPEARTIONS_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <random>
#include <sstream>

#include "graph_operations.h"
#include "graph.h"

void readGraph(std::istream &inp, Graph &g);

int cutLeafs(Graph &g);

int cutLinkVertexes(Graph &g);

std::vector<int> cutSelfLoops(Graph &g);

void printAdjacentList(std::ostream &outp, Graph &g);

void printEdgeList(std::ostream &outp, Graph &g);

int makeGraphRich(Graph &g, std::vector<int> &ans);

#endif // __GRAPH_OPEARTIONS_H__
