/******************************************************************************
 *
 * Nome: Gabriel Sponda Freitas Bettarello
 * RA: 11201932580
 *
 *****************************************************************************/

/******************************************************************************
 *
 * Observação: até a linha 121 os códigos foram retirados dos código-fonte
 * disponibilizados (graph_adjacency_list.c, graph.c/graph.h e utils.c).
 * O código desenvolvido para resolver o problema é a partir da linha 131.
 *
 ******************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void *Malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr)
    abort();
  return ptr;
}

typedef int Vertex;
typedef struct {
  int u;
  int v;
} Edge;
typedef struct graph *Graph;

Edge edge(int u, int v) {
  Edge e = {u, v};
  return e;
}

typedef struct node *link;

struct node {
  Vertex w;
  link next;
};

link list_insert(link head, Vertex w) {
  link p = Malloc(sizeof(*p));
  p->w = w;
  p->next = head;
  return p;
}

link list_remove(link head, Vertex w) {
  if (head == NULL)
    return NULL;

  if (head->w == w) {
    link p = head->next;
    free(head);
    return p;
  } else {
    head->next = list_remove(head->next, w);
    return head;
  }
}

link list_destroy(link head) {
  if (head) {
    list_destroy(head->next);
    free(head);
  }
  return NULL;
}

struct graph {
  int V;
  int E;
  link *adj;
};

Graph graph(int V) {
  assert(V > 0);

  // http://c-faq.com/aryptr/fn33.html
  Graph G = Malloc(sizeof(*G));

  G->V = V;
  G->E = 0;
  G->adj = Malloc(V * sizeof(link));

  for (Vertex u = 0; u < V; u++)
    G->adj[u] = NULL;

  return G;
}

int graph_order(Graph G) {
  assert(G);
  return G->V;
}

void graph_insert_edge(Graph G, Edge e) {
  assert(G);
  assert(e.u >= 0 && e.u < G->V);
  assert(e.v >= 0 && e.v < G->V);

  G->adj[e.u] = list_insert(G->adj[e.u], e.v);
  G->adj[e.v] = list_insert(G->adj[e.v], e.u);
  G->E += 1;
}

int graph_neighbors(Graph G, Vertex u, Vertex *neigh) {
  assert(G);
  assert(u >= 0 && u < G->V);
  assert(neigh);

  int k = 0;
  for (link p = G->adj[u]; p != NULL; p = p->next) {
    neigh[k] = p->w;
    k += 1;
  }
  return k;
}

/******************************************************************************
 *
 * Funções: DFS_odd_cycle e main
 *
 ******************************************************************************/

// Variáveis globais para armazenar o "início" e "fim" do ciclo ímpar
Vertex u = -1, v = -1;

// Por se tratar de uma DFS, só alguns comentários foram feitos
int DFS_odd_cycle(Graph G, Vertex s, int visitado[], int predecessor[],
                  int biparticao[]) {
  visitado[s] = 1;
  link vizinho = G->adj[s];
  while (vizinho != NULL) {
    Vertex p = vizinho->w;
    if (visitado[p] == 0) {
      predecessor[p] = s;
      biparticao[p] = !biparticao[s];
      if (DFS_odd_cycle(G, p, visitado, predecessor, biparticao)) {
        return 1;
      }

      // Se o vértice já foi visitado e não é o predecessor, então existe um
      // ciclo Se a bipartição do vértice atual e do vizinho forem iguais, ciclo
      // ímpar.
    } else if (predecessor[s] != p && biparticao[s] == biparticao[p]) {

      // Assim que o ciclo ímpar é detectado, os vértices são armazenados
      // Podem até existir outros ciclos ímpares, mas apenas um é necessário
      if (u == -1) {
        u = s;
        v = p;
      }
      return 1;
    }
    vizinho = vizinho->next;
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
  int predecessor[V];
  int biparticao[V];

  // Incializa os vetores
  for (int i = 0; i < V; i++) {
    visitado[i] = 0;
    predecessor[i] = -1;
    biparticao[i] = -1;
  }

  // Chama a DFS
  predecessor[0] = 0;
  biparticao[0] = 0;
  int ciclo = DFS_odd_cycle(G, 0, visitado, predecessor, biparticao);

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
    while (u != v) {
      printf("%d ", u);
      u = predecessor[u];
    }
    printf("%d ", u);
  }

  return 0;
}