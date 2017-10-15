// searchPagerank.h file

// function prototypes

#ifndef SEARCH_H
#define SEARCH_H

void normaliseInput(char *ch);
void getFreq(char **arr, int length);
void order(char **arr, int *freq, int length);
void consolePrint(char **arr, int *freq, int length);
int getMax(int *freq, int length);
int getDistinct(int *freq, int length);
void sortpageRank(char **arr, int length);

#endif
