#ifndef	FILE_IO_SGI_H
#define	FILE_IO_SGI_H

#include "fileIO.h"

unsigned char* readSgiRgbImage(char* infn, int* m, int* n);
int writeSgiRgbImage(unsigned char* img, int m, int n, char* outfn);


#endif