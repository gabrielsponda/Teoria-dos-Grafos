#include "graph.h"
#include <stdio.h>


int main(int argc, char const *argv[]) {

  int V, E;
  scanf("%d %d", &V, &E);

  Graph G = graph(V);

  for (int i = 0; i < E; i++) {
    Vertex u, v;
    scanf("%d %d", &u, &v);
    graph_insert_edge(G, edge(u, v));
  }

  Graph G2 = graph_squared(G);
  graph_print_edges(G2);

  return 0;
}