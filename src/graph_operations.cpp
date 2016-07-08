#include "graph_operations.h"

void readGraph(std::istream &inp, Graph &g) {
    std::string row, b, e;
    int ib, ie;
    while (std::getline(inp, row)) {
        if (row.empty() || row[0] == '#') {
            continue;
        }

        std::stringstream row_stream(row);
        row_stream >> b >> e;
        g.addToDict(b, e, ib, ie);
        g.addEdge(ib, ie);
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

std::vector<int> cutSelfLoops(Graph &g) {
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

    return std::move(selfLoops);
}

void printAdjacentList(std::ostream &outp, Graph &g) {
    for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
        outp << g.translate(vertexIterator->first) << ": ";
        auto &adjList = vertexIterator->second;
        for (auto edgeIterator = adjList.begin(); edgeIterator != adjList.end(); ++edgeIterator) {
            outp << g.translate(*edgeIterator) << " ";
        }
        outp << std::endl;
    }
}

void printEdgeList(std::ostream &outp, Graph &g) {
    for (auto vertexIterator = g.vertexIteratorBegin(); vertexIterator != g.vertexIteratorEnd(); ++vertexIterator) {
        auto &adjList = vertexIterator->second;
        for (auto edgeIterator = adjList.begin(); edgeIterator != adjList.end(); ++edgeIterator) {
            if (vertexIterator->first <= *edgeIterator) {
                outp << g.translate(vertexIterator->first) << " " << g.translate(*edgeIterator) << std::endl;
            }
        }
    }
}

int makeGraphRich(Graph &g, std::vector<int> &ans) {
    int deletedVertices = 0;
    std::vector<int> selfLoops;

    bool continueFlag = true;

    while (continueFlag) {
        continueFlag = false;
        continueFlag = continueFlag || (cutLeafs(g) > 0);
        continueFlag = continueFlag || (cutLinkVertexes(g) > 0);;

        selfLoops = cutSelfLoops(g);
        ans.insert(ans.end(), selfLoops.begin(), selfLoops.end());
        deletedVertices += selfLoops.size();
        continueFlag = continueFlag || (selfLoops.size() > 0);
    }

    return deletedVertices;
}
