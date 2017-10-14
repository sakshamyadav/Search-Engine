// readData.c file

//builds a Graph structure for URL links

//returns graph to main function 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "readData.h"

//function to build graph
Graph buildGraph() {

    //open file for reading
    FILE *fp;
    fp = fopen("collection.txt", "r");
    char chararr[100];
    char **str = NULL;
    int numURLs = 0;
    int i,j; 
    
    //read URL names to str array
    while(fscanf(fp, "%100s", chararr) != EOF){
        str = realloc(str, (numURLs + 1)*sizeof(*str));
        str[numURLs] = malloc(strlen(chararr)+1);
        strcpy(str[numURLs++], chararr);
    }

    //close file
    fclose(fp);

    //create a new graph with number of vertices equal to number of URLs
    Graph g = newGraph(numURLs);

    //open each URL to determine it's outLinks and update the graph structure accordingly
    char temp[3000];
    for(i=0; i<numURLs; i++){
        char urlName[100], fileType[100];
        strcpy(urlName, str[i]);
        strcpy(fileType, ".txt");
        char *current = strcat(urlName,fileType);

        fp = fopen(current, "r");

        //make sure that only the outLinks of a URL are read, and not the actual contents of that 'page'
        while(fscanf(fp, "%s", temp)!=EOF){
            if(found(temp, numURLs, str)){
                int pos = position(temp, numURLs, str);
                insertEdge(g, i, pos, 1);
            }
        }

        //close file
        fclose(fp);
    }

    //remove any self-referential edges (i.e. edge from vertex v to v) - to ensure diagonal entries are 0, i.e. no URL is linked to itself
    for(i=0, j=0; i<numURLs; i++, j++){
        if(vLinkw(g,i,j)){
            removeEdge(g, i, j);
        }
    }

    //free memory associated with pointers
    free(str);

    //return graph to main function
    return g;

}

//while reading a URL file, this function will return true if it has found an outLink in the header - "section 1"
int found(char *string, int length, char **str){
    int i; 
    for(i=0; i<length; i++){
        if(strcmp(string, str[i])==0){
            return 1; 
        }
    }
    return 0; 
}

//this function will return the "position" of an outLink (it's position in the header "section 1") in order to properly build a graph
int position(char *string, int length, char **str){
    int i; 
    for(i=0; i<length; i++){
        if(strcmp(string, str[i])==0){
            return i; 
        }
    }
    return 0; 
}
