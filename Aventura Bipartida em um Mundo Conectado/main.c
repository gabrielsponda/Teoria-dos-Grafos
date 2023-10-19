/******************************************************************************
 *
 * Nome: Gabriel Sponda Freitas Bettarello
 * RA: 11201932580
 *
 *****************************************************************************/

#include "graph.h"
#include <stdio.h>

Vertex u = -1, v = -1;

int DFS_cycle(Graph G, Vertex s, int visitado[], int tempo, int ordem[],
              int predecessor[], int biparticao[]) {
  visitado[s] = 1;
  tempo = tempo + 1;
  ordem[s] = tempo;
  Vertex vizinhos[graph_order(G)];
  int n = graph_neighbors(G, s, vizinhos);
  for (int i = 0; i < n; i++) {
    if (visitado[vizinhos[i]] == 0) {
      predecessor[vizinhos[i]] = s;
      biparticao[vizinhos[i]] = !biparticao[s];
      DFS_cycle(G, vizinhos[i], visitado, tempo, ordem, predecessor,
                biparticao);
    } else if (predecessor[s] != vizinhos[i] &&
               biparticao[s] == biparticao[vizinhos[i]]) {
      if (u == -1) {
        u = s;
        v = vizinhos[i];
      }
      return 1;
    }
  }
  return 0;
}

int main() {

  // Lê o número de vértices e arestas
  int V, E;
  scanf("%d %d", &V, &E);

  // Verifica se o número de vértices e arestas são válidos
  if (V < 1 || V > 1000 || E < 0 || E > V * (V - 1) / 2) {
    printf("Invalid number of vertices or edges!\n");
    return 0;
  }

  // Cria um grafo
  Graph G = graph(V);

  // Lê e insere as arestas
  for (int i = 0; i < E; i++) {

    int x, y;
    scanf("%d %d", &x, &y);

    // Verifica se a aresta é válida
    if (x < 0 || y < 0 || x > V - 1 || y > V - 1) {
      printf("Invalid edge!\n");
      return 0;
    }

    graph_insert_edge(G, edge(x, y));
  }

  int visitado[V];
  int tempo = 0;
  int ordem[V];
  int predecessor[V];
  int biparticao[V];

  for (int i = 0; i < V; i++) {
    visitado[i] = 0;
    ordem[i] = 0;
    predecessor[i] = -1;
    biparticao[i] = -1;
  }

  predecessor[0] = 0;
  biparticao[0] = 0;
  int ciclo = DFS_cycle(G, 0, visitado, tempo, ordem, predecessor, biparticao);

  if (ciclo == 0) {
    printf("PAZ!\n");
    for (int i = 0; i < V; i++) {
      if (biparticao[i] == 0) {
        printf("%d ", i);
      }
    }
    printf("\n");
    for (int i = 0; i < V; i++) {
      if (biparticao[i] == 1) {
        printf("%d ", i);
      }
    }
  } else {
    printf("GUERRA!\n");
  }

  while (u != 0) {
    printf("%d ", u);
    u = predecessor[u];
  }
  do {
    printf("%d ", v);
    v = predecessor[v];
  } while (v != 0);

  getchar();
  printf("\nPress any key to continue...");
  getchar();

  return 0;
}