/******************************************************************************
 *
 * Created by: Carla Negri Lintzmayer
 *             Maycon Sambinelli
 * To be used by our students
 *
 *****************************************************************************/


#include<assert.h>
#include<math.h>

#include "graph.h"
#include "utils.h"

Edge edge(int u, int v) {
    Edge e = {u, v};
    return e;
}

void graph_insert_edges(Graph G, Edge* edges, int size) {
    for (int i = 0; i < size; i++)
        graph_insert_edge(G, edges[i]);
}

void graph_remove_edges(Graph G, Edge* edges, int size) {
    for (int i = 0; i < size; i++)
        graph_remove_edge(G, edges[i]);
}

Graph graph_squared(Graph G) {
    assert(G);

    int V = graph_order(G);
    Graph H = graph(V);

    // There is an edge uv in H if dist_G(u,v) <= 2 
    for (int u = 0; u < V; u++)
        for (int v = u+1; v < V; v++)
            if (graph_has_edge(G, edge(u, v))) 
                graph_insert_edge(H, edge(u, v));
            else
                for (int w = 0; w < V; w++)
                    if (graph_has_edge(G, edge(u, w)) && graph_has_edge(G, edge(v, w)))
                        graph_insert_edge(H, edge(u, v));
    return H;
}

Graph graph_GNP(int V, double p) {

    assert(V > 0);
    assert(p >= 0 && p <= 1);
    
    Graph G = graph(V);

    // Edge uv exists with probability p
    for (int u = 0; u < V; u++)
        for (int v = u+1; v < V; v++)
            if (rand_double() <= p)
                graph_insert_edge(G, edge(u, v));
    return G;
}

Graph graph_euclidean_neighborhood(int V, double dist) {

    assert(V > 0);
    assert(dist >= 0 && dist <= 1);

    Graph G = graph(V);

    // Vertices are points in the plane with random coordinates
    Point *pos = Malloc(V * sizeof(*pos));
    for (int u = 0; u < V; u++) {
        pos[u].x = rand_double();
        pos[u].y = rand_double();
    }

    // There is an edge uv if euclidean_distance(u,v) <= dist
    for (int u = 0; u < V; u++)
        for (int v = u + 1; v < V; v++) {
            double d = sqrt((pos[u].x - pos[v].x) * (pos[u].x - pos[v].x)
                            +(pos[u].y - pos[v].y) * (pos[u].y - pos[v].y));

            if (d <= dist)
                graph_insert_edge(G, edge(u, v));
        }
    return G;
}