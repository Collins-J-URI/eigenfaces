#include <stdlib.h>        
#include <fstream>
#include <iostream>

#include "fileIO_TGA.h"

using namespace std;

							

void swapRGB(unsigned char *theData, int nbRows, int nbCols);


//----------------------------------------------------------------------
//	Utility function for memory swapping
//	Used because TGA stores the RGB data in reverse order (BGR)
//----------------------------------------------------------------------
void swapRGB(unsigned char *theData, int nbRows, int nbCols)
{
	int			imgSize;
	unsigned char tmp;

	imgSize = nbRows*nbCols;

	for(int k = 0; k < imgSize; k++)
	{
		tmp = theData[k*3+2];
		theData[k*3+2] = theData[k*3];
		theData[k*3] = tmp;
	}
}



// ---------------------------------------------------------------------
//	Function : readTGA 
//	Description :
//	
//	This function reads an image of type TGA (8 or 24 bits, uncompressed
//	
//	Parameters:
//
//	*fileName - Pointer to a string that contains the name of the file
//	*nbCols, *nbRows - Dimensions XxY of the image
//	
//	Return value: pointer to the pixel data
//----------------------------------------------------------------------

unsigned char *readTGA(const char *fileName, int *nbCols, int *nbRows, eImageFileType *theType)
{
	unsigned char *theData, *ptr;
	int		imgSize;
	char	head[18] ;
	FILE	*tga_in;

	/* --- open TARGA input file ---------------------------------- */
	tga_in = fopen(fileName, "rb" );

	if (tga_in == NULL )
	{
		printf("Cannot open pixel file %s \n", fileName);
		std::exit(kCannotOpenRead);
	}

	/* --- Read the header (TARGA file) --- */
	fread( head, sizeof(char), 18, tga_in ) ;
	/* Get the size of the image */
	*nbCols = (int)((unsigned int)head[12]&0xFF | (unsigned int)head[13]*256);
	*nbRows = (int)((unsigned int)head[14]&0xFF | (unsigned int)head[15]*256);


	if((head[2] == 2) && (head[16] == 24))
		*theType = kTGA_COLOR;
	else if((head[2] == 3) && (head[16] == 8))
		*theType = kTGA_GRAY;
	else
	{
		printf("Unsuported TGA image: ");
		printf("Its type is %d and it has %d bits per pixel.\n", head[2], head[16]);
		printf("The image must be uncompressed while having 8 or 24 bits per pixel.\n");
		fclose(tga_in);
		std::exit(kWrongFileType);
	}

	imgSize = *nbCols * *nbRows;
	/* Create the buffer for image */

	if (*theType == kTGA_COLOR)
		theData = new unsigned char[imgSize*3];
	else
		theData = new unsigned char[imgSize];

	if(theData == NULL)
	{
		printf("Unable to allocate memory\n");
		fclose(tga_in);
		std::exit(kMemAllocError);
	}

	/* Check if the image is vertically mirrored */
	if (*theType == kTGA_COLOR)
	{
		if(head[17]&0x20)
		{
			ptr = theData + imgSize*3 - ((*nbCols)*3);  
			for(int i = 0; i < *nbRows; i++)
			{
				fread( ptr, 3*sizeof(char), *nbCols, tga_in ) ;
				ptr -= (*nbCols)*3;
			}
		}
		else
			fread(theData, 3*sizeof(char), imgSize, tga_in);

        //  tga files store color information in the order B-G-R
        //  we need to swap the Red and Blue components
    	swapRGB(theData, *nbRows, *nbCols);
	}
	else
	{
		if(head[17]&0x20)
		{
			ptr = theData + imgSize - *nbCols;  
			for(int i = 0; i < *nbRows; i++)
			{
				fread( ptr, sizeof(char), *nbCols, tga_in ) ;
				ptr -= *nbCols;
			}
		}
		else
		fread(theData, sizeof(char), imgSize, tga_in);
	}

	fclose( tga_in) ;
	return(theData);
}	


//---------------------------------------------------------------------*
//	Function : writeTGA 
//	Description :
//	
//	 This function write out an image of type TGA (24-bit color)
//	
//	 Parameters:
//	
//	  *fileName - Pointer to a string that contains the name of the file
//	  nbCols, nbRows - Dimensions XxY of the image
//	  *data - pointer to the array containing the pixels. This array
//	          is of type char {r, g, b, r, g, b, ... }
//
//	Return value: Error code (0 = no error)
//----------------------------------------------------------------------*/ 
int writeTGA(const char *fileName, const unsigned char *theData, int nbRows, int nbCols)
{
	int		i,j;
	long	offset;
	int		swapflag = 1;
	char	head[18] ;
	FILE	*tga_out;
  
	/* --- open TARGA output file ---------------------------------- */

	tga_out = fopen(fileName, "wb" );

	if ( !tga_out )
	{
		printf("Cannot create pixel file %s \n", fileName);
		return(kCannotOpenWrite);
	}

	// --- create the header (TARGA file) ---
	head[0]  = 0 ;		  					// ID field length.
	head[1]  = 0 ;		  					// Color map type.
	head[2]  = 2 ;		  					// Image type: true color, uncompressed.
	head[3]  = head[4] = 0 ;  				// First color map entry.
	head[5]  = head[6] = 0 ;  				// Color map lenght.
	head[7]  = 0 ;		  					// Color map entry size.
	head[8]  = head[9] = 0 ;  				// Image X origin.
	head[10] = head[11] = 0 ; 				// Image Y origin.
	head[13] = (char) (nbCols >> 8) ;		// Image width.
	head[12] = (char) (nbCols & 0x0FF) ;
	head[15] = (char) (nbRows >> 8) ;		// Image height.
	head[14] = (char) (nbRows & 0x0FF) ;
	head[16] = 24 ;		 					// Bits per pixel.
	head[17] = 0 ;		  					// Image descriptor bits ;
	fwrite( head, sizeof(char), 18, tga_out );

	for(i = 0; i < nbRows; i++)
	{
		offset = i*3*nbCols;
		for(j = 0; j < nbCols; j++)
		{
			fwrite(&theData[offset+2], sizeof(char), 1, tga_out);
			fwrite(&theData[offset+1], sizeof(char), 1, tga_out);
			fwrite(&theData[offset], sizeof(char), 1, tga_out);
			offset+=3;
		}
	}

	fclose( tga_out ) ;

	return(kNoIOerror);
}	

