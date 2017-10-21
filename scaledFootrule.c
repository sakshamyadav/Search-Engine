#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

double scaleFootrule = 100;
int optimumPos[100];

void sort(int *pr, int length);
void positions(int *pos, int *unranked, int *ranked, int length, int lengthAll);
void swap(int *pos, int i, int j);
void permute(int *array,int i,int length, int pos[100][100], int *numUrls, int argc);

int main(int argc, char **argv) {

	//Error when no file in given in commandline
	if(argc < 2) {
		fprintf(stderr, "Usage: %s rankA.txt  rankD.txt", argv[0]);
		exit(1);
	}

	//Declaration of variables
	int i;
	//mainMatrix contains the url suffix no. of each file (e.g. array[0][] would be urls from first file)
	int mainMatrix[1000][1000];
	int mainMatrixS[1000][1000];	//numbers from mainMatrix sorted in increasing order
	//Number of urls in each file (e.g. array[0] would be the no. of urls from first file)
	int numUrls[1000] = {0};
	char string[3000];
	//Max no. of urls given among all files (e.g. rankA.txt has 5 urls and rankB.txt has 4 urls, this variable stores 5)
	int numUrlsMax = 0;
	int pos[100][100];
	//int maxUrlsLocation = 0;
	int numAllUrls = 0;
	int allUrls[100] = {0};

	//Opening the files and storing the no. suffix of url into the matrix
	for(i = 0; i < argc-1; i++) {
		FILE *fp;
		fp = fopen(argv[i+1], "r");
		int j = 0;
		int k;
		while(fscanf(fp, "%s", string) != EOF) {
			char *token = strtok(string, "url");
			mainMatrix[i][j] = mainMatrixS[i][j] = atoi(token);
			int check = 0;
			for(k = 0; k < numAllUrls; k++) {
				if(allUrls[k] == atoi(token)) {
					check = 1;
					break;
				}
			}
			if(!check) {
				allUrls[numAllUrls] = atoi(token);
				numAllUrls++;
			}
			j++;
			numUrls[i]++;
		}
		fclose(fp);
	}

	sort(allUrls, numAllUrls);



	//Storing the max urls among all files within numUrlsMax
	for(i = 0; i < argc-1; i++) {
		if(numUrls[i]>numUrlsMax) {
			numUrlsMax = numUrls[i];
			//maxUrlsLocation = i;
		}
	}

	//Sorting array using the function sort()
	for(i = 0; i < argc-1; i++) {
		sort(mainMatrixS[i], numUrls[i]);
	}

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


	for(i = 0; i < argc-1; i++) {
		positions(pos[i], mainMatrix[i], allUrls, numUrls[i], numAllUrls);
	}

	//orderedPosArray creates an array with ordered numbers upto the the max no. of items in the biggest array.
	//e.g. rankA.txt has 5 items rankB.txt has 4 items, therefore orderedPosArray would contain [1,2,3,4,5]
	//Purpose - to pass into permute function and change the order to find the smallest scale foot distance.
	int orderedPosArray[100];
	for(i = 0; i < numAllUrls; i++) {
		orderedPosArray[i] = i+1;
	}

	//Passing values into the permute() function
	//permute(orderedPosArray, 0, numAllUrls, pos, numUrls, argc);
	permute(orderedPosArray, 0, numAllUrls, pos, numUrls, argc);
	printf("%lf\n", scaleFootrule);
	
	for(i = 0; i < numAllUrls; i++) {
		char placeholder[20];
		sprintf(placeholder,"url%d",allUrls[optimumPos[i]-1]);
		printf("%s\n", placeholder);
	}

	return 0;

}

//Sort function sorts the given array in increasing order
void sort(int *pr, int length){
    int i, j;
    int temp;

    for(i=0; i<length; i++){
        for(j=i+1; j<length; j++){
            if(pr[i]>pr[j]){
                //swap values if pr[i] > pr[j]
                temp = pr[i];
                pr[i] = pr[j];
                pr[j] = temp;
            }
        }
    }

}

void positions(int *pos, int *unsorted, int *sortedAll, int length, int lengthAll) {
	int posNum = 0;

	int i, j;
	for(i = 0; i < lengthAll; i++) {
		for(j = 0; j < lengthAll; j++) {
			if(sortedAll[i] == unsorted[j]) {
				pos[posNum++] = j+1;
				break;
			}
		}
	}
}

//Used as part of the permute function. It is used to swap the values in the array to permute the order
void swap(int *pos, int i, int j) {
	int temp = pos[i];
	pos[i] = pos[j];
	pos[j] = temp;
}

//Permutes the order the positions and calculates the smallest scaled foot rule
void permute(int *array,int i,int length, int pos[100][100], int *numUrls, int argc) { 

	double temp = 0;
	int j, k, l;

	//printf("length:%d\n", length);
  	if (i == length){
  		for(l = 0; l < argc-1; l++) {
  			for(j = 0; j < numUrls[l]; j++) {
  				temp += fabs(((double)pos[l][j]/(double)numUrls[l]) - ((double)array[j]/(double)length));
  			}
  		}

     	if(temp < scaleFootrule) {
     		scaleFootrule = temp;
     		for(k = 0; k < length; k++) {
     				optimumPos[k] = array[k];     			
     		}

     	}
    	return;
  	}
	for (j = i; j < length; j++) { 
 		swap(array,i,j);
		permute(array,i+1, length, pos, numUrls, argc);
	}
	return;
}
