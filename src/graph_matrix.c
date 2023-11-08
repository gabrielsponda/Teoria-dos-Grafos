/******************************************************************************
 *
 * Created by: Carla Negri Lintzmayer
 *             Maycon Sambinelli
 * To be used by our students
 *
 *****************************************************************************/


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "graph.h"
#include "utils.h"

struct graph {
    int V;
    int E;
    char **adj;
};

Graph graph(int V) {
    assert(V > 0);

    // http://c-faq.com/aryptr/fn33.html
    Graph G = Malloc(sizeof(*G));
    G->V = V;
    G->E = 0;

    G->adj = Malloc(V * sizeof(*G->adj)); 
    for (Vertex u = 0; u < V; u++) {
        G->adj[u] = Malloc(V * sizeof(G->adj[u]));
        for (Vertex v = 0; v < V; v++)
            G->adj[u][v] = 0;
    }
    return G;
}

void graph_destroy(Graph G) {
    assert(G);

    for (Vertex u = 0; u < G->V; u++)
        free(G->adj[u]);

    free(G);
}

int graph_order(Graph G) {
    assert(G);
    return G->V;
}

int graph_num_edges(Graph G) {
    assert(G);
    return G->E;
}

void graph_insert_edge(Graph G, Edge e) {
    assert(G);
    assert(e.u >= 0 && e.u < G->V);
    assert(e.v >= 0 && e.v < G->V);

    if (!G->adj[e.u][e.v])
        G->E += 1;
    G->adj[e.u][e.v] = 1;
    G->adj[e.v][e.u] = 1;
}

void graph_remove_edge(Graph G, Edge e) {
    assert(G);
    assert(e.u >= 0 && e.u < G->V);
    assert(e.v >= 0 && e.v < G->V);

    if (G->adj[e.u][e.v])
        G->E -= 1;
    G->adj[e.u][e.v] = 0;
    G->adj[e.v][e.u] = 0;
}

int graph_has_edge(Graph G, Edge e) {
    assert(G);
    assert(e.u >= 0 && e.u < G->V);
    assert(e.v >= 0 && e.v < G->V);

    return G->adj[e.u][e.v];
}

int graph_edges(Graph G, Edge* edges) {
    assert(G);
    assert(edges);

    int k = 0;
    for (Vertex u = 0; u < G->V; u++)
        for (Vertex v = 0; v < G->V; v++)
            if (G->adj[u][v]) {
                edges[k] = edge(u, v);
                k += 1;
            }
    return k;
}

int graph_vertex_degree(Graph G, Vertex u) {
    assert(G);
    assert(u >= 0 && u < G->V);

    int d = 0;
    for (Vertex v = 0; v < G->V; v++)
        if (G->adj[u][v])
            d += 1;
    return d;

}

int graph_neighbors(Graph G, Vertex u, Vertex* neigh) {
    assert(G);
    assert(u >= 0 && u < G->V);
    assert(neigh);

    int k = 0;
    for (Vertex v = 0; v < G->V; v++)
        if (G->adj[u][v]) {
            neigh[k] = v;
            k += 1;
        }
    return k;
}

Graph graph_copy(Graph G) {
    assert(G);

    Graph H = graph(G->V);
    
    for (Vertex u = 0; u < G->V; u++)
        for (Vertex v = 0; v < G->V; v++)
            if (G->adj[u][v])
                graph_insert_edge(H, edge(u, v));
    return H;
}

void graph_print(Graph G) {
    assert(G);

    printf("V: %d, E: %d\n", G->V, G->E);
    for (Vertex u = 0; u < G->V; u++) {
        printf("%2d: ", u);
        for (Vertex v = 0; v < G->V; v++)
            if (G->adj[u][v])
                printf("%d, ", v);
        printf("\n");
    }
}

void graph_print_edges(Graph G) {
    assert(G);

    printf("%d %d\n", G->V, G->E);
    for (Vertex u = 0; u < G->V; u++)
        for (Vertex v = u + 1; v < G->V; v++)
            if (G->adj[u][v])
                printf("%d %d\n", u, v);
}


