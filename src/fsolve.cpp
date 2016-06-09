#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <random>

class Graph {
    std::unordered_map<int, std::unordered_multiset<int> > graph;
    std::minstd_rand lce_gen;
    int _edgesCount, _recordsCount;
public:
    Graph();
    int vertexCount();
    int edgesCount();
    bool hasEdge(int b, int e);
    void addEdge(int b, int e);
    void eraseVertex(int v);
    void getRandomEdge(int &b, int &e);
    const std::unordered_multiset<int> & getAdjacentList(int v);
    std::unordered_map<int, std::unordered_multiset<int> >::const_iterator vertexIteratorBegin();
    std::unordered_map<int, std::unordered_multiset<int> >::const_iterator vertexIteratorEnd();
};

Graph::Graph() {
    _edgesCount = _recordsCount = 0;
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

    graph[b].insert(e);
    if (b != e) {
        ++_recordsCount;
        graph[e].insert(b);
    }
}

int Graph::vertexCount() {
    return graph.size();
}

int Graph::edgesCount() {
    return _edgesCount;
}

void Graph::getRandomEdge(int &b, int &e) {
    std::uniform_int_distribution<> udistr(0, _recordsCount - 1);

    int edgeNumber = udistr(lce_gen);

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

    _edgesCount -= graph[v].size();
    _recordsCount -= graph[v].size();
    graph.erase(v);
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

void readGraph(std::istream &inp, Graph &g) {
    int b, e;
    while (inp >> b >> e) {
        g.addEdge(b, e);
    }
}

int cutLeafs(Graph &g) {
    // TODO: rewrite with DFS
    std::vector<int> leafs;
    int deletedLeafs = 0;
    do {
        leafs.clear();

        for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
            if (vertexIterator->second.size() <= 1) {
                leafs.push_back(vertexIterator->first);
            }
        }

        deletedLeafs += leafs.size();

        for (int &leaf: leafs) {
            g.eraseVertex(leaf);
        }
    } while (! leafs.empty());

    return deletedLeafs;
}

int cutLinkVertexes(Graph &g) {
    // TODO: optimize
    std::vector<int> linkVertices;
    int deletedLinksVerteces = 0;
    bool continueFlag = false;
    do {
        continueFlag = false;
        linkVertices.clear();
        for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
            if (vertexIterator->second.size() == 2) {
                linkVertices.push_back(vertexIterator->first);
            }
        }

        deletedLinksVerteces += linkVertices.size();

        int v1, v2;
        std::vector<int> neighbours_vec;
        for (int &linkVertex: linkVertices) {
            auto neighbours = g.getAdjacentList(linkVertex);
            assert(neighbours.size() == 2);
            neighbours_vec.assign(neighbours.begin(), neighbours.end());
            v1 = neighbours_vec[0];
            v2 = neighbours_vec[1];

            if (v1 != linkVertex && v2 != linkVertex) {
                g.eraseVertex(linkVertex);

                g.addEdge(v1, v2);
                if (v1 == v2) {
                    g.addEdge(v1, v2);
                }

                continueFlag = true;
            }
        }
    } while (continueFlag);

    return deletedLinksVerteces;
}

int cutSelfLoops(Graph &g) {
    std::vector<int> selfLoops;
    for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
        auto &adjList = vertexIterator->second;
        if (adjList.count(vertexIterator->first)) {
            selfLoops.push_back(vertexIterator->first);
        }
    }

    for (int &vertex: selfLoops) {
        g.eraseVertex(vertex);
    }

    return selfLoops.size();
}

void printAdjacentList(std::ostream &outp, Graph &g) {
    for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
        outp << vertexIterator->first << ": ";
        auto &adjList = vertexIterator->second;
        for (auto edgeIterator = adjList.begin(); edgeIterator != adjList.end(); ++edgeIterator) {
            outp << *edgeIterator << " ";
        }
        outp << std::endl;
    }
}

void printEdgeList(std::ostream &outp, Graph &g) {
    for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
        auto &adjList = vertexIterator->second;
        for (auto edgeIterator = adjList.begin(); edgeIterator != adjList.end(); ++edgeIterator) {
            if (vertexIterator->first <= *edgeIterator) {
                outp << vertexIterator->first << " " << *edgeIterator << std::endl;
            }
        }
    }
}

bool beckerSingleGuess(Graph &g, int k) {
    Graph g_new = g;
    cutLeafs(g_new);
    cutLinkVertexes(g_new);

    int cntSelfLoops = cutSelfLoops(g_new);
    k -= cntSelfLoops;

    if (k < 0) {
        return false;
    }
    if (g_new.vertexCount() == 0) {
        return true;
    }

    int b, e, res;
    g_new.getRandomEdge(b, e);
    res =  (rand() % 2 ? b : e);
    g_new.eraseVertex(res);
    return beckerSingleGuess(g_new, k - 1);
}

bool beckerGuess(Graph &g, int k) {
    const int c = 30;
    long long maxit = (1l << (2 * k)); // 4^k steps
    for (int step = 0; step < c*maxit; ++step) {
        if (beckerSingleGuess(g, k)) {
            return true;
        }
    }

    return false;
}

int minFeedbackVertexSetSize(Graph &g) {
    int n = g.vertexCount();
    for (int k = 1; k < n - 1; ++k) {
        if (beckerGuess(g, k)) {
            return k;
        }
    }
    return n - 1;
}

int main() {
    srand(179);
    Graph g;
    readGraph(std::cin, g);
    std::cout << minFeedbackVertexSetSize(g) << std::endl;
    return 0;
}