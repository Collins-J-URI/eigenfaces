/*  NAME:
        _ImageReader_hack.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _ImageReader_hack class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_HACK_RW


#include <cstdlib>
#include <cstring>
#include <cstdio>
//
#include "_ImageReader_hack.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace uriVL;
using namespace std;

const char* kReadExtensions_[3] = {
										"tga",
										"ppm",
										"pgm"
									};
int readExtensionIndex(const char* ext);


//----------------------------------------------------------------------------------
//-------------------------------Class implementation-------------------------------
//


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

_ImageReader_hack::_ImageReader_hack(void)
	:	filePath_(NULL)
{
    filePath_ = NULL;
}

_ImageReader_hack::_ImageReader_hack(const char* theFilePath)
	:	filePath_(NULL)
{
    setSource_(theFilePath);
}

_ImageReader_hack::_ImageReader_hack(const _ImageReader_hack& ovbj)
	:	filePath_(NULL)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_ImageReader_hack copy constructor not implemented.");
}

_ImageReader_hack::~_ImageReader_hack(void)
{
    if (filePath_ != NULL)
        delete []filePath_;
}

const _ImageReader_hack& _ImageReader_hack:: operator = (const _ImageReader_hack& ovbj)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_ImageReader_hack copy operator not implemented.");
	return *this;
}


#if 0
#pragma mark -
#endif

//  This function should let the user select a file through a dialog window.
//  Will be implemented when I complete the QT "takeover"
void _ImageReader_hack::setSource_(void)
{
    FAIL_CONDITION( true,
                    kFileIOError,
                    "file selection dialog not implemented yet");
}

//  Sets the source (image file) by specifying a path
void _ImageReader_hack::setSource_(const char* thePath)
{
    filePath_ = NULL;

    FAIL_CONDITION( thePath == NULL,
                    kFileNullPathError,
                    "null file path send as parameter");

    const   int l = strlen(thePath) + 1;
    filePath_ = new char[l];
    strcpy(filePath_, thePath);
}

#if 0
#pragma mark -
#endif

RasterImage* _ImageReader_hack::readImage_(bool readUpsideDown)
{
	FAIL_CONDITION( true,
	                kFunctionNotImplemented,
	                "function readImage_(bool readUpsideDown) not implemented yet");
    return NULL;
}


RasterImage* _ImageReader_hack::readImage_(const char* filePath, bool readUpsideDown)
{
	#if URIVL_USE_HACK_RW
		//	As a Mac programmer, I am really embarassed to be using something as
		//	crude as the "file extension" to find out what image format I am dealing
		//	with, but I lack the time to do something better.  Still, it's an ugly hack.
		int pathLength = strlen(filePath);
		const char* extension = filePath + (pathLength - 3);

		switch (readExtensionIndex(extension))
		{
			case kTGA:
				return readTGA_(filePath, readUpsideDown);
				break;

			case kPPM:
			case kPGM:
				return readPPM_(filePath, readUpsideDown);
				break;

			default:
				char    myError[256];
				sprintf(myError, "Unknown file extension: %s", filePath);
				FAIL_CONDITION( true,
								kUnknownFileTypeError,
								myError);
				return NULL;
				break;
		}
	#elif URIVL_USE_FREEIMAGE
	
	
	#else
	
		#error no image reader 
		
	#endif
}


RasterImage* _ImageReader_hack::getFrame_(bool readUpsideDown)
{
	FAIL_CONDITION( filePath_ == NULL,
					kVideoSourceNotSetError,
					"Source file path not defined prior to call to _ImageReader_hack::getFrame_");

	return readImage_(filePath_, readUpsideDown);
}


void _ImageReader_hack::getFrame_(RasterImage* imgOut, bool readUpsideDown)
{
	FAIL_CONDITION( filePath_ == NULL,
					kVideoSourceNotSetError,
					"Source file path not defined prior to call to _ImageReader_hack::getFrame_");

	try
	{
		RasterImage* imgTemp = readImage_(filePath_, readUpsideDown);
		imgTemp->copyInto(imgOut);
		delete imgTemp;
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by _ImageReader_hack::getFrame_");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in _ImageReader_hack::getFrame_");
	}
}


#if 0
#pragma mark -
#endif



void _ImageReader_hack::getHeaderInfoPPM_(FILE *inPPM, const char* filePath, BaseRasterType *theType,
								bool *isAscii, int* nbRows, int* nbCols, int* maxVal)
{
	int		tempRows, tempCols, tempMax;
	char	buf[80], skip[80], imgCode[2], c;
	bool	commentsSkipped = false;

	// --- find if we have a known picture format ---
	fscanf(inPPM, "%s", buf);
	imgCode[0] = buf[0];
	imgCode[1] = buf[1];

	FAIL_CONDITION ((imgCode[0] != 'P') || ((imgCode[1] != '2') && (imgCode[1] != '5') &&
								(imgCode[1] != '3') && (imgCode[1] != '6')),
					kWrongFileTypeError,
					"Wrong image prefix code for PPM-PGM");

	//	Get the images type
	switch (imgCode[1])
	{
		case '2':
		case '5':
			*theType = kGrayRaster;
			break;

		case '3':
		case '6':
			*theType = kRGBa32Raster;
			break;

		default:
			break;

	}

	//	Note: I am doing some really ugly stuff with file input and buffers.
	//	I am not sure if I had to do that because ANSI file I/O libraries
	//	on the Mac are screwed up (they might be, for all I know), or just
	//	because I kept missing something important somewhere.

	//	skip whitespace
	fscanf(inPPM, "%[\n\r\t ]", skip);
	//	read until we encounter a non-whitespace character.  This could be a $#@# comment
	//	or the nb of columns
	fscanf(inPPM, "%[^\n\r\t ]", buf);

	if (buf[0] != '#')
		commentsSkipped = true;

	while (!commentsSkipped)
	{
		//	if we stepped into  some comment...
		if (buf[0] == '#')
		{
			//	...skip the rest of the line
			fscanf(inPPM, "%[^\n\r]", skip);

			//	skip additional whitespace on the next line
			fscanf(inPPM, "%[\n\r\t ]", skip);

			//	then read until the next whitespace
			fscanf(inPPM, "%[^\n\r\t ]", buf);
		}
		else
			commentsSkipped = true;
	}

	//	buf should contain only one thing: the width of the image
	sscanf(buf,"%d", &tempCols);
	*nbCols = static_cast<int>(tempCols);

	//	the rest of the info is read from the file stream
	fscanf(inPPM,"%d", &tempRows);
	fscanf(inPPM,"%d", &tempMax);
	*nbRows = static_cast<int>(tempRows);
	*maxVal = static_cast<int>(tempMax);

	//	Read the extra space
	fscanf(inPPM, "%c", &c);

	//	If the code was P2 or P3, then the image file is in the ASCII format
	if ((imgCode[1] == '2') || (imgCode[1] == '3'))
		*isAscii = true;
	else
		*isAscii = false;

}

RasterImage* _ImageReader_hack::readPPM_(const char* filePath, bool readUpsideDown)
{
	RasterImage		*myImage;
	BaseRasterType	theCode;
	//
	FILE			*inPPM;
	float			valScale=1.f;
	int				theGray,
					theRed, theGreen, theBlue;
	int			    nbRows,
	                nbCols,
					maxVal, bytesPerRow;
//	int             i, j, k, l;
	bool			isASCII = false;

	//	reset dimensions (just to play it safe)
	nbCols = 0;
	nbRows = 0;

	// --- open file ---
	inPPM = fopen(filePath, "r");
	FAIL_CONDITION(	inPPM == NULL,
					kFileNotFoundError,
					"Input image not found");

	getHeaderInfoPPM_(inPPM, filePath, &theCode, &isASCII, &nbRows, &nbCols, &maxVal);
	unsigned long   ulNbCols = static_cast<unsigned long>(nbCols);
    

	//	We only want to handle values in the [0, 255] range
	if (maxVal != 255)
		valScale = 255.f/maxVal;


	//--------------------------------
	//	Case of a gray-level image
	//--------------------------------
	switch (theCode)
	{
	    case kGrayRaster:
		{
        	myImage = new RasterImage_gray(nbRows, nbCols);
        	unsigned char* const* gray = myImage->getRaster2D(R_W_ACCESS);

    		//	The image is stored in an ASCII format
    		if (isASCII)
    		{
    			//	the image is read , row by row, upside down, directly into the raster
    			if (readUpsideDown)
    			{
					if (maxVal == 255)
            		    for (int i=nbRows-1; i>=0; i--)
            		    {
            	            unsigned char* grayRow = gray[i];
            		        for (int j=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d", &theGray);
        						grayRow[j] = static_cast<unsigned char>(theGray);
                       	    }
                       	}
    				else
            		    for (int i=nbRows-1; i>=0; i--)
            		    {
            	            unsigned char* grayRow = gray[i];
            		        for (int j=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d", &theGray);
        						grayRow[j] = static_cast<unsigned char>(valScale*theGray);
                       	    }
                       	}
        		}
    			//	the image is not read upside down
        		else
        		{
					if (maxVal == 255)
            		    for (int i=0; i<nbRows; i++)
            	        {
            	            unsigned char* grayRow = gray[i];
            		        for (int j=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d", &theGray);
        						grayRow[j] = static_cast<unsigned char>(theGray);
                       	    }
                    	}
                    else
            		    for (int i=0; i<nbRows; i++)
            	        {
            	            unsigned char* grayRow = gray[i];
            		        for (int j=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d", &theGray);
         						grayRow[j] = static_cast<unsigned char>(valScale*theGray);
                       	    }
                    	}
        		}
    		}
    		//	The image is stored in a binary format
    		else
    		{
    			//	the image is read upside down
    			if (readUpsideDown)
    			{
        		    for (int i=nbRows-1; i>=0; i--)
        		        fread(gray[i], 1, ulNbCols, inPPM) ;

        		}
    			//	the image is not read upside down
        		else
        		{
        		    for (int i=0; i<nbRows; i++)
        		        fread(gray[i], 1, ulNbCols, inPPM) ;

        		}
    		}
		}
		break;


    	//--------------------------------
    	//	Case of a color image
    	//--------------------------------
    	case kRGBa32Raster:
		{
	        bytesPerRow = 3*nbCols;
        	myImage = new RasterImage_RGBa(nbRows, nbCols);
            unsigned char* const* rgba = myImage->getRaster2D(R_W_ACCESS);

    		//	The image is stored in an ASCII format
    		if (isASCII)
    		{
    			//	the image is read upside down
    			if (readUpsideDown)
    			{
					if (maxVal == 255)
            		    for (int i=nbRows-1; i>=0; i--)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            		        for (int j=0, k=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d%d%d", &theRed, &theGreen, &theBlue);
         						rgbaRow[k++] = static_cast<unsigned char>(theRed);
    							rgbaRow[k++] = static_cast<unsigned char>(theGreen);
    							rgbaRow[k++] = static_cast<unsigned char>(theBlue);
    							rgbaRow[k++] = static_cast<unsigned char>(255);
                       	    }
                       	}
					else
            		    for (int i=nbRows-1; i>=0; i--)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            		        for (int j=0, k=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d%d%d", &theRed, &theGreen, &theBlue);
         						rgbaRow[k++] = static_cast<unsigned char>(valScale*theRed);
    							rgbaRow[k++] = static_cast<unsigned char>(valScale*theGreen);
    							rgbaRow[k++] = static_cast<unsigned char>(valScale*theBlue);
    							rgbaRow[k++] = static_cast<unsigned char>(255);

                       	    }
                       	}
    			}
    			//	the image is not read upside down
    			else
    			{
					if (maxVal == 255)
            		    for (int i=0; i<nbRows; i++)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            		        for (int j=0, k=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d%d%d", &theRed, &theGreen, &theBlue);
         						rgbaRow[k++] = static_cast<unsigned char>(theRed);
    							rgbaRow[k++] = static_cast<unsigned char>(theGreen);
    							rgbaRow[k++] = static_cast<unsigned char>(theBlue);
    							rgbaRow[k++] = static_cast<unsigned char>(255);
                       	    }
                       	}
					else
            		    for (int i=0; i<nbRows; i++)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            		        for (int j=0, k=0; j<nbCols; j++)
                	        {
        						fscanf(inPPM, "%d%d%d", &theRed, &theGreen, &theBlue);
         						rgbaRow[k++] = static_cast<unsigned char>(valScale*theRed);
    							rgbaRow[k++] = static_cast<unsigned char>(valScale*theGreen);
    							rgbaRow[k++] = static_cast<unsigned char>(valScale*theBlue);
    							rgbaRow[k++] = static_cast<unsigned char>(255);

                       	    }
                       	}
    			}
    		}
    		//	The image is stored in a binary format
    		else
    		{
            	unsigned char* theRow = new unsigned char[bytesPerRow];

        		if (readUpsideDown)
        		{
					if (maxVal == 255)
            		    for (int i=nbRows-1; i>=0; i--)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            				fread(theRow, 3*sizeof(char), ulNbCols, inPPM ) ;
                        	for(int k=0, l=0; k<bytesPerRow; )
                        	{
                        	    rgbaRow[l++] = theRow[k++];
                        	    rgbaRow[l++] = theRow[k++];
                        	    rgbaRow[l++] = theRow[k++];
                        	    rgbaRow[l++] = static_cast<unsigned char>(255);
                        	}
                       	}
					else
            		    for (int i=nbRows-1; i>=0; i--)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            				fread(theRow, 3*sizeof(char), ulNbCols, inPPM ) ;
                        	for(int k=0, l=0; k<bytesPerRow; )
                        	{
                        	    rgbaRow[l++] = static_cast<unsigned char>(valScale*theRow[k++]);
                        	    rgbaRow[l++] = static_cast<unsigned char>(valScale*theRow[k++]);
                        	    rgbaRow[l++] = static_cast<unsigned char>(valScale*theRow[k++]);
                        	    rgbaRow[l++] = static_cast<unsigned char>(255);
                        	}
                       	}
                }
        		else
        		{
					if (maxVal == 255)
        		        for (int i=0; i<nbRows; i++)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            				fread(theRow, 3*sizeof(char), ulNbCols, inPPM ) ;
                        	for(int k=0, l=0; k<bytesPerRow; )
                        	{
                        	    rgbaRow[l++] = theRow[k++];
                        	    rgbaRow[l++] = theRow[k++];
                        	    rgbaRow[l++] = theRow[k++];
                        	    rgbaRow[l++] = static_cast<unsigned char>(255);
                        	}
                       	}
					else
        		        for (int i=0; i<nbRows; i++)
            		    {
            		        unsigned char* rgbaRow = rgba[i];
            				fread(theRow, 3*sizeof(char), ulNbCols, inPPM ) ;
                        	for(int k=0, l=0; k<bytesPerRow; )
                        	{
                        	    rgbaRow[l++] = static_cast<unsigned char>(valScale*theRow[k++]);
                        	    rgbaRow[l++] = static_cast<unsigned char>(valScale*theRow[k++]);
                        	    rgbaRow[l++] = static_cast<unsigned char>(valScale*theRow[k++]);
                        	    rgbaRow[l++] = static_cast<unsigned char>(255);
                        	}
                       	}
        		}

        		delete []theRow;
    		}
		}
		break;


    	//--------------------------------
    	//	Bo other case accepted
    	//--------------------------------
    	default:
    	    break;

	}

	fclose(inPPM);

	myImage->setValidRect(0, 0, nbCols, nbRows);
	return myImage;
}




#if 0
#pragma mark -
#endif

RasterImage* _ImageReader_hack::readTGA_(const char* filePath, bool readUpsideDown)
{
	RasterImage*	myImage;
	BaseRasterType	theCode;
	//
	FILE*			inTGA;
	int				nbRows, nbCols,
					bytesPerPixel, bytesPerRow;
	char			head[18];
	bool			doUpsideDown = false;

	/* --- open TARGA input file ---------------------------------- */
	inTGA = fopen(filePath, "rb" );

	FAIL_CONDITION( inTGA == NULL,
	                kFileOpenReadError,
	                "Cannot open pixel file");

	/* --- Read the header (TARGA file) --- */
	fread(head, sizeof(char), 18, inTGA) ;
	/* Get the size of the image */
	nbCols = static_cast<int>(static_cast<unsigned int>(head[12]&0xFF) | 
                              static_cast<unsigned int>(head[13]*256));
	nbRows = static_cast<int>(static_cast<unsigned int>(head[14]&0xFF) | 
                              static_cast<unsigned int>(head[15]*256));
    unsigned long ulNbCols = static_cast<unsigned long>(nbCols);
    
	if((head[2] == 2) && (head[16] == 24))
	{
		theCode = kRGB24Raster;
		bytesPerPixel = 3;
		bytesPerRow = 3*nbCols;
	}
	else if((head[2] == 3) && (head[16] == 8))
	{
		theCode = kGrayRaster;
		bytesPerPixel = 1;
		bytesPerRow = nbCols;
	}
	else
	{
//		printf("Unsuported TGA image: ");
//		printf("Its type is %d and it has %d bits per pixel.\n", head[2], head[16]);
//		printf("The image must be uncompressed while having 8 or 24 bits per pixel.\n");
//		fclose(inTGA);
//		std::exit(kWrongFileTypeError);
		FAIL_CONDITION( true,
						kWrongFileTypeError,
						"Unsuported TGA image: The image must be uncompressed while having 8 or 24 bits per pixel");
	}

	if (readUpsideDown)
		doUpsideDown =  static_cast<bool>(!(head[17]&0x20));
	else
		doUpsideDown =  static_cast<bool>(head[17]&0x20);


	if (bytesPerPixel == 3)
	{
    	unsigned char   *rgbaRow;
    	unsigned char* theRow = new unsigned char[bytesPerRow];
    	myImage = new RasterImage_RGBa(nbRows, nbCols);
    	unsigned char* const* rgba = myImage->getRaster2D(R_W_ACCESS);

		if(doUpsideDown)
		{
		    for (int i=nbRows-1; i>=0; i--)
	        {
	            rgbaRow = rgba[i];
				fread(theRow, 3*sizeof(char), ulNbCols, inTGA ) ;
            	for(int k=0, l=0; k<bytesPerRow; k+=3)
            	{
            	    rgbaRow[l++] = theRow[k+2];
            	    rgbaRow[l++] = theRow[k+1];
            	    rgbaRow[l++] = theRow[k];
            	    rgbaRow[l++] = static_cast<unsigned char>(255);
            	}
        	}
		}
		else
		{
		    for (int i=0; i<nbRows; i++)
	        {
	            rgbaRow = rgba[i];
				fread(theRow, 3*sizeof(char), ulNbCols, inTGA ) ;
            	for(int k=0, l=0; k<bytesPerRow; k+=3)
            	{
            	    rgbaRow[l++] = theRow[k+2];
            	    rgbaRow[l++] = theRow[k+1];
            	    rgbaRow[l++] = theRow[k];
            	    rgbaRow[l++] = static_cast<unsigned char>(255);
            	}
        	}
		}
	}
	else
	{
    	myImage = new RasterImage_gray(nbRows, nbCols);
    	unsigned char* const* gray = myImage->getRaster2D(R_W_ACCESS);

		if (doUpsideDown)
		{
		    for (int i=nbRows-1; i>=0; i--)
				fread(gray[i], sizeof(char), ulNbCols, inTGA ) ;

        }
		else
		{
		    for (int i=0; i<nbRows; i++)
				fread(gray[i], sizeof(char), ulNbCols, inTGA ) ;

		}
	}

	fclose( inTGA) ;
	myImage->setValidRect(0, 0, nbCols, nbRows);

	return myImage;
}



#if 0
#pragma mark -
#endif

int readExtensionIndex(const char* ext)
{
    for (int i = 0; i < kNbImageFileExtensions; i++)
        if (strncmp(ext, kReadExtensions_[i], strlen(kReadExtensions_[i])) == 0)
            return (i);

    return (-1);
}



#endif // use URIVL_USE_HACK_RW

