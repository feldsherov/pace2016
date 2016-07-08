#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <unistd.h>
#include <cstdlib>

#include "graph.h"
#include "graph_operations.h"

int seed;

bool deterministicBranchByMultiplyEdges(Graph &g, int k, std::vector<int> &ans);

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

int minFeedbackVertexSet(Graph &g, std::vector<int> &ans) {
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
#ifdef COUNTANS
    std::cout << minFeedbackVertexSet(g, ans) << std::endl;
#else
    minFeedbackVertexSet(g, ans);
#endif

#ifdef FULLANS
    for (int &v: ans) {
        std::cout << g.translate(v) << std::endl;
    }
#endif
    return 0;
}
