// inverted.h file

#ifndef INVERT_H
#define INVERT_H

//prototype functions for inverted.c 
int duplicates(char **str, char *in, int size);
void normalise(char *ch);
void writetoFile(char **str, char **url, int length, int urlLength);
void sortAlpha(char **str, int length);

#endif