/******************************************************************************
 *
 * Created by: Carla Negri Lintzmayer
 *             Maycon Sambinelli
 * To be used by our students
 *
 *****************************************************************************/


#ifndef __GRAPH_H_
#define __GRAPH_H_

typedef int Vertex;
typedef struct {int u; int v;} Edge;
typedef struct graph* Graph;

// Edge and graph constructors
Edge edge(Vertex, Vertex);
Graph graph(int);

// Deallocates the memory used by a graph
void graph_destroy(Graph);

// Return number of vertices of a graph
int graph_order(Graph);
// Return number of edges of a graph
int graph_num_edges(Graph);

// Inserts an edge on a graph
void graph_insert_edge(Graph, Edge);
/**
 * Inserts several edges in a graph.
 * 
 * @param Graph the input graph
 * @param Edge* an array of edges
 * @param int size of the array of edges
 */
void graph_insert_edges(Graph, Edge*, int);
// Removes an edge from a graph
void graph_remove_edge(Graph, Edge);
/**
 * Removes several edges from a graph.
 * 
 * @param Graph the input graph
 * @param Edge* an array of edges
 * @param int size of the array of edges
 */
void graph_remove_edges(Graph, Edge*, int);

// Tests whether an edge exists on a graph
int graph_has_edge(Graph, Edge);

/**
 * Returns all edges of a graph.
 * 
 * @param Graph the input graph
 * @param Edge* an allocated array to be filled with the edges
 * @return int the amount of edges in the graph
 */
int graph_edges(Graph, Edge*);

// Returns the degree of a vertex
int graph_vertex_degree(Graph, Vertex);
/**
 * Returns all neighbors of a vertex
 * 
 * @param Graph the input graph
 * @param Vertex the input vertex
 * @param Vertex* an allocated array to be filled with the neighbors of the input vertex
 * @return int the amount of neighbors of the input vertex
 */
int graph_neighbors(Graph, Vertex, Vertex*);

// Returns a copy of a graph
Graph graph_copy(Graph);

// Prints a graph
void graph_print(Graph);
// Prints the edges of a graph
void graph_print_edges(Graph);

// Returns the square of a graph
Graph graph_squared(Graph);

// Returns a random graph using the model G(n,p)
Graph graph_GNP(int, double);
// Returns a random graph using the model Euclidean Neighbor
Graph graph_euclidean_neighbor(int, double);


#endif // __GRAPH_H_
