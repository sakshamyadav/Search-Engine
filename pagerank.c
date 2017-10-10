// pagerank.c file

// program will:
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

// Created 10/10/2017
// last modified: 10/10/2017

// COMP2521 Assignment 2
// Group: Ctrl+Alt+Elit

#include <sdtio.h>
#include <stdlib.h>
#include <assert.h>

PageRankW(int d, int diffPR, int maxIterations);

int main(int argc, char *argv[]) {
	// read in each line of the file
	// discern URLs from each other
	// store URLs in an array
	FILE *in;
	
	// access URLs and build graph
	
	// pageRank
	
	return 0;
}

int PageRankW(Graph g, Vertex v, int d, int diffPR, int maxIterations) {
	// algorithm
}
