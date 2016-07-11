#ifndef __GRAPH_H__

#define __GRAPH_H__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <random>

class Graph {
    std::unordered_map<int, std::unordered_multiset<int> > graph;
    std::unordered_map<int, std::unordered_multiset<int> > mutiplyEdges;
    std::unordered_map<std::string, int> _dict;
    std::unordered_map<int, std::string> _idict;
    std::minstd_rand lce_gen;
    int _edgesCount, _recordsCount, _multiplyEdgesCount;
    int _lastId;

public:
    Graph();
    int vertexCount();
    int edgesCount();

    bool hasEdge(int b, int e);
    void addEdge(int b, int e);
    void addToDict(const std::string &b, std::string &e, int &ib, int &ie);
    int translate(const std::string &s);
    std::string translate(int s);

    void eraseVertex(int v);

    void getRandomEdge(int &b, int &e);

    int multiplyEdgesCount();
    std::pair<int, int> anyMultiplyEdge();

    const std::unordered_multiset<int> & getAdjacentList(int v);
    std::unordered_map<int, std::unordered_multiset<int> >::const_iterator vertexIteratorBegin();
    std::unordered_map<int, std::unordered_multiset<int> >::const_iterator vertexIteratorEnd();
};
#endif
