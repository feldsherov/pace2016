#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <cassert>

std::unordered_set<std::string> ans;
std::unordered_map<std::string, std::vector<std::string>> graph;

void readAns(std::istream &inp, std::unordered_set<std::string> &ans) {
    std::string row;
    while (std::getline(inp, row)) {
        if (ans.count(row)) {
            std::cout << "Duplicate vertex in answer" << std::endl;
            exit(-1);
        }
        ans.insert(row);
    }
}

void readGraph(std::istream &inp, std::unordered_map<std::string, std::vector<std::string>> &graph) {
    std::string row;
    std::string b, e;
    while (std::getline(inp, row)) {
        if (row[0] == '#') {
            continue;
        }
        std::stringstream row_stream(row);
        row_stream >> b >> e;
        if (!graph.count(b)) graph.insert(make_pair(b, std::vector<std::string>()));
        if (!graph.count(e)) graph.insert(make_pair(e, std::vector<std::string>()));
        graph[b].push_back(e);
        graph[e].push_back(b);
    }
}

bool hasCycle(std::string v, std::unordered_set<std::string> &ans,
                std::unordered_map<std::string, std::vector<std::string>> &graph,
                std::unordered_set<std::string> used) {
    for (auto &el: graph[v]) {
        if (ans.count(el)) continue;

        if (used.count(el)) return 1;

        used.insert(el);
        if (hasCycle(el, ans, graph, used)) {
            return 1;
        }
    }
    return 0;
}

int check(std::unordered_set<std::string> &ans, std::unordered_map<std::string, std::vector<std::string>> &graph) {
    for (auto &el: ans) {
        if (!graph.count(el)) {
            std::cout << "One of verteces is not from graph" << std::endl;
            return 0;
        }
        graph.erase(el);
    }

    if (graph.empty()) {
        std::cout << "Delete all vertices it is not optimal" << std::endl;
        return 0;
    }
    auto v = graph.begin()->first;
    std::unordered_set<std::string> used;
    return (hasCycle(v, ans, graph, used) == 0);
}

int main(int argc, char **argv) {
    assert(argc == 3);
    std::ifstream testin(argv[1]), ansin(argv[2]);

    readAns(ansin, ans);
    readGraph(testin, graph);
    if (!check(ans, graph)) {
        std::cout << "Answer is not FVS" << std::endl;
        return -2;
    }
    return 0;
}
