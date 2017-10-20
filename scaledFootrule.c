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
void permute(int *array,int i,int length, int pos[100][100], int *numUrls, int argc);
int main(int argc, char **argv) {

	if(argc < 2) {
		fprintf(stderr, "Usage: %s rankA.txt  rankD.txt", argv[0]);
		exit(1);
	}
	int i;
	//opening the first file (e.g. rankA.txt)
	//FILE *fp;
	int mainMatrix[1000][1000];
	int mainMatrixS[1000][1000];
	int numUrls[1000] = {0};
	char string[3000];

	for(i = 0; i < argc-1; i++) {
		FILE *fp;
		fp = fopen(argv[i+1], "r");
		int j = 0;
		while(fscanf(fp, "%s", string) != EOF) {
			char *token = strtok(string, "url");
			mainMatrix[i][j] = mainMatrixS[i][j] = atoi(token);
			j++;
			numUrls[i]++;
		}
		fclose(fp);
	}


	printf("Matrix[0][0]: %d\n", mainMatrix[0][0]);
	printf("Matrix[0][1]: %d\n", mainMatrix[0][1]);
	printf("Matrix[0][2]: %d\n", mainMatrix[0][2]);
	printf("Matrix[0][3]: %d\n", mainMatrix[0][3]);
	printf("Matrix[0][3]: %d\n", mainMatrix[0][4]);
	printf("Matrix[1][0]: %d\n", mainMatrix[1][0]);
	printf("Matrix[1][1]: %d\n", mainMatrix[1][1]);
	printf("Matrix[1][2]: %d\n", mainMatrix[1][2]);
	printf("Matrix[1][3]: %d\n", mainMatrix[1][3]);
	printf("numUrls[0]: %d\n", numUrls[0]);
	printf("numUrls[1]: %d\n", numUrls[1]);

	//char ***urlarray = (char***) malloc(numUrls*sizeof(char**));

/*
	for(i = 0; i < argc-1; i++) {
		fp = fopen(argv[i+1], "r");
		while (fscanf(fp, "%s", string) != EOF) {
			urlarray = realloc(urlarray, (numUrls+1)*sizeof(char**));

			urlarray[numUrls] = malloc(strlen(string)+1);
			strcpy(urlarray[numUrls++], string);
		}
	}*/

	//opening the second file (e.g. rankB.txt)

	//no. of items in the first file (rankA.txt)
	//int numItems1 = 0;
	//no. of items in the second file (rankB.txt)
	//int numItems2 = 0;

	//char chararr[100];


	// int rank1[100];
	// int rank2[100];

	// int rank1S[100];
	// int rank2S[100];

	// int pos1[100];
	// int pos2[100];

	//rankx[] array stores the no suffix of the url 
	//rankxS[] array stores the sorted no suffix of the url
	// while(fscanf(fp1, "%s", chararr) != EOF)	{
	// 	char *token = strtok(chararr, "url");
	//     rank1[numItems1] = rank1S[numItems1] = atoi(token);
	//     numItems1++;
	// }

	// while(fscanf(fp2, "%s", chararr) != EOF)	{
	// 	char *token = strtok(chararr, "url");
	// 	rank2[numItems2] = rank2S[numItems2]= atoi(token);
	// 	numItems2++;
	// }

	int numUrlsMax = 0;

	for(i = 0; i < argc-1; i++) {
		if(numUrls[i]>numUrlsMax) numUrlsMax = numUrls[i];
	}
	printf("numUrls: %d\n", numUrlsMax);

	// fclose(fp1);
	// fclose(fp2);

	//Sorting array
	for(i = 0; i < argc-1; i++) {
		sort(mainMatrixS[i], numUrls[i]);
	}

	for(i = 0; i < numUrls[0]; i++) {
		printf("Matrix Sorted: %d\n", mainMatrixS[0][i]);
	}
	// sort(rank1S, numItems1);
	// sort(rank2S, numItems2);

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
	// positions(pos1, rank1, rank1S, numItems1);
	// positions(pos2, rank2, rank2S, numItems2);
	int pos[100][100];

	for(i = 0; i < argc-1; i++) {
		positions(pos[i], mainMatrix[i], mainMatrixS[i], numUrls[i]);
	}

	for(i = 0; i < numUrls[0]; i++) {
		printf("Position: %d\n", pos[0][i]);
	}

	//orderedPosArray creates an array with ordered numbers upto the the max no. of items in the biggest array.
	//e.g. rankA.txt has 5 items rankB.txt has 4 items, therefore orderedPosArray would contain [1,2,3,4,5]
	//Purpose - to pass into permute function and change the order to find the smallest scale foot distance.
	int orderedPosArray[100];
	//int i;
	for(i = 0; i < numUrlsMax; i++) {
		orderedPosArray[i] = i+1;
		printf("orderedPos%d\n", orderedPosArray[i]);
	}

	// //double scaleFootrule = 100;

	permute(orderedPosArray, 0, numUrlsMax, pos, numUrls, argc);
	printf("%lf\n", scaleFootrule);
	
	// for(i = 0; i < numItems; i++) {
	// 	char placeholder[20];
	// 	sprintf(placeholder,"url%d",optimumPos[i]);
	// 	printf("%s\n", placeholder);
	// }

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

void positions(int *pos, int *unsorted, int *sorted, int length) {
	int posNum = 0;

	int i, j;
	for(i = 0; i < length; i++) {
		for(j = 0; j < length; j++) {
			if(sorted[i] == unsorted[j]) {
				pos[posNum++] = j+1;
				break;
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
void permute(int *array,int i,int length, int pos[100][100], int *numUrls, int argc) { 

	double temp = 0;
	int j, l;
	//int k;

  	if (i == length){

  		for(l = 0; l < argc-1; l++) {
  			for(j = 0; j < numUrls[l]; j++) {
  				temp += fabs(((double)pos[l][j]/(double)numUrls[l]) - ((double)array[j]/(double)length));
  			}
  		}

  		

     	// for(j = 0; j < pos1Len; j++) {
     	// 	temp += fabs(((double)pos1[j]/(double)pos1Len) - ((double)array[j]/(double)length));
     	// }

     	// for(j = 0; j < pos2Len; j++) {
     	// 	temp += fabs(((double)pos2[j]/(double)pos2Len - (double)array[j]/(double)length));
     	// }
     	if(temp < scaleFootrule) {
     		scaleFootrule = temp;
     		// for(k = 0; k < length; k++) {
     		// 	if(pos1Len > pos2Len) {
     		// 		optimumPos[k] = pos1[k];
     		// 	} else {
     		// 		optimumPos[k] = pos2[k];
     		// 	}
     			
     		// }

     	}
    	return;
  	}
	for (j = i; j < length; j++) { 
 		swap(array,i,j);
		permute(array,i+1, length, pos, numUrls, argc);
		//swap(array,i,j);
	}
	return;
}

//double footruleCalculator(int *pos, int )

//double posCheck()