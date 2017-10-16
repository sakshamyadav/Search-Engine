// searchPagerank.c 

// search words as cmdline arguments
// finds pages with one or more search terms and outputs to stdout top 30 pages
// find pages in descending order or number of search terms found
// in each group descending order of weighted PageRank
// if < 30 matches, output all

// must use data from invertedIndex.txt and pagerankList.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"
#include "inverted.h"
#include "pagerank.h"
#include "readData.h"
#include "searchPagerank.h"
//include relevant files

//main function begins
int main(int argc, char *argv[]) {
 
    //declare and initialise variables/pointers
    int nWords = 0;
    char **wordArray = NULL;
    char **urlArray = NULL;
    int numURLs = 0;
    char string[3000];
    char newLine[3000];
    int i;

    //ensure that words are entered into command line
    if (argc < 2) {
        fprintf(stderr, "Usage: %s word1 word2 word3...\n", argv[0]);
        exit(1);
    }

    //read words into wordArray
    for(i=0; i<argc-1; i++){
        FILE *fp = fopen("invertedIndex.txt", "r");
        normaliseInput(argv[i+1]);
        wordArray = realloc(wordArray, (nWords+1)*sizeof(char*));
        wordArray[nWords] = malloc(strlen(argv[i])+1);
        strcpy(wordArray[nWords++],argv[i+1]);

        //when a word from command line is found, store it's URLs in urlArray
        while(fscanf(fp, "%99s", string)==1){
            if(strcmp(wordArray[i], string)==0){
                while (fscanf ( fp, "%99s%99[ \t\n]", string, newLine)==2){
                    urlArray = realloc(urlArray, (numURLs+1)*sizeof(char*));
                    urlArray[numURLs] = malloc(strlen(string)+1);
                    strcpy(urlArray[numURLs++], string);

                    if(strchr(newLine, '\n')){
                        break;
                    }
                }
            }
        }
        //close file
        fclose(fp);
    }

    //determine the number of search queries appearing in each URL
    getFreq(urlArray, numURLs);

    //free memory associated with wordArray and urlArray
    freePointer(wordArray, nWords);
    freePointer(urlArray, numURLs);

    //success
    return 0;
}

//normalise input characters for matching purposes
void normaliseInput(char *ch){
    int i;
    char *tempChar, *tempStr;

    //convert each character to lowercase
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

//determine the number of search terms present in each URL and prepare data for processing
void getFreq(char **arr, int length){
    int i, j, count; 
    int freq[length]; 
    char **urlHold = NULL;
    int *freqHold = malloc(sizeof(int)*length);
    int counter = 0;

    //set initial frequency of each URL to -1
    for(i=0 ;i<length; i++){
        freq[i] = -1;
    }

    //determine the frequency of each URL based on search terms
    for(i=0; i<length; i++){
        count = 1; 
        for(j=i+1; j<length; j++){
            if(strcmp(arr[i], arr[j])==0){
                count++;
                freq[j] = 0;
            }   
        }
        if(freq[i]!= 0){
            freq[i] = count; 
        }
    }

    //transfer data from arr and freq pointers to urlHold and freqHold dynamic structures
    for(i=0; i<length; i++){
        if(freq[i] != 0){
            urlHold = realloc(urlHold, (counter+1)*sizeof(char*));
            urlHold[counter] = malloc(strlen(arr[i])+1);
            strcpy(urlHold[counter], arr[i]);
            freqHold[counter] = freq[i];
            counter++;
        }
    }   
    
    //call order function to rank in descending order of frequency
    order(urlHold, freqHold, counter);

    //call function to print to console
    consolePrint(urlHold, freqHold, counter);

    //free data associated with freqHold and urlHold
    free(freqHold);
    freePointer(urlHold, counter);
}

//order function to rank from high to low frequency using bubble sort
void order(char **arr, int *freq, int length){
    int i,j, temp;
    char *temp2; 

    for(i=0; i<length; i++){
        for(j=i+1; j<length; j++){
            if(freq[i]<freq[j]){
                
                //swap frequency value
                temp = freq[i];
                freq[i] = freq[j];
                freq[j] = temp;

                //swap URL name to maintain correct order
                temp2 = arr[i];
                arr[i] = arr[j];
                arr[j] = temp2;
            }
        }
    }

}

//function to print to console
void consolePrint(char **arr, int *freq, int length){
    int max, i, j, k;
    int tempCounter = 0; 
    int distinct = getDistinct(freq, length);
    char **temp = NULL;
    int start = 0;
    char **output = NULL; 
    int outputCounter = 0; 

    //loop for each frequency set (e.g. set of 10 occurrences, 5 occurences etc)
    for(i=0; i<distinct; i++){
        //reset tempCounter and max frequency value for each iteration
        tempCounter = 0;
        max = getMax(freq, length);
        //read all frequency values correponding to 'max'
        for(j=start; j<length; j++){
            //stop the loop counter reaches an element outside the frequency set
            if(freq[j] != max){
                //set start value of next iteration to j, to maintain sequence
                start = j;
                break;
            }
            //store the URLs corresponding to this frequency set in temp data structure
            temp= realloc(temp, (tempCounter+1)*sizeof(char*));
            temp[tempCounter] = malloc(strlen(arr[j])+1);
            strcpy(temp[tempCounter++],arr[j]); 
            //set frequency to -1 to mark as 'seen'
            freq[j] = -1;
        }
        //sort the frequency set based on pagerank value
        sortpageRank(temp, tempCounter);
        //store the sorted frequency set in the 'output' data structure, following previous frequency sets
        for(k=0; k<tempCounter; k++){
            output = realloc(output, (outputCounter+1)*sizeof(char*));
            output[outputCounter] = malloc(strlen(temp[k])+1);
            strcpy(output[outputCounter++], temp[k]);
        }

    }

    //if number of pages returned by this algorithm is less than 30, display all
    if(outputCounter<30){
        for(i=0; i<outputCounter; i++){
            printf("%s\n", output[i]);
        }
    }
    //if there are 30 or more pages, only display 30
    else{
        for(i=0; i<30; i++){
            printf("%s\n", output[i]);
        }   
    }

    //free memory associated with output and temp pointers
    freePointer(output, outputCounter);
    freePointer(temp, tempCounter);
}

//determine the maximum frequency URL
int getMax(int *freq, int length){
    int max = freq[0];
    int i; 

    //determine max frequency
    for(i=1; i<length; i++){
        if(freq[i] > max){
            max = freq[i];
        }
    }

    //return max value
    return max; 
}

//get the number of distinct frequency sets 
int getDistinct(int *freq, int length){
    int distinct = 1; 
    int isDistinct; 
    int i, j;

    //check the freq array for number of distinct frequencies
    for(i=1 ; i<length; i++){
        isDistinct = 1; 
        for(j=0; isDistinct && j<i; j++){
            if(freq[i] == freq[j]){
                isDistinct = 0;
            }
        }
        if(isDistinct){
            distinct++;
        }
    }   

    //return number of distinct frequencies
    return distinct; 
}

//sort URLs in the same frequency set based on pagerank value
void sortpageRank(char **arr, int length){
    int i, j; 
    char string[3000];
    char newLine[3000];
    double *prValues = malloc(sizeof(double)*length);
    int prCount = 0;
    double temp; 
    char *tempStr; 

    //read pagerankList.txt to find the pagerank of each URL in the freq set
    for(i=0; i<length; i++){
        FILE *pr = fopen("pagerankList.txt", "r");
        while(fscanf(pr, "%s", string) != EOF){
            //normalise URL to remove the comma in pagerankList.txt for matching purposes
            normaliseInput(string);
            //if URL is found, keep reading until pagerank is found
            if(strcmp(string, arr[i])==0){
                while (fscanf ( pr, "%99s%99[ \t\n]", string, newLine)==2){
                    if(strchr(newLine, '\n')){
                        //convert string value of pagerank to float and store in prValues
                        prValues[prCount++] = atof(string);
                        break;
                    }
                }   
            }
        }
    }

    //bubble sort to order pagerank in descending order
    for(i = 0; i<prCount; i++){
        for(j=i+1; j<prCount; j++){
            if(prValues[i] < prValues[j]){

                //swap pagerank position
                temp = prValues[i];
                prValues[i] = prValues[j];
                prValues[j] = temp;

                //swap corresponding URL name to maintain order
                tempStr = arr[i];
                arr[i] = arr[j];
                arr[j] = tempStr;  
            }
        }
    }

    //free prValues memory
    free(prValues);

}



