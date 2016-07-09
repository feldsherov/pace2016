#include <iostream>
#include <vector>

#include "graph.h"
#include "graph_operations.h"

inline bool getBit(int v, int pos) {
    return ((v >> pos) & 1);
}

int solveExp(Graph &g, std::vector<int> &ans) {

    Graph gTmp;
    std::vector<int> selfLoops;
    int vSize = g.vertexCount();
    int cAnsSize = vSize, cSetSize;

    assert(vSize < 30);

    for (uint32_t msk = 0; msk < (1 << vSize); ++msk) {
        gTmp = g;
        cSetSize = 0;
        for (int v = 0; v < vSize; ++v) {
            if (getBit(msk, v)) {
                gTmp.eraseVertex(v);
                cSetSize++;
            }
        }

        selfLoops.clear();
        makeGraphRich(gTmp, selfLoops);

        if (gTmp.vertexCount() == 0 && cSetSize + selfLoops.size() < cAnsSize) {
            ans.clear();
            ans.insert(ans.end(), selfLoops.begin(), selfLoops.end());
            for (int v = 0; v < vSize; ++v) {
                if (getBit(msk, v)) {
                    ans.push_back(v);
                }
            }

            cAnsSize = ans.size();
        }
    }

    // min FVS is alway less than vSize

    return cAnsSize;
}

int main(int argc, char **argv) {
    Graph g;
    std::vector<int> ans;
    readGraph(std::cin, g);
    std::cout << solveExp(g, ans) << std::endl;

    for (auto &v: ans) {
        std::cout << g.translate(v) << " ";
    }
    std::cout << std::endl;
    return 0;
}
