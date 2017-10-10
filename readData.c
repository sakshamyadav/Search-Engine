// readData.c file
// creates a set of URLs to process by reading in data from file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "readData.h"

// implement breadth first traversal
Graph read(Graph g) {

    FILE *fp = stdin;
    fp = fopen("collection.txt", "r");
    char chararr[100];
    char **str = NULL;
    int numURLs = 0;
 
    while(fscanf(fp, "%63s", chararr) != EOF){
        str = realloc(str, (numURLs + 1)*sizeof(*str));
        str[numURLs] = malloc(strlen(chararr)+1);
        strcpy(str[numURLs++], chararr);
    }

    //Now the 'str' array contains all the URLs, we can access it normally like str[0], str[1] etc

    return 0;
 
}





