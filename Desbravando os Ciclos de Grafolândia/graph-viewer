#!/bin/env python


#-------------------------------------------------------------------------------
#
#                                Dependencies
#
#-------------------------------------------------------------------------------

# This script requires the following python packages

# - mathplotlib
# - networkx

# to install theses dependencies on Linux/BSD/MACOS, the easiest way is to use
# pip:
#
# pip install mathplotlib
# pip install networkx
#
# On windows, the best way is by installing Anaconda

#-------------------------------------------------------------------------------
#
#                                   USAGE
#
#-------------------------------------------------------------------------------

# python graph-viewer  -h                                                                                                                                                                                 [13:54:26]
# usage: graph-viewer [-h] [--oriented] [--weighted] [-o filepath] [--layout style] filepath
#
# Naive Graph Viewer
#
# positional arguments:
#   filepath        File path to the input file
#
# optional arguments:
#   -h, --help      show this help message and exit
#   --oriented      The input graph is an oriented graph (digraph).
#   --weighted      The input graph is a weighted graph.
#   -o filepath     Specify the name of the output file (with the extension) in which the graph plot should be written.
#   --layout style  Specify the layout algorithm used to draw the graph. Accepted values are: circular, planar, spring (default), spectral, shell.


import matplotlib.pyplot as plt
import networkx as nx
import argparse
import sys
import os

def iostream(fpath):

    if not os.path.exists(fpath):
        raise Exception("ERROR: {} is not a valid file".format(fpath))

    return open(fpath, "r")

def create_parser():

    parser = argparse.ArgumentParser(description='Naive Graph Viewer')

    parser.add_argument('input',
                        metavar='filepath',
                        action='store',
                        type=iostream,
                        help='File path to the input file')

    parser.add_argument('--oriented',
                        action="store_true",
                        help='The input graph is an oriented graph (digraph).')

    parser.add_argument('--weighted',
                        action="store_true",
                        help='The input graph is a weighted graph.')

    parser.add_argument('-o',
                        metavar="filepath",
                        dest='output',
                        action="store",
                        help="Specify the name of the output file (with the extension) in which the graph plot should be written.")

    parser.add_argument('--layout',
                        metavar="style",
                        action="store",
                        help="Specify the layout algorithm used to draw the graph. Accepted values are: circular, planar, spring (default), spectral, shell.")
    return parser

if __name__ == "__main__":

    parser = create_parser()
    args = parser.parse_args()

    V, E = map(int, args.input.readline().split())
    G = None

    if args.oriented:
        G = nx.DiGraph()
    else:
        G = nx.Graph()

    G.add_nodes_from(range(V))

    for i in range(E):
        line = args.input.readline().split()
        u, v = map(int, line[:2])
        if args.weighted:
            w = float(line[2])
            G.add_edge(u, v, weight=w)
        else:
            G.add_edge(u, v)

    pos = None
    if args.layout == 'circular':
        pos = nx.circular_layout(G)
    elif args.layout == 'planar':
        if not nx.check_planarity(G)[0]:
            print("ERROR: input (di)graph is non-planar.")
            sys.exit(1)
        pos = nx.planar_layout(G)
    elif args.layout == 'spring':
        pos = nx.spring_layout(G)
    elif args.layout == 'spectral':
        pos = nx.spectral_layout(G)
    elif args.layout == 'shell':
        pos = nx.shell_layout(G)
    else:
        pos = nx.spring_layout(G)

    nx.draw_networkx(G, pos)
    if args.weighted:
        labels = nx.get_edge_attributes(G, 'weight')
        nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)

    if args.output:
        plt.savefig(args.output)
    else:
        plt.show()