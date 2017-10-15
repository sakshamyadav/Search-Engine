// readData.h file

// function prototypes

#ifndef READ_H
#define READ_H

Graph buildGraph();
int found(char *string, int length, char **str);
int position(char *string, int length, char **str);
void freePointer(char **ptr, int length); 

#endif
