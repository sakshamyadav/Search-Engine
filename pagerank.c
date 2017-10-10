// pagerank.c file

// reads data from a txt file 
// builds graph structure with connections between files
// adds page weight rank to each vertex of the graph using algorithm

// takes in 3 arguments:
//  d - dampening factor
//  diffPR - difference in page rank sum
//  maxiterations - max iterations

// output in pagerankList.txt file:
//  url name: list of urls in descending order of page rank (lowest to highest) in "%.7f"
//  number of links each page has
//  each value separated by a comma

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "readData.h"
//#include "readData.h"

double calculatePageRank(Graph g, int d, int diffPR, int maxIt);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s dampener diffPRsum maxIterations\n", argv[0]);
        exit(1);
    }
    // for algo
    int dampener = atoi(argv[1]);
    int diffPRsum = atoi(argv[2]);
    int maxIt = atoi(argv[3]);
    // read in the file
    Graph g = newGraph();
    read(g);

    // call readData.c function to open files, returns g


    return 0;
}

double calculatePageRank(Graph g, int d, int diffPR, int maxIt) {
    return 0;
}