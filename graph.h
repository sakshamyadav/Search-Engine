// graph.h

#ifndef GRAPH_H
#define GRAPH_H

typedef struct GraphRep *Graph;

Graph newGraph();
void disposeGraph(Graph g);
void showGraph(Graph g);
int addEdge(Graph g);
int nVertices(Graph g);

#endif