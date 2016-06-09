#!/usr/bin/env python3
from optparse import OptionParser
from sys import stdin

import graphviz as gv

from fproperties import read_graph


def vizualize(g, outp_filename):
    gv_graph = gv.Graph()

    for vertex in g:
        gv_graph.node(str(vertex))

    for v1 in g:
        for v2 in g[v1]:
            if v1 <= v2:
                for _ in range(g[v1][v2]):
                    gv_graph.edge(str(v1), str(v2))

    outp_filename = gv_graph.render(outp_filename)
    print ("I have saved vizualization in file {}".format(outp_filename))


if __name__ == "__main__":
    parser = OptionParser("usage: %prog filename")
    options, args = parser.parse_args()

    if len(args) < 1:
        print ("Required one argument: output filename")
        exit()

    output_filename = args[0]

    g = read_graph(stdin)
    vizualize(g, output_filename)
