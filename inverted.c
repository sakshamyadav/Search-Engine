// inverted.c 

// reads data from collection.txt
// generates an index of all words seen in the "URLs"
// provides a sorted list (a-z) of each word and in which files they appear in (no duplicates)
// urls names in ascending order

// output in invertedIndex.txt

// need to first normalise words
// removing leading and trailing spaces,
// convert all characters to lowercase,
// remove punctuation marks if at the end of a word:
// '.' (dot), ',' (comma), ';' (semicolon), ? (question mark)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "inverted.h"

#define MAX_LENGTH 100

int main(int argc, char *argv[]) {
    // open file
    FILE *collection = stdin;
    collection = fopen("collection.txt", "r");
    
    // read in each "URL", mostly copied from pagerank.c, could try to store in char array
    char string[MAX_LENGTH];
    char **urlArray = NULL;
    int numURLs = 0;
    while (fscanf(collection, "%s", string) != EOF) { // scan in each url
        // store each URL in char array
        // printf("%s\n", string);
        urlArray = realloc(urlArray, (numURLs + 1)*sizeof(*urlArray));
        urlArray[numURLs] = malloc(strlen(string)+1);
        strcpy(urlArray[numURLs++], string);
    }
    fclose(collection); // close collection.txt
    for (int i = 0; i<numURLs; i++) { // checks
        // printf("%d %s\n", i, urlArray[i]);
    }
    printf("total: %d\n", numURLs);

    // going through each file and building a graph of words
    FILE *url;
    // char temp[3000];
    int nWords = 0;
    Graph wGraph = newGraph(1);
    int read;
    char word[MAX_LENGTH];
    char **wordArray = NULL;
    for(int i=0; i<numURLs; i++){
        char *currentURL = strcat(urlArray[i], ".txt");
        url = fopen(currentURL, "r");
        // printf("%s\n", currentURL);
        // find words, jump to section 2
        read = 0; // reset read for each file opened
        while (fscanf(url, "%s", word) != EOF) {
            // from Section-2 to #end
            if (strcmp(word, "#end") == 0) { // if word is #end, don't read
                read = 0;
            }
            if (read == 1) { // read only when 1, i.e. after 1st Section-2 is seen
                // printf("%s\n", word);
                // nWords++;
                if (duplicates(wordArray, word, nWords) != 1) { // not duplicated
                    printf("%s\n", word);
                    wordArray = realloc(wordArray, (nWords + 1)*sizeof(*wordArray));
                    wordArray[nWords] = malloc(strlen(word)+1);
                    strcpy(wordArray[nWords++], word);
                }
                /*urlArray = realloc(urlArray, (numURLs + 1)*sizeof(*urlArray));
                urlArray[numURLs] = malloc(strlen(string)+1);
                strcpy(urlArray[numURLs++], string);*/
            }
            if (strcmp(word, "Section-2") == 0) { // read = 1 only after 1st Section-2, for second read = 2
                read++;
            }
        }
        //make sure that only the outLinks of a URL are read, and not the actual contents of that 'page'
        /*while(fscanf(fp, "%s", temp)!=EOF){
            if(found(temp, numURLs, str)){
                int pos = position(temp, numURLs, urlArray);
                insertEdge(index, i, pos, TRUE);
            }
        }
        */
        
        //close file
        fclose(url);
    }
    printf("%d\n", nWords);
    showGraph(wGraph);

    printf("Create File\n");


    // output 
    FILE *new;
    new = fopen("invertedIndex.txt", "w");
    // print graph function
    fclose(new);
    return 0;
}

// check for if the current string is already in the string array
// return 0 if not found, else 1
int duplicates(char **str, char *in, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp(str[i], in) == 0) {
            return 1;
        }
    }
    return 0;
}
// normalise words function
// removes leading and trailing spaces,
// convert all chars to lowercase,
// remove punctuation marks at end of words
// '.', ',', ';', ?

/*void normalise(char *in, char *out) {
    char *c;
    if (tolower()
    strcpy(out,in);
}*/

// mars  url25 url31 url101