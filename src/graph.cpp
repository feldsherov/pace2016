#include "graph.h"

Graph::Graph() {
    _edgesCount = _recordsCount = 0;
    _multiplyEdgesCount = 0;
    _lastId = 0;
}

void Graph::addToDict(const std::string &b, std::string &e, int &ib, int &ie) {
    if (_dict.count(b)) {
        ib = _dict[b];
    }
    else {
        _dict[b] = ib = _lastId++;
        _idict[ib] = b;
    }

    if (_dict.count(e)) {
        ie = _dict[e];
    }
    else {
        _dict[e] = ie = _lastId++;
        _idict[ie] = e;
    }
}

int Graph::translate(const std::string &s) {
    return (_dict.count(s) ? _dict[s] : -1);
}

std::string Graph::translate(int v) {
    return (_idict.count(v) ? _idict[v] : "");
}

int Graph::vertexCount() {
    return graph.size();
}

int Graph::edgesCount() {
    return _edgesCount;
}

bool Graph::hasEdge(int b, int e) {
    if (! graph.count(b)) {
        return false;
    }
    return static_cast<bool>(graph[b].count(e));
}

void Graph::addEdge(int b, int e) {
    ++_edgesCount;
    ++_recordsCount;

    if (b != e && graph[b].count(e)) {
        mutiplyEdges[b].insert(e);
        mutiplyEdges[e].insert(b);
        _multiplyEdgesCount++;
    }

    graph[b].insert(e);
    if (b != e) {
        ++_recordsCount;
        graph[e].insert(b);
    }
}

void Graph::eraseVertex(int v) {
    if (! graph.count(v)) {
        return;
    }

    for (auto &target: graph[v]) {
        if (target != v) {
            assert(graph[target].count(v));

            auto it = graph[target].find(v);
            graph[target].erase(it);
            _recordsCount -= 1;
        }
    }

    _multiplyEdgesCount -= mutiplyEdges[v].size();
    for (int to: mutiplyEdges[v]) {
        auto it = mutiplyEdges[to].find(v);
        mutiplyEdges[to].erase(it);
    }

    mutiplyEdges.erase(v);

    _edgesCount -= graph[v].size();
    _recordsCount -= graph[v].size();
    graph.erase(v);
}


void Graph::getRandomEdge(int &b, int &e) {
    int edgeNumber = rand() % _recordsCount;

    for (auto vertexIterator = graph.begin(); vertexIterator != graph.end(); ++vertexIterator) {
        auto &adjList = vertexIterator->second;
        if (edgeNumber >= adjList.size()) {
            edgeNumber -= adjList.size();
        }
        else {
            auto it = adjList.begin();
            for (int i = 0; i < edgeNumber; ++i) {
                it++;
            }

            b = vertexIterator->first;
            e = *it;
            return;
        }
    }

    assert(false);
}

int Graph::countMultiplyEdges() {
    return _multiplyEdgesCount;
}

std::pair<int, int> Graph::anyMultiplyEdge() {
    assert(_multiplyEdgesCount > 0);

    auto it = mutiplyEdges.begin();
    while (it->second.empty()) {
        it++;
    }

    int b = it->first, e = *(it->second.begin());
    auto ans = std::make_pair(b, e);

    return ans;
}

const std::unordered_multiset<int> & Graph::getAdjacentList(int v) {
    return graph[v];
}

std::unordered_map<int, std::unordered_multiset<int> >::const_iterator Graph::vertexIteratorBegin() {
    return graph.begin();
}

std::unordered_map<int, std::unordered_multiset<int> >::const_iterator Graph::vertexIteratorEnd() {
    return graph.end();
}
