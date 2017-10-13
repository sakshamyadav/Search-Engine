// Graph.h ... interface to Graph ADT
// This file was initially taken from course material, however it has been heavily modified to include many task-specific functions

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

// graph representation is hidden
typedef struct GraphRep *Graph;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int   validV(Graph,Vertex); //validity check

// edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex w; } Edge;
void insertEdge(Graph, Vertex, Vertex, int);
void removeEdge(Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph(int nV);
void disposeGraph(Graph g);
Graph makeGraph(int, int**);
void showGraph(Graph);
int  findPath(Graph, Vertex, Vertex, int, int *);
int getnumV(Graph g);
int vLinkw(Graph g, Vertex v, Vertex w);
double numInLinks(Graph g, Vertex v);
double numOutLinks(Graph g, Vertex v);
double sumInLinks(Graph g, Vertex v);
double sumOutLinks(Graph g, Vertex v);

#endif
