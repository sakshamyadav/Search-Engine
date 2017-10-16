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
#include <ctype.h>
#include <assert.h>
#include "graph.h"
#include "readData.h"
#include "inverted.h"
//include inverted.h, readData.h, graph.h

#define MAX_LENGTH 100

//main function
int main(void) {
    // open collection.txt for reading
    FILE *collection;
    collection = fopen("collection.txt", "r");
    
    //store url names in urlArray
    char string[MAX_LENGTH];
    char **urlArray = NULL;
    int numURLs = 0;
    while (fscanf(collection, "%s", string) != EOF) { // scan in each url
        urlArray = realloc(urlArray, (numURLs+1)*sizeof(char*));
        urlArray[numURLs] = malloc(strlen(string)+1);
        strcpy(urlArray[numURLs++], string);
    }
    fclose(collection); // close collection.txt

    //declare variables/file pointer for storing the words
    FILE *url;
    int nWords = 0, i;
    int read;
    char word[MAX_LENGTH];
    char **wordArray = NULL;
	char urlName[100], fileType[100];

	//begin loop to store each word in every file into wordArray
    for(i=0; i<numURLs; i++){
    	strcpy(urlName, urlArray[i]);
    	strcpy(fileType, ".txt");
        char *currentURL = strcat(urlName, fileType);
        url = fopen(currentURL, "r");
        read = 0; // reset read for each file opened
        while (fscanf(url, "%s", word) != EOF) {
            // from Section-2 to #end
            if (strcmp(word, "#end") == 0) { // if word is #end, don't read
                read = 0;
            }
            //if 'word' is in the body of the URL (section-2), read it in
            if (read == 1) { // read only when 1, i.e. after 1st Section-2 is seen
            	//apply normalise function to word to remove punctuation marks and convert to lowercase
            	normalise(word);
            	//check to make sure that the word doesn't already exist in array
                if (duplicates(wordArray, word, nWords) != 1) { // not duplicated
                    wordArray = realloc(wordArray, (nWords+1)*sizeof(char*));
                    wordArray[nWords] = malloc(strlen(word)+1);
                    strcpy(wordArray[nWords++], word);
                }
            }
            if (strcmp(word, "Section-2") == 0) { // read = 1 only after 1st Section-2, for second read = 2
                read++;
            }
        }
   		//close file
        fclose(url);
    }

    //call the function to sort wordArray and urlArray in alphabetiacl order
    sortAlpha(wordArray, nWords);
    sortAlpha(urlArray, numURLs);

    //call function to write words and URLs to file in the right format
    writetoFile(wordArray, urlArray, nWords, numURLs);

    //free memory associated with pointers
    freePointer(urlArray, numURLs);
    freePointer(wordArray, nWords);

    //success
    return 0;
}


//check for duplicates - make sure word doesn't already exist in array
int duplicates(char **str, char *in, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp(str[i], in) == 0) {
            return 1;
        }
    }
    return 0;
}

//normalise function - convert all characters to lowercase and remove punctuation marks
void normalise(char *ch){
	int i;
	char *tempChar, *tempStr;

	//convert each word to lowercase
	for(i=0; i<strlen(ch); i++){
		ch[i] = tolower(ch[i]);
	}
	
	//remove ".", ",", "?", ";" characters
	tempStr = tempChar = ch;
	while(*tempStr){
		*tempChar = *tempStr++;
       	if((*tempChar != '.' && *tempChar != ','  && *tempChar != ';'  && *tempChar != '?' && *tempChar != ' ' )){
       		tempChar++;
       	}
	}
	*tempChar = '\0'; 
}

//sort wordArray and urlArray in alphabetical order using a bubble sort
void sortAlpha(char **str, int length){
	int i, j; 
	char *temp;

	//sort wordArray in alphabetical order
	for(i=0; i<length; i++){
		for(j=i+1; j<length; j++){
			if(strcmp(str[i], str[j])>0){
				temp = str[i];
				str[i] = str[j];
				str[j] = temp;
			}
		}
	}
}

//write words and URLs to file 
void writetoFile(char **str, char **url, int length, int urlLength){
	int i, j; 
	FILE *fp; 
	//open file for writing
	FILE *write = fopen("invertedIndex.txt", "w");

	//begin loop for each word 
	for(i=0; i<length; i++){
		//write the current word to file
		fprintf(write, "%s ", str[i]);
		//begin loop for each URL
		for(j=0; j<urlLength; j++){
			char urlName[100], fileType[100], temp[3000];
        	strcpy(urlName, url[j]);
        	strcpy(fileType, ".txt");
        	char *current = strcat(urlName,fileType);

        	//open URL for reading
        	fp = fopen(current, "r");

        	while(fscanf(fp, "%s", temp)!=EOF){
        		//apply normalise function to word for comparison 
        		normalise(temp);
        		//if the word being read is equal to the word currently being compared against, then the word exists in this URL - so write it to file
            	if(strcmp(temp, str[i])==0){
            		//write word to file 
                	fprintf(write, "%s ", url[j]);
                	break;
            	}
        	}
        	//close URL file
        	fclose(fp);
		}
		//write a newline at the end of each loop until last word is reached 
		if(i != length-1){
			fprintf(write, "\n");
		}
	}
	//close invertedIndex file (the one being written to)
	fclose(write);
}