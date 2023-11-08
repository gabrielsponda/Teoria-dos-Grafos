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
#include <string.h>

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
 * Implementação de uma pilha e do algoritmo de Tarjan para encontrar
 * componentes fortemente conectados
 *
 ******************************************************************************/

typedef struct stack *Stack;

struct stack {
  int top;
  int capacity;
  int *array;
};

Stack createStack(int capacity) {
  Stack stack = Malloc(sizeof(*stack));
  stack->capacity = capacity;
  stack->top = -1;
  stack->array = Malloc(stack->capacity * sizeof(int));
  return stack;
}

int isFull(Stack stack) { return stack->top == stack->capacity - 1; }

int isEmpty(Stack stack) { return stack->top == -1; }

void push(Stack stack, int item) {
  if (isFull(stack))
    return;
  stack->array[++stack->top] = item;
}

int pop(Stack stack) {
  if (isEmpty(stack))
    return -1;
  return stack->array[stack->top--];
}

int peek(Stack stack) {
  if (isEmpty(stack))
    return -1;
  return stack->array[stack->top];
}

int min(int a, int b) { return (a < b) ? a : b; }

void strongconnect(Graph G, Vertex v, int *order, int *low, int *visited,
                   Stack S, int *index, int **sccs, int *scc_sizes,
                   int *scc_count) {
  order[v] = *index;
  low[v] = *index;
  (*index)++;
  push(S, v);
  visited[v] = 1;

  for (link p = G->adj[v]; p != NULL; p = p->next) {
    Vertex w = p->w;
    if (order[w] == -1) {
      strongconnect(G, w, order, low, visited, S, index, sccs, scc_sizes,
                    scc_count);
      low[v] = min(low[v], low[w]);
    } else if (visited[w]) {
      low[v] = min(low[v], order[w]);
    }
  }

  if (low[v] == order[v]) {
    sccs[*scc_count] = malloc(G->V * sizeof(int));
    scc_sizes[*scc_count] = 0;
    Vertex w;
    do {
      w = pop(S);
      visited[w] = 0;
      sccs[*scc_count][scc_sizes[*scc_count]] = w;
      scc_sizes[*scc_count]++;
    } while (w != v);
    (*scc_count)++;
  }
}

int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void tarjan(Graph G) {
  int V = G->V;
  int *order = malloc(V * sizeof(int));
  int *low = malloc(V * sizeof(int));
  int *visited = malloc(V * sizeof(int));
  Stack S = createStack(V);
  int index = 0;

  for (Vertex v = 0; v < V; v++) {
    order[v] = -1;
    low[v] = 0;
    visited[v] = 0;
  }

  int **sccs = malloc(V * sizeof(int *));
  int *scc_sizes = malloc(V * sizeof(int));
  int scc_count = 0;

  for (Vertex v = 0; v < V; v++) {
    if (order[v] == -1) {
      strongconnect(G, v, order, low, visited, S, &index, sccs, scc_sizes,
                    &scc_count);
    }
  }
  if (scc_count == 1)
    printf("Ha 1 regiao altamente conectada:\n");
  else
    printf("Ha %d regioes altamente conectadas:\n", scc_count);
  for (int i = 0; i < scc_count; i++) {
    qsort(sccs[i], scc_sizes[i], sizeof(int), compare);
    //    printf("");
    for (int j = 0; j < scc_sizes[i]; j++) {
      printf("%d ", sccs[i][j]);
    }
    printf("\n");
    free(sccs[i]);
  }

  free(sccs);
  free(scc_sizes);
  free(order);
  free(low);
  free(visited);
  free(S);
}

int main() {

  // Lê o número de vértices e arestas
  int V, E;
  scanf("%d %d", &V, &E);

  // Verifica se o número de vértices e arestas são válidos
  if (V < 1 || V > 1000 || E < 0 || E > V * (V - 1)) {
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

  tarjan(G);

  return 0;
}