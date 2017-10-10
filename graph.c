// graph.c adt implementation
// implementing from scratch

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"

typedef struct GraphRep {
    int nV;
    int nE;
    char **vertex;
    int **edges;
} GraphRep;

Graph newGraph();
void disposeGraph(Graph g);
void showGraph(Graph g);
int addEdge(Graph g);
int nVertices(Graph g);

// create new empty graph
Graph newGraph() {
    Graph new = malloc(sizeof(Graph));
    if (new == NULL) {
        fprintf(stderr, "Error! Cannot create new graph, out of memory!\n");
        exit(1);
    }
    new->nV = 0;
    new->nE = 0;
}

// delete/ free graph
void disposeGraph(Graph g) {

}

// add edges to graph

int addEdge(Graph g) {

}
// count amount of vertices, returns int
int nVertices(Graph g) {
    assert(g != NULL);
    return (g->nV);
}