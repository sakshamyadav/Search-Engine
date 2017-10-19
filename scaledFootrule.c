#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

double scaleFootrule = 100;
int optimumPos[100];

void sort(int *pr, int length);
void positions(int *pos, int *unranked, int *ranked, int length);
void swap(int *pos, int i, int j);
void permute(int *array,int i,int length, int *pos1, int pos1Len, int *pos2, int pos2Len);
int main(int argc, char **argv) {

	if(argc < 2) {
		fprintf(stderr, "Usage: %s rankA.txt  rankD.txt", argv[0]);
		exit(1);
	}
	//opening the first file (e.g. rankA.txt)
	FILE *fp1;
	fp1 = fopen(argv[1], "r");

	//opening the second file (e.g. rankB.txt)
	FILE *fp2;
	fp2 = fopen(argv[2], "r");

	//no. of items in the first file (rankA.txt)
	int numItems1 = 0;
	//no. of items in the second file (rankB.txt)
	int numItems2 = 0;

	char chararr[100];


	int rank1[100];
	int rank2[100];

	int rank1S[100];
	int rank2S[100];

	int pos1[100];
	int pos2[100];

	//rankx[] array stores the no suffix of the url 
	//rankxS[] array stores the sorted no suffix of the url
	while(fscanf(fp1, "%s", chararr) != EOF)	{
	    rank1[numItems1] = rank1S[numItems1] = atoi(chararr+3);
	    numItems1++;
	}
	printf(" numitems1: %d\n", numItems1);

	while(fscanf(fp2, "%s", chararr) != EOF)	{
		rank2[numItems2] = rank2S[numItems2]= atoi(chararr+3);
		numItems2++;
	}

	int numItems = (numItems1 > numItems2) ? numItems1:numItems2;

	fclose(fp1);
	fclose(fp2);

	sort(rank1S, numItems1);
	sort(rank2S, numItems2);

	//position function makes an array with the position of the urls
	//e.g. 
	/*
	url3
	url2
	url4
	url1
	the postion inputs the postions of these into the given array
	pos1[4,2,1,3]
	As url1 has 4th position, url2 has 2nd position, url3 has 1st position and url4 has 3rd position
	*/ 
	positions(pos1, rank1, rank1S, numItems1);
	positions(pos2, rank2, rank2S, numItems2);

	//orderedPosArray creates an array with ordered numbers upto the the max no. of items in the biggest array.
	//e.g. rankA.txt has 5 items rankB.txt has 4 items, therefore orderedPosArray would contain [1,2,3,4,5]
	//Purpose - to pass into permute function and change the order to find the smallest scale foot distance.
	int orderedPosArray[100];
	int i;
	for(i = 0; i < numItems; i++) {
		orderedPosArray[i] = i+1;
		printf("%d\n", orderedPosArray[i]);
	}

	//double scaleFootrule = 100;

	printf("RankA: 1st: %d\nRankB: 1st: %d\nRank2S: 1st: %d\nPos: 1st: %d\n", rank1[0], rank2[0], rank2S[0], pos2[0]);
	permute(orderedPosArray, 0, numItems, pos1, numItems1, pos2, numItems2);
	printf("squarerile: %lf\n", scaleFootrule);
	for(i = 0; i < numItems; i++) {
		printf("%d\n", optimumPos[i]);
	}

	return 0;

}

void sort(int *pr, int length){
    //declare temp variables for pageRank array, URL name array and outLinks array
    int i, j;
    int temp;

    for(i=0; i<length; i++){
        for(j=i+1; j<length; j++){
            if(pr[i]>pr[j]){
                //swap pageRank values if pr[i] > pr[j]
                temp = pr[i];
                pr[i] = pr[j];
                pr[j] = temp;
            }
        }
    }

}

void positions(int *pos, int *unranked, int *ranked, int length) {
	int posNum = 0;

	int i, j;
	for(i = 0; i < length; i++) {
		for(j = 0; j < length; j++) {
			if(ranked[i] == unranked[j]) {
				pos[posNum++] = j+1;
			}
		}
	}
}

void swap(int *pos, int i, int j) {
	int temp = pos[i];
	pos[i] = pos[j];
	pos[j] = temp;
}

//This function needs correcting.
void permute(int *array,int i,int length, int *pos1, int pos1Len, int *pos2, int pos2Len) { 

	double temp = 0;
	int j, k;

  	if (i == length){

     	for(j = 0; j < pos1Len; j++) {
     		printf("small cla:%lf\n", ((double)pos1[j]/(double)length));
     		temp += fabs(((double)pos1[j]/(double)pos1Len) - ((double)array[j]/(double)length));
     		printf("1********************%lf\n", temp);
     	}

     	for(j = 0; j < pos2Len; j++) {
     		temp += fabs(((double)pos2[j]/(double)pos2Len - (double)array[j]/(double)length));
     		printf("2********************%lf\n", temp);
     	}
     	printf("3********************%lf\n", temp);
     	if(temp < scaleFootrule) {
     		scaleFootrule = temp;
     		for(k = 0; k < length; k++) {
     			if(pos1Len > pos2Len) {
     				optimumPos[k] = pos1[k];
     			} else {
     				optimumPos[k] = pos2[k];
     			}
     			
     		}

     	}
     	printf("4********************%lf\n", scaleFootrule);
    	return;
  	}
	for (j = i; j < length; j++) { 
		printf("i: %d\nj: %d\n", i, j);
 		swap(array,i,j);
		permute(array,i+1, length, pos1, pos1Len, pos2, pos2Len);
		//swap(array,i,j);
		printf("********************%lf\n", scaleFootrule);
	}
	return;
}

//double footruleCalculator(int *pos, int )

//double posCheck()