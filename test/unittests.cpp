#include <fstream>

#include "gtest/gtest.h"

#include "graph.h"
#include "graph_operations.h"

TEST(TEST_readGraph, empty) {
    std::ifstream graph_in("test_instances/empty.graph");
    Graph g;
    readGraph(graph_in, g);
    ASSERT_EQ(g.vertexCount(), 0);
    ASSERT_EQ(g.edgesCount(), 0);
}

TEST(TEST_readGraph, empty_line) {
    std::ifstream graph_in("test_instances/empty_line.graph");
    Graph g;
    readGraph(graph_in, g);
    ASSERT_EQ(g.vertexCount(), 4);
    ASSERT_EQ(g.edgesCount(), 2);
    auto name1 = g.translate("1");
    auto name2 = g.translate("2");
    auto name3 = g.translate("3");
    auto name4 = g.translate("4");
    auto list1 = g.getAdjacentList(name1);
    auto list2 = g.getAdjacentList(name2);
    auto list3 = g.getAdjacentList(name3);
    auto list4 = g.getAdjacentList(name4);

    ASSERT_EQ(list1, std::unordered_multiset<int>({name2}));
    ASSERT_EQ(list2, std::unordered_multiset<int>({name1}));
    ASSERT_EQ(list4, std::unordered_multiset<int>({name3}));
    ASSERT_EQ(list3, std::unordered_multiset<int>({name4}));
}

TEST(TEST_readGraph, empty_line_end) {
    std::ifstream graph_in("test_instances/empty_line_end.graph");
    Graph g;
    readGraph(graph_in, g);
    ASSERT_EQ(g.vertexCount(), 2);
    ASSERT_EQ(g.edgesCount(), 1);
    ASSERT_EQ(g.multiplyEdgesCount(), 0);
    auto name1 = g.translate("1");
    auto name2 = g.translate("2");
    auto list1 = g.getAdjacentList(name1);
    auto list2 = g.getAdjacentList(name2);

    ASSERT_EQ(list1, std::unordered_multiset<int>({name2}));
    ASSERT_EQ(list2, std::unordered_multiset<int>({name1}));
}


TEST(TEST_readGraph, comment_end) {
    std::ifstream graph_in("test_instances/comment_end.graph");
    Graph g;
    readGraph(graph_in, g);
    ASSERT_EQ(g.vertexCount(), 2);
    ASSERT_EQ(g.edgesCount(), 3);
    ASSERT_EQ(g.multiplyEdgesCount(), 2);
    auto name1 = g.translate("1");
    auto name2 = g.translate("2");
    auto list1 = g.getAdjacentList(name1);
    auto list2 = g.getAdjacentList(name2);

    ASSERT_EQ(list1, std::unordered_multiset<int>({name2, name2, name2}));
    ASSERT_EQ(list2, std::unordered_multiset<int>({name1, name1, name1}));
}


TEST(TEST_readGraph, comment_middle) {
    std::ifstream graph_in("test_instances/comment_middle.graph");
    Graph g;
    readGraph(graph_in, g);
    ASSERT_EQ(g.vertexCount(), 2);
    ASSERT_EQ(g.edgesCount(), 2);
    ASSERT_EQ(g.multiplyEdgesCount(), 1);
    auto name1 = g.translate("1");
    auto name2 = g.translate("2");
    auto list1 = g.getAdjacentList(name1);
    auto list2 = g.getAdjacentList(name2);

    ASSERT_EQ(list1, std::unordered_multiset<int>({name2, name2}));
    ASSERT_EQ(list2, std::unordered_multiset<int>({name1, name1}));
}

TEST(TEST_readGraph, comment_first) {
    std::ifstream graph_in("test_instances/comment_first.graph");
    Graph g;
    readGraph(graph_in, g);
    ASSERT_EQ(g.vertexCount(), 2);
    ASSERT_EQ(g.edgesCount(), 2);
    ASSERT_EQ(g.multiplyEdgesCount(), 1);
    auto name1 = g.translate("1");
    auto name2 = g.translate("2");
    auto list1 = g.getAdjacentList(name1);
    auto list2 = g.getAdjacentList(name2);

    ASSERT_EQ(list1, std::unordered_multiset<int>({name2, name2}));
    ASSERT_EQ(list2, std::unordered_multiset<int>({name1, name1}));
}
