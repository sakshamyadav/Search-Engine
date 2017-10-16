// searchIfIdf.c 

// content based search engine
// given key words as smd line arguments
// 30 pages in descending order of number of search terms found and then within each group
// descending order of summation of tf-idf values of all search terms found

// use data from invertedIndex.txt and collection.txt
// (similar to inverted index)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "graph.h"

#define MAX_PAGES 30
#define MAX_LENGTH 225

// function prototypes
int duplicates(char **str, char *in, int size);
void normalise(char *ch);
double calculateTF(char *str, char **wordArray, int arrayLength); 
double calculateIDF();
double sum(double tf, double idf);

int main(int argc, char *argv[]) {

    // error message
    if (argc < 2) {
        fprintf(stderr, "Usage: %s word1 word2 word3... \n", argv[0]);
        exit(1);
    }
    
    // storing search terms into an word array
    int i = 1;
    int nWords = 0;
    char **wordArray = NULL;
    while (i < argc) {
        normalise(argv[i]);
        wordArray = realloc(wordArray, 50000);
        wordArray[nWords] = malloc(50000);
        strcpy(wordArray[nWords++],argv[i]);
        i++;
    }

    // now open invertedIndex.c to get urls corresponding each word
    int j = 0;
    int totalWords = nWords;
    int numURLs = 0;
    char string[MAX_LENGTH];
    char newLine[MAX_LENGTH];
    char **urlArray = NULL;
    while (j < totalWords) {
        FILE *url = fopen("invertedIndex.txt", "r");
        while (fscanf(url, "%s", string) != EOF) {
            if (strcmp(string, wordArray[j]) == 0) {
                while (fscanf(url, "%99s%99[ \t\n]", string, newLine) == 2) {
                    urlArray = realloc(urlArray, 50000);
                    urlArray[numURLs] = malloc(50000);
                    strcpy(urlArray[numURLs++], string);
					if (strchr(newLine, '\n')) {
						break;
					}
				}
            }
        }
        j++;
    }
    int totalURLs = numURLs;
    for (int z = 0; z < totalWords; z++) {
        printf("%s\n", wordArray[z]);
    }
    for (int x = 0; x < numURLs; x++) {
        printf("%s\n", urlArray[x]);
    }
    // urls which appear most in the url array are the most frequently occuring ones
    // getFreq(urlArray, totalURLs);

    double termFreq;
    int y = 0;
    while (y < totalURLs) {
        termFreq= calculateTF(urlArray[y], wordArray, totalWords);
        printf("%lf\n", termFreq);
        y++;
    }
    return 0;
}


// check if the string array already has a string
int duplicates(char **str, char *in, int size) {
    if (str == NULL) {
        return 0;
    }
    int i;
    for (i = 0; i < size; i++) {
        if (strcmp(str[i], in) == 0) {
            return 1;
        }
    }
    return 0;
}

//normalise function - convert all characters to lowercase and remove punctuation marks
void normalise(char *ch) {
	int i;
	char *tempChar, *tempStr;
	//convert each word to lowercase
	for (i = 0; i < strlen(ch); i++) {
		ch[i] = tolower(ch[i]);
	}
	//remove ".", ",", "?", ";" characters
	tempStr = tempChar = ch;
	while (*tempStr) {
		*tempChar = *tempStr++;
       	if (*tempChar != '.' && *tempChar != ','  && *tempChar != ';'  && *tempChar != '?' && *tempChar != ' '){
       		tempChar++;
       	}
	}
	*tempChar = '\0'; 
}

// reordering the URL array such that there is 

// calculate term frequency adjusted for document length
// frequency of term t in url/ number of words in url
double calculateTF(char *str, char **wordArray, int arrayLength) {
    double frequency = 0;
    // finding frequency of the search terms
    strcat(str, ".txt");
    int i = 0;
    char word[MAX_LENGTH];
    while (i < arrayLength) {
        FILE *url = fopen(str, "r");
        while (fscanf(url, "%s", word) != EOF) {
            normalise(word);
            if (strcmp(wordArray[i], word) == 0) {
                frequency++;
            }
        }
        fclose(url);
        i++;
    }
    // printf("%lf\n", frequency);
    // finding number of words in file, words in section 2
    int nWords = 0;
    double total = 0;
    char **temp = NULL;
    int read = 0;
    FILE *url = fopen(str, "r");
    while (fscanf(url, "%s", word) != EOF) {
        // from Section-2 to #end
        if (strcmp(word, "#end") == 0) { // if word is #end, don't read
            read = 0;
        }
        //if 'word' is in the body of the URL (section-2), read it in
        if (read == 1) { // read only when 1, i.e. after 1st Section-2 is seen
            //apply normalise function to word to remove punctuation marks and convert to lowercase
            // normalise(word);
            //check to make sure that the word doesn't already exist in array
            total++;
        }
        if (strcmp(word, "Section-2") == 0) { // read = 1 only after 1st Section-2, for second read = 2
            read = read + 1;
        }
    }
    // printf("%lf\n", total);
    if (total > 0) {
        double tf = frequency/total;
        return tf;
    }
    return 0;
}

// calculate
double calculateIDF() {


}

double calcsum(double tf, double idf) {
    double sum = tf + idf;
    return sum;
}
