/******************************************************************************
 *
 * Nome: Gabriel Sponda Freitas Bettarello
 * RA: 11201932580
 *
 *****************************************************************************/

/******************************************************************************
 *
 * Observação: até a linha 126 os códigos foram retirados dos código-fonte
 * disponibilizados (graph_adjacency_list.c, graph.c/graph.h e utils.c).
 * O código desenvolvido para resolver o problema é a partir da linha 131.
 *
 ******************************************************************************/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Vertex;

typedef struct node *link;

typedef struct {
  int u;
  int v;
} Edge;

typedef struct graph *Graph;

struct node {
  Vertex w;
  link next;
};

struct graph {
  int V;
  int E;
  link *adj;
};

void *Malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr)
    abort();
  return ptr;
}

Edge edge(int u, int v) {
  Edge e = {u, v};
  return e;
}

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
 * Implementação de uma fila de prioridade mínima usando heap binário;
 * Implementação do algoritmo de Prim para encontrar a árvore geradora mínima
 *
 * Partes desse código foram retiradas de um slide e complementadas
 *
 ******************************************************************************/

typedef struct priority_queue *PQ;

typedef struct {
  int label;
  double prio;
} Elem;

struct priority_queue {
  Elem *queue;
  int *ind;
  int size;
  int capacity;
};

int parent(int ind) { return floor(ind / 2.0); }

int left_children(int ind) { return 2 * ind; }

int right_children(int ind) { return 2 * ind + 1; }

PQ pq(int max_size) {
  PQ pq = Malloc(sizeof(*pq));
  pq->size = 0;
  pq->capacity = max_size;
  pq->queue = Malloc((max_size + 1) * sizeof(*pq->queue));
  pq->ind = Malloc(max_size * sizeof(*pq->ind));
  for (int i = 0; i < max_size; i++) {
    pq->ind[i] = -1;
  }
  return pq;
}

void pq_destroy(PQ pq) {
  free(pq->queue);
  free(pq->ind);
  free(pq);
}

int pq_size(PQ pq) { return pq->size; }

void pq_insert(PQ pq, int label, double prio) {
  if (pq->size == pq->capacity) {
    printf("Priority queue is full!\n");
    return;
  }
  pq->size += 1;
  pq->queue[pq->size].label = label;
  pq->queue[pq->size].prio = prio;
  pq->ind[label] = pq->size;
  int i = pq->size;
  while (i > 1 && pq->queue[parent(i)].prio > pq->queue[i].prio) {
    Elem aux = pq->queue[i];
    pq->queue[i] = pq->queue[parent(i)];
    pq->queue[parent(i)] = aux;
    pq->ind[pq->queue[i].label] = i;
    pq->ind[pq->queue[parent(i)].label] = parent(i);
    i = parent(i);
  }
}

int pq_contains(PQ pq, int label) {
  if (pq->ind[label] == -1) {
    return 0;
  }
  return 1;
}

void pq_heapify(PQ pq, int i) {
  int l = left_children(i);
  int r = right_children(i);
  int smallest;
  if (l <= pq->size && pq->queue[l].prio < pq->queue[i].prio) {
    smallest = l;
  } else {
    smallest = i;
  }
  if (r <= pq->size && pq->queue[r].prio < pq->queue[smallest].prio) {
    smallest = r;
  }
  if (smallest != i) {
    Elem aux = pq->queue[i];
    pq->queue[i] = pq->queue[smallest];
    pq->queue[smallest] = aux;
    pq->ind[pq->queue[i].label] = i;
    pq->ind[pq->queue[smallest].label] = smallest;
    pq_heapify(pq, smallest);
  }
}

int pq_extract(PQ pq) {
  if (pq->size == 0) {
    return -1;
  }
  int min = pq->queue[1].label;
  pq->queue[1] = pq->queue[pq->size];
  pq->ind[pq->queue[1].label] = 1;
  pq->ind[min] = -1;
  pq->size -= 1;
  pq_heapify(pq, 1);
  return min;
}

void pq_change(PQ pq, int label, double new_prio) {
  int i = pq->ind[label];
  double old_prio = pq->queue[i].prio;
  pq->queue[i].prio = new_prio;
  if (new_prio < old_prio) {
    while (i > 1 && pq->queue[parent(i)].prio > pq->queue[i].prio) {
      Elem aux = pq->queue[i];
      pq->queue[i] = pq->queue[parent(i)];
      pq->queue[parent(i)] = aux;
      pq->ind[pq->queue[i].label] = i;
      pq->ind[pq->queue[parent(i)].label] = parent(i);
      i = parent(i);
    }
  } else if (new_prio > old_prio) {
    pq_heapify(pq, i);
  }
}

void prim(Graph G, double peso[][G->V]) {
  int visitado[G->V];
  int pred[G->V];
  PQ heap = pq(G->V);
  double total_weight = 0;
  for (int i = 0; i < G->V; i++) {
    visitado[i] = 0;
    pred[i] = -1;
  }
  pq_insert(heap, 0, 0);
  for (int i = 1; i < G->V; i++) {
    pq_insert(heap, i, 501);
  }
  while (pq_size(heap) > 0) {
    int u = pq_extract(heap);
    visitado[u] = 1;
    if (pred[u] != -1) {
      total_weight += peso[pred[u]][u];
    }
    for (link p = G->adj[u]; p != NULL; p = p->next) {
      int v = p->w;
      if (visitado[v] == 0 && pq_contains(heap, v) &&
          peso[u][v] < heap->queue[heap->ind[v]].prio) {
        pred[v] = u;
        pq_change(heap, v, peso[u][v]);
      }
    }
  }
  printf("%.2lf\n", total_weight);
  for (int i = 1; i < G->V; i++) {
    if (pred[i] != -1) {
      printf("%d %d\n", pred[i], i);
    }
  }
  pq_destroy(heap);
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

  // Cria uma matriz de pesos
  double peso[V][V];

  // Inicializa a matriz de pesos
  for (int i = 0; i < V; i++) {
    for (int j = 0; j < V; j++) {
      peso[i][j] = 501;
    }
  }

  // Lê e insere as arestas
  for (int i = 0; i < E; i++) {

    int x, y;
    double w;
    scanf("%d %d %lf", &x, &y, &w);

    // Verifica se a aresta é válida
    if (x < 0 || y < 0 || x > V - 1 || y > V - 1 || w < 0 || w > 500) {
      printf("Invalid edge!\n");
      return 0;
    }

    graph_insert_edge(G, edge(x, y));
    peso[x][y] = w;
    peso[y][x] = w;
  }

  prim(G, peso);

  return 0;
}