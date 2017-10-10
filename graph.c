// Graph ADT file implementation
// Created 9/10/2017
// last modified: 9/10/2017

// COMP2521 Assignment 2
// Group: Ctrl+Alt+Elite 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

typedef struct GraphRep {
	int nV; // number of vertices
	int nE; // number of edges
} GraphRep;

// create new graph
Graph newGraph() {
	
}

// create new node or is it add new edge
void addVertex(Graph g, Vertex v) {

}

// dispose graph
void disposeGraph(Graph g) {
	if (g == NULL) {
		return;
    }
	int i;
	// change this
	for (i = 0; i < g->nV; i++) {
		free(g->vertex[i]);
	}
	for (i = 0; i < g->maxV; i++) {
		free(g->edges[i]);
	}
	free();
}

// show print out graph
void showGraph(Graph g) {
    assert(g! = NULL);
}
