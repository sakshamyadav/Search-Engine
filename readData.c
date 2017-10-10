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

    FILE *input;
    if ((input = fopen("collection.txt","r")) == NULL) {
        fprintf(stderr, "Couldn't open file: collection.txt");
    }

    int nURLs =0;
    int counter = 0;
    char *str = malloc(sizeof(char *));
    while(!feof(input) && !ferror(input)){
        fscanf(input, "%s", str);
        printf("%c \n", str[counter]);
        nURLs++;
        counter++;
    }
    fclose(input);

    printf("%c", str[0]);





















    /*while (c != EOF) {
        if (c != ' ' || c != ) {

        }
        putchar(c);
        printf("\n");
        c = fgetc(input);
    }
    printf("")*/
    /*while(fgets(name,21, input) != NULL) {
        name[strlen(name)-1] = '\0';
        url[nURLs] = strdup(name);
        while (counter < 21) {
            if (name[counter] == ' ' || name[counter])
        }
    }*/
    // if (!= ' ' || != '\n' || != '\t')
    
    printf("%d\n", nURLs);
    /*for(i = 0; i<nURLs; i++) {
        printf("%s\n", url[i]);
    }*/
}

    /*fscanf(data,"%d",&distance) == 1) {
		fromCity = n / ncities;
		toCity = n % ncities;
		// convert miles to km
		distance = distance * 100 * 1.609344;
		insertEdge(world, toCity, fromCity, distance);
		n++;
	}
	fclose(data);*/