#ifndef	FILE_IO_PPM_H
#define	FILE_IO_PPM_H

#include "fileIO.h"

unsigned char *readPPM(char *fileName, int *nbRows, int *nbCols, eImageFileType *theType);
unsigned char *readPPM_(char *fileName, bool upsideDown, int *nbRows, int *nbCols,
							eImageFileType *theType);
eFileErrorCode writePPM(char *fileName, unsigned char *image, int nbRows, int nbCols);
eFileErrorCode writePPM_(char *fileName, unsigned char *image, int nbRows, int nbCols,
							bool upsideDown);

#endif