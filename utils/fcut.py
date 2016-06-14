#!/usr/bin/env python3
from sys import argv, stdin

if __name__ == "__main__":
    deletedVertices = {int(v) for v in argv[1:]}
    for row in stdin:
        b, e = map(int, row.split())
        if b not in deletedVertices and e not in deletedVertices:
            print(b, e)