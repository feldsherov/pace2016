#!/usr/bin/env python3
from sys import stdin

def read_graph(inp):
    graph = dict()
    for row in inp:
        b, e = map(int, row.split())
        if b not in graph:
            graph[b] = dict()
        if e not in graph:
            graph[e] = dict()

        if e not in graph[b]:
            graph[b][e] = 0
        if b not in graph[e]:
            graph[e][b] = 0

        graph[b][e] += 1

        if b != e:
            graph[e][b] += 1

    return graph

def triangles_count(graph):
    ans = 0
    for v1 in graph:
        for v2 in graph[v1]:
            for v3 in graph:
                if v1 in graph[v3] and v2 in graph[v3]:
                    ans += 1
    return ans // 6

def linkpoints_count(graph):
    ans = 0
    for v1 in graph:
        if len(graph[v1]) == 2:
            ans += 1
    return ans

def leafs_count(graph):
    ans = 0
    for v1 in graph:
        if len(graph[v1]) == 1:
            ans += 1
    return ans

def multiply_edge_count(graph):
    ans = 0
    for v1 in graph:
        for v2 in graph[v1]:
            if graph[v1][v2] > 1:
                ans += 1
    return ans // 2

def edges_count(graph):
    ans = 0
    for v1 in g:
        for v2 in g[v1]:
            ans += g[v1][v2]
            if v1 == v2:
                ans += g[v1][v2]
    return ans // 2

if __name__ == "__main__":
    g = read_graph(stdin)
    print("Count of vertices: ", len(g))
    print("Count of edges: ", edges_count(g))
    print("Count of triangles: ", triangles_count(g))
    print("Count of linkpoints: ", linkpoints_count(g))
    print("Count of leafs: ", leafs_count(g))
    print("Count of multiply edges: ", multiply_edge_count(g))
