// pagerank.h file

// function prototypes

#ifndef PAGE_H
#define PAGE_H

double *calculatePageRank(Graph g, double d, double diffPR, int maxIt);
void sort(double *pr, char **str, int out[], int length);
void writeToText(double *pr, char **str, int out[], int length);

#endif
