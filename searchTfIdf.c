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
    int totalWords = nWords;
    /*for (int a = 0; a < totalWords; a++) {
        printf("%s\n", wordArray[a]);
    }*/
    // words stored in wordsArray
    
    // now get every url from collection.txt
    int numURLs = 0;
    char **urlArray = NULL;
    char string[MAX_LENGTH];
    FILE *url = fopen("collection.txt", "r");
    while (fscanf(url, "%s", string) != EOF) {
        urlArray = realloc(urlArray, 50000);
        urlArray[numURLs] = malloc(50000);
        strcpy(urlArray[numURLs++], string);
    }
    int totalURLs = numURLs;
    /*for (int b = 0; b < numURLs; b++) {
        printf("%s\n", urlArray[b]);
    }*/
    // urls in urlArray

    // going through each url, find tf and idf for each query and store in array
    // also store frequency in an array corresponding to URL position
    // search terms, mars, design, moon
    // for e.g. url31
    // both mars and design appear 0 times but moon appears once, total 7 words
    // 0/7, 0/7, 1/7
    // idf = mars- log(7/6), design- log(7/2), moon- log(7/5)
    // url31 tf-idf = 0*log(7/6) + 0*log(7/2) + (1/7)*log(7/5), 1 out of 3 search terms appeared, freq[0] = 1;
    
    int frequency[totalURLs]; // frequency of search terms in url corresponding to index
    for (int c = 0; c < totalURLs; c++) {
        frequency[c] = 0;
    }

    double tfidf[totalURLs];
    for (int d = 0; d < totalURLs; d++) {
        frequency[d] = 0;
    }

    int m = 0;
    int n = 0;
    while (m < totalURLs) {
        while (n < totalWords) {
            // printf("%d, %s %s\n", m, urlArray[m], wordArray[n]);
            tfidf[m] = tfidf[m] + calculateTF(urlArray[m], wordArray[n])*calculateIDF(wordArray[n], totalURLs);
            n++;
        }
        frequency[m] = getFreq(urlArray[m], wordArray, totalWords);
        n = 0;
        m++;
    }


    // so far calucalted frequency values and tf-idf and have urlArray
    /*for (int g = 0; g < totalURLs; g++) {
        printf("%s  %.6f %d\n", urlArray[g], tfidf[g], frequency[g]);
    }
    
    printf("\n");*/

    // ordering thingy goes here, order based on frequency first, then tfidf values
    order(urlArray, tfidf, frequency, totalURLs);
    int final = 0;
    while (final < MAX_URL && final < totalURLs) {
        if (frequency[final] != 0) {
            printf("%s  %.6f\n", urlArray[final], tfidf[final]);
        }
        final++;
    }

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
    fclose(url1);
    // printf("word: %lf ", frequency);
    // finding number of words in file, only words in section 2
    int nWords = 0;
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
    fclose(url2);
    // strcpy(str, urlString);
    // printf("total: %lf\n", total);
    if (total > 0) {
        double tf = frequency/total;
        // printf("TF: %lf\n", tf);
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
                temp = realloc(temp, 50000);
                temp[nDocs] = malloc(50000);
                strcpy(temp[nDocs++], string);
				if (strchr(newLine, '\n')) {
					break;
				}
			}
		}
	}
    free(temp);
    // printf("nDocs: %d ", nDocs);
    // find total amount of documents
    // printf("TotalDocs: %d\n", totalDocs);
    if (nDocs > 0) {
        double IDF = log10((double)totalDocs/nDocs); 
        // printf("IDF: %lf\n", IDF);
        return IDF;
    }
    return 0;
}

// how many times do the query terms appear in the given URL
int getFreq(char *url, char **wordArray, int size) {
    int count = 0;
    char urlname[MAX_LENGTH], filetype[MAX_LENGTH];
    strcpy(urlname, url);
    strcpy(filetype, ".txt");
    char *current = strcat(urlname, filetype);
    
    int i = 0;
    char string[MAX_LENGTH];
    int found = 0;
    while (i < size) {
        // printf("find: %s\n", wordArray[i]);
        FILE *open = fopen(current, "r");
        while (fscanf(open, "%s", string) != EOF && found != 1) {
            normalise(string);
            if (strcmp(wordArray[i], string) == 0) {
                count++;
                found = 1;
                // printf("Found!\n");
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
                /*for (int g = 0; g < length; g++) {
                    printf("%s  %.6f %d\n", arr[g], tfidf[g], freq[g]);
                }*/
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