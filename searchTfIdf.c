// searchIfIdf.c 

// content based search engine
// given key words as smd line arguments
// 30 pages in descending order of number of search terms found and then within each group
// descending order of summation of tf-idf values of all search terms found

// use data from invertedIndex.txt and collection.txt
// (similar to searchPagerank)

// a url's tf-idf is the tf for that document * idf- idf is the same for all documents for the word
// calculate tf-idf values for each query term in a page, rank pages based on the summation of tf-idf values for all query terms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "graph.h"
#include "readData.h"

#define MAX_PAGES 30
#define MAX_LENGTH 2401
#define MAX_URL 30

// function prototypes
int duplicates(char **str, char *in, int size);
void normalise(char *ch);
double calculateTF(char *str, char *wordArray); 
double calculateIDF(char *word, int totalDocs);
int getFreq(char *url, char **wordArray, int size);
void order(char **array, double *tfidf, int *freq, int size);

//main function start
int main(int argc, char *argv[]) {

    // error message
    if (argc < 2) {
        fprintf(stderr, "Usage: %s word1 word2 word3... \n", argv[0]);
        exit(1);
    }
    
    // storing search terms into a word array
    int i = 1;
    int nWords = 0;
    char **wordArray = NULL;
    while (i < argc) {
        normalise(argv[i]);
        wordArray = realloc(wordArray, (nWords+1)*sizeof(char*));
        wordArray[nWords] = malloc(strlen(argv[i])+1);
        strcpy(wordArray[nWords++],argv[i]);
        i++;
    }

    int totalWords = nWords;

    //storing url names into urlArray
    int numURLs = 0;
    char **urlArray = NULL;
    char string[MAX_LENGTH];
    FILE *url = fopen("collection.txt", "r");
    while (fscanf(url, "%s", string) != EOF) {
        urlArray = realloc(urlArray, (numURLs+1)*sizeof(char*));
        urlArray[numURLs] = malloc(strlen(string)+1);
        strcpy(urlArray[numURLs++], string);
    }
    int totalURLs = numURLs;

    //close file
    fclose(url);
    
    // initialise frequency of search terms to 0 
    int frequency[totalURLs]; 
    for (int c = 0; c < totalURLs; c++) {
        frequency[c] = 0;
    }

    //initialise tf-idf values to 0 
    double tfidf[totalURLs];
    for (int d = 0; d < totalURLs; d++) {
        tfidf[d] = 0;
    }

    int m = 0;
    int n = 0;

    //determine tf-idf and frequency values 
    while (m < totalURLs) {
        while (n < totalWords) {
            tfidf[m] = tfidf[m] + calculateTF(urlArray[m], wordArray[n])*calculateIDF(wordArray[n], totalURLs);
            n++;
        }
        frequency[m] = getFreq(urlArray[m], wordArray, totalWords);
        n = 0;
        m++;
    }


    //call order function to order based on frequency first, then tfidf values
    order(urlArray, tfidf, frequency, totalURLs);
    int final = 0;

    //output the URLs, along with corresponding tf-idf value
    while (final < MAX_URL && final < totalURLs) {
        if (frequency[final] != 0) {
            printf("%s  %.6f\n", urlArray[final], tfidf[final]);
        }
        final++;
    }

    //free memory associated with wordArray and urlArray
    freePointer(wordArray, nWords);
    freePointer(urlArray, numURLs);

    return 0;
}


// calculate term frequency adjusted for document length
// frequency of term t in url/ number of words in url
// takes in the url, the word
// returns the TF of th word in the url
double calculateTF(char *str, char *wordArray) {

    // finding frequency of the search term in the current document
    double frequency = 0;
    char urlname[MAX_LENGTH], filetype[MAX_LENGTH];
    strcpy(urlname, str);
    strcpy(filetype, ".txt");
    char *current = strcat(urlname, filetype);
    FILE *url1 = fopen(current, "r");
    char word1[MAX_LENGTH];
    while (fscanf(url1, "%s", word1) != EOF) {
        normalise(word1);
        if (strcmp(wordArray, word1) == 0) {
            frequency = frequency + 1;
        }
    }
    //close file
    fclose(url1);

    //read number of words in each URL
    double total = 0;
    int read = 0;
    char word2[MAX_LENGTH];
    FILE *url2 = fopen(current, "r");
    while (fscanf(url2, "%s", word2) != EOF) {
        // from Section-2 to #end
        if (strcmp(word2, "#end") == 0) { // if word is #end, don't read
            read = 0;
        }
        //if 'word' is in the body of the URL (section-2), read it in
        if (read == 1) { // read only when 1, i.e. after 1st Section-2 is seen
            //apply normalise function to word to remove punctuation marks and convert to lowercase
            //check to make sure that the word doesn't already exist in array
            total = total + 1;
        }
        if (strcmp(word2, "Section-2") == 0) { // read = 1 only after 1st Section-2, for second read = 2
            read = read + 1;
        }
    }
    //close file
    fclose(url2);

    //return term frequency 
    if (total > 0) {
        double tf = frequency/total;
        return tf;
    }
    return 0;
}

// calculate inverse document frequency
// log base 10 (total number of documents/ number of documents containing the terms)
double calculateIDF(char *word, int totalDocs) {
    // find amount of documents containing the word
    int nDocs = 0;
    char string[MAX_LENGTH];
    char newLine[MAX_LENGTH];
    char **temp = NULL;
    FILE *inverted = fopen("invertedIndex.txt", "r");
    while (fscanf(inverted, "%s", string) != EOF) {
        if (strcmp(word, string) == 0) {
            while (fscanf(inverted, "%99s%99[ \t\n]", string, newLine) == 2) {
                temp = realloc(temp, (nDocs+1)*sizeof(char*));
                temp[nDocs] = malloc(strlen(string)+1);
                strcpy(temp[nDocs++], string);
                if (strchr(newLine, '\n')) {
                    break;
                }
            }
        }
    }
    //free memory associate with temp 
    freePointer(temp, nDocs);
    // find total amount of documents
    if (nDocs > 0) {
        double IDF = log10((double)totalDocs/nDocs); 
        return IDF;
    }
    return 0;
}

// find how many times do the query terms appear in the given URL
int getFreq(char *url, char **wordArray, int size) {
    int count = 0;
    char urlname[MAX_LENGTH], filetype[MAX_LENGTH];
    strcpy(urlname, url);
    strcpy(filetype, ".txt");
    char *current = strcat(urlname, filetype);
    
    int i = 0;
    char string[MAX_LENGTH];
    int found = 0;
    //check if found 
    while (i < size) {
        FILE *open = fopen(current, "r");
        while (fscanf(open, "%s", string) != EOF && found != 1) {
            normalise(string);
            if (strcmp(wordArray[i], string) == 0) {
                count++;
                found = 1;
            }
        }
        found = 0;
        fclose(open);
        i++;
    }
    return count;
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

// orders array in terms of frequency and tfidf
// first move those with higher frequencies above the current index
// order in descending order, those with higher frequencies are at top, lower at bottom
void order(char **arr, double *tfidf, int *freq, int length) {
    int i, j, temp1;
    double temp2;
    char *temp3;
    for (i = 0; i < length; i++) {
        for (j = i + 1; j < length; j++) {
            if (freq[i] < freq[j]) { // compare frequency
                //swap frequency value
                temp1 = freq[i];
                freq[i] = freq[j];
                freq[j] = temp1;
                // swap tfidf to maintain order
                temp2 = tfidf[i];
                tfidf[i] = tfidf[j];
                tfidf[j] = temp2;
                //swap URL name to maintain correct order
                temp3 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp3;
            }
            if (freq[i] == freq[j] && tfidf[i] < tfidf[j]) {
                temp1 = freq[i];
                freq[i] = freq[j];
                freq[j] = temp1;
                // swap tfidf to maintain order
                temp2 = tfidf[i];
                tfidf[i] = tfidf[j];
                tfidf[j] = temp2;
                //swap URL name to maintain correct order
                temp3 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp3;
            }
        }
    }
}
