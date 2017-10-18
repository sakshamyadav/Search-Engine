// pagerank.c file

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "readData.h"
#include "pagerank.h"
#include <math.h>
//#include "readData.h"
//#include "pagerank.h"


//Main function
int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s dampener diffPRsum maxIterations\n", argv[0]);
        exit(1);
    }
    //get arguments from command line
    double dampener = atof(argv[1]);
    double diffPRsum = atof(argv[2]);
    int maxIt = atoi(argv[3]);

    //open collection.txt to read URL names
    FILE *fp;
    fp = fopen("collection.txt", "r");
    char chararr[100];
    char **str = NULL;
    int numURLs = 0;
    int i;
    
    //store URL names in str array
    while(fscanf(fp, "%100s", chararr) != EOF){
        str = realloc(str, (numURLs+1)*sizeof(char*));
        str[numURLs] = malloc(strlen(chararr)+1);
        strcpy(str[numURLs++], chararr);
    }
    //close file
    fclose(fp);

    //declare array to store number of outgoing URLs for each URL
    int outgoing[numURLs];

    //create a graph by calling the buildGraph function
    Graph g = buildGraph();

    //pointer to pageRank array
    double *p = malloc(sizeof(double)*getnumV(g));

    //call calculatePageRank function to calculate the pageRank for each URL. p is the pointer to the array returned by the function
    p = calculatePageRank(g, dampener, diffPRsum, maxIt);

    //get the number of outLinks for each URL and store in outgoing array
    for(i=0; i<numURLs; i++){
        outgoing[i] = numOutLinks(g, i);
    }

    //call sort function to sort the pageRanks in descending order
    sort(p, str, outgoing, numURLs);

    //call writeToText function to write required pageRank information to a text file
    writeToText(p, str, outgoing, numURLs);

    //free memory associated with str and p
    freePointer(str, numURLs);
    free(p);

    //dispose graph
    disposeGraph(g);

    //success
    return 0;
}

//function to calculate pageRank
double *calculatePageRank(Graph g, double d, double diffPR, int maxIt) {

    //declare and initialise required variables
    int N = getnumV(g);
    double Nfloat = (double)N;
    double *pageRank = malloc(sizeof(double)*N);
    double oldPR[N];

    //some more variables to assist in loops!
    int i, j, u, k;

    //set the initial pageRank of each URL to 1/(num URLs)
    for(i=0; i<N; i++){
        pageRank[i] = 1/Nfloat;
    } 

    //initialise variables to start the loop
    int iterations = 0;
    double diff = diffPR; 
    double sigma = 0; 
    double wIn = 0;
    double wOut = 0;
    double diffSum = 0; 

    //the main loop to calculate pageRank begins here
    while(iterations<maxIt && diff >= diffPR){
        //increment the number of iterations by 1
        iterations++;
        //reset value of diffSum for each iteration
        diffSum = 0;

        //update old pageRank array
        for(u=0; u<N; u++){
            oldPR[u] = pageRank[u];
        }

        //MAIN i LOOP STARTS HERE
        //Update PageRank of each URL
        for(i=0; i<N; i++){
            //Reset sigma value
            sigma = 0; 
            //Find all URLs with outgoing links to URL i 
            for(j=0; j<N; j++){
                //If URL j links to i, calculate wIn, wOut and sigma value
                if(vLinkw(g, j, i)){
                    wIn = numInLinks(g, i)/sumInLinks(g, j);
                    wOut = numOutLinks(g, i)/sumOutLinks(g, j);
                    sigma = sigma + (oldPR[j]*wIn*wOut); 
                }
            }
            //update pageRank for URL i
            pageRank[i] = ((1-d)/Nfloat)+(d*sigma);

        }

        //update value of diffSum 
        for(k=0; k<N; k++){
            diffSum = diffSum + fabs(pageRank[k] - oldPR[k]);
        }

        //update diff to diffSum
        diff = diffSum; 

    }

    //return pointer to pageRank array 
    return pageRank;
}

//bubble sort function to sort pageRanks in descending order
void sort(double *pr, char **str, int out[], int length){
    //declare temp variables for pageRank array, URL name array and outLinks array
    int i, j;
    double temp;
    int temp2; 
    char *tempStr;

    for(i=0; i<length; i++){
        for(j=i+1; j<length; j++){
            if(pr[i]<pr[j]){
                //swap pageRank values if pr[i] < pr[j]
                temp = pr[i];
                pr[i] = pr[j];
                pr[j] = temp;

                //ensure that the order is maintained for the URL names
                tempStr = str[i];
                str[i] = str[j];
                str[j] = tempStr;

                //ensure that the order is maintained for outLinks for each URL
                temp2 = out[i];
                out[i] = out[j];
                out[j] = temp2;
            }
        }
    }

}

//function to write pageRank information to text file
void writeToText(double *pr, char **str, int out[], int length){
    //open file for writing
    FILE *fp = fopen("pagerankList.txt", "w");
    int i; 

    //write URL name, outLinks and pageRank value for each URL
    for(i=0; i<length; i++){
        fprintf(fp, "%s, %d, %.7f\n", str[i], out[i], pr[i]);
    }

    //close file
    fclose(fp);

}
