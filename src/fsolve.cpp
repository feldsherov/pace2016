#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <unistd.h>
#include <cstdlib>

#include "graph.h"

int seed;

bool deterministicBranchByMultiplyEdges(Graph &g, int k, std::vector<int> &ans);

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

bool beckerSingleGuess(Graph &g, int k, std::vector<int> &ans) {
    Graph g_new = g;
    int countDeletedSelfLoops = makeGraphRich(g_new, ans);
    k -= countDeletedSelfLoops;


    if (k < 0) {
        ans.resize(ans.size() - countDeletedSelfLoops);
        return false;
    }
    if (g_new.vertexCount() == 0) {
        return true;
    }

    bool sucessFlag;
    int b, e, res;
    g_new.getRandomEdge(b, e);

    res = (rand() % 2 ? b : e);

    ans.push_back(res);
    g_new.eraseVertex(res);

    sucessFlag = beckerSingleGuess(g_new, k - 1, ans);
    if (!sucessFlag) {
        ans.pop_back();
    }
if (!sucessFlag) {
        ans.resize(ans.size() - countDeletedSelfLoops);
    }

    return sucessFlag;
}

bool beckerGuess(Graph &g, int k, std::vector<int> &ans) {
#ifdef DEBUG
    std::cerr << "Becker k = " << k << std::endl;
#endif
    const int c = 30;
    long long maxit = (1l << (2 * k)); // 4^k steps
    for (int step = 0; step < c*maxit; ++step) {
        if (beckerSingleGuess(g, k, ans)) {
            return true;
        }
    }

    return false;
}

bool deterministicBranchByMultiplyEdges(Graph &g, int k, std::vector<int> &ans) {
    #ifdef DEBUG
    std::cerr << "deterministicBranchByMultiplyEdges k = " << k << std::endl;
    #endif
    Graph g_new = g;
    int countDeletedSelfLoops = makeGraphRich(g_new, ans);
    k -= countDeletedSelfLoops;

    if (k < 0) {
        ans.resize(ans.size() - countDeletedSelfLoops);
        return false;
    }
    if (g_new.vertexCount() == 0) {
        return true;
    }

    if (!g_new.countMultiplyEdges()) {
        bool sucessFlag = beckerGuess(g_new, k, ans);
        if (!sucessFlag) {
            ans.resize(ans.size() - countDeletedSelfLoops);
        }
        return sucessFlag;
    }
    auto multiplyEdge = g_new.anyMultiplyEdge();
    int b = multiplyEdge.first, e = multiplyEdge.second;

    Graph g_without_b = g_new;
    Graph g_without_e = g_new;

    g_without_b.eraseVertex(b);
    ans.push_back(b);
    if(deterministicBranchByMultiplyEdges(g_without_b, k - 1, ans)) {
        return true;
    }
    ans.pop_back();

    g_without_e.eraseVertex(e);
    ans.push_back(e);
    if(deterministicBranchByMultiplyEdges(g_without_e, k - 1, ans)) {
        return true;
    }
    ans.pop_back();

    ans.resize(ans.size() - countDeletedSelfLoops);

    return false;
}

int minFeedbackVertexSetSize(Graph &g, std::vector<int> &ans) {
    int n = g.vertexCount();
    for (int k = 0; k < n - 1; ++k) {
        if (deterministicBranchByMultiplyEdges(g, k, ans)) {
            return k;
        }
    }
    return n - 1;
}

void init(int argc, char **argv) {
    char op;

    seed = 179;

    while ( (op = getopt(argc, argv, "s:")) ) {
        switch(op) {
            case 's':
                seed = atoi(optarg);
                break;
            case '?':
                std::cout << "Unexpected parametr: " << op << std::endl;
                exit(0);
        }
    }

    srand(seed);
}

int main(int argc, char **argv) {
    Graph g;
    std::vector<int> ans;
    readGraph(std::cin, g);
    std::cout << minFeedbackVertexSetSize(g, ans) << std::endl;
#ifdef FULLANS
    for (int &v: ans) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
#endif
    return 0;
}