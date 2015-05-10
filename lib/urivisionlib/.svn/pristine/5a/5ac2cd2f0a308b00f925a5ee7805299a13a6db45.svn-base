/*  NAME:
        ImageWarper_GL.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageWarper_GL class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "ImageWarper_GL.h"
#include "RasterImage.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;

//#define DEBUG

//----------------------------------------------------------
//  constructors & destructor
//----------------------------------------------------------

ImageWarper_GL::ImageWarper_GL(InnerWarpingMode innerMode,
                               BorderWarpingMode outerMode)
    :   ImageWarper(GL_WARPER, innerMode, outerMode),
		//
		textureID_(0),
		texturePoint_(NULL)
{
}


ImageWarper_GL::ImageWarper_GL(const ImageWarper_GL& theObj)
    :   ImageWarper(GL_WARPER),
		//
		textureID_(0),
		texturePoint_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageWarper_GL copy constructor not implemented.");
}
//catch (ErrorReport& e) {
//	e.appendToMessage("called by ImageWarper_GL constructor");
//	throw e;
//}



ImageWarper_GL::~ImageWarper_GL(void)
{
    if (textureID_ != 0)
    {
    	for (int i=0; i<gridRows_; i++)
        {		
    		for (int j=0; j<gridCols_; j++)
    			delete []texturePoint_[i][j];
    		
    		delete []texturePoint_[i];
        }
    	delete []texturePoint_;
	}	

    if (textureID_ > 0)
        glDeleteTextures (1, &textureID_);

}

const ImageWarper_GL& ImageWarper_GL::operator = (const ImageWarper_GL& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageWarper_GL copy operator not implemented.");

	return *this;
}


//----------------------------------------------------------
//  public funcs
//----------------------------------------------------------
			
void ImageWarper_GL::setImages(const RasterImage* imgIn, RasterImage* imgOut)
{
    ImageWarper::setImages(imgIn, imgOut);

    glEnable(GL_TEXTURE_2D);
    
    if (textureID_ > 0)
        glDeleteTextures (1, &textureID_);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer	

	//	gets a free identificator for the texture
	glGenTextures(1, &textureID_);	
	glBindTexture(GL_TEXTURE_2D, textureID_);

    //int n = 256;
	/*
	imgIn1D_ = new unsigned char [n*n*4];
	long    idx = 0;
	for (short i=0; i<n; i++)
	{
		idx = 4 * n * i;
		
		for (short j=0; j<n; j++)
		{
			imgIn1D_[idx++] = (unsigned char) floor((i-n/2.)*(i-n/2.)/120. + 20);
			imgIn1D_[idx++] = (unsigned char) j/4 + 30;
			imgIn1D_[idx++] = (unsigned char) floor(127*(cos(0.06*i)*sin(0.1*j)+0.5));
			imgIn1D_[idx++] = (unsigned char) 255;
				
		}
	}
	*/
	switch (rasterType_)
	{
		case kGrayRaster:
			glTexImage2D(GL_TEXTURE_2D, 0, 	GL_LUMINANCE,
											imgHeight_,
											imgHeight_,
											0,
											GL_LUMINANCE, GL_UNSIGNED_BYTE,
											imgIn1D_);
			break;
			
		case kRGBa32Raster:
			glTexImage2D(GL_TEXTURE_2D, 0, 	GL_RGBA,
											imgHeight_,
											imgHeight_,
											0,
											GL_RGBA, GL_UNSIGNED_BYTE,
											imgIn1D_);
			break;
			
		default:
			break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if ((gridRows_ != 0) && (gridCols_ != 0))
	    setGrid(gridRows_, gridCols_);
	    
}


void ImageWarper_GL::setGrid(int gridRows, int gridCols)
{
    ImageWarper::setGrid(gridRows, gridCols);	

    float scaleU = 1.f;    
    if (imgIn1D_ != NULL)
        scaleU = 1.f * imgWidth_ / imgHeight_;
        
		
    texturePoint_ = new	GLfloat**[gridRows_];		
	for (int i=0; i<gridRows_; i++)
    {		
        texturePoint_[i] = new	GLfloat*[gridCols_];		
		for (int j=0; j<gridCols_; j++) 
		{
			texturePoint_[i][j] = new GLfloat[2];
		    texturePoint_[i][j][0] = scaleU * j / (gridCols_ -  1);
		    texturePoint_[i][j][1] = 1.f * i / (gridRows_ -  1);
        }				
	}			
}
			
			
void ImageWarper_GL::getImageSuitableDimensions(int imgWidth, int imgHeight,
                                                int* lowImgWidth, int* lowImgHeight,
                                                int* highImgWidth, int* highImgHeight) const
{
    int tempDim, lowWidth, lowHeight;
    
    //  We find the largest power of two lower than or equal to the input width
    tempDim = imgWidth;
    lowWidth = 1;
    tempDim--;
    while (tempDim != 0)
    {
        tempDim /= 2;
        lowWidth *= 2;
    }
    
    //  We do the same thing for the height
    tempDim = imgHeight,
    lowHeight = 1;
    tempDim--;
    while (tempDim != 0)
    {
        tempDim /= 2;
        lowHeight *= 2;
    }

    //  We determine the low and high height dimensions
    *lowImgHeight = lowHeight;
    if (imgHeight == lowHeight)
        *highImgHeight = lowHeight;
    else
        *highImgHeight = 2*lowHeight;

    //  Now, depending on the platform on which we run, we will have different
    //  lower and upper width
    //
    //  QuickTime gworlds are padded on the Mac side
    #if URIVL_USE_QUICKTIME && URIVL_OS_MACOS
        *lowImgWidth = lowWidth - 4;
        *highImgWidth = 2*lowWidth - 4;
    #else
        *lowImgWidth = lowWidth;
        if (imgHeight == lowWidth)
            *highImgWidth = lowWidth;
        else
            *highImgWidth = 2*lowWidth;
    #endif
}


//----------------------------------------------------------
//  Warping functions
//----------------------------------------------------------
#if 0
#pragma mark -
#endif

void ImageWarper_GL::warp_(float*** xy)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer	

	glEnable(GL_TEXTURE_2D);	
	glBindTexture(GL_TEXTURE_2D, textureID_);			
	
	glColor4f(1.f, 1.f, 1.f, 1.f);

	//	map the texture to each square
	for (int i=0; i<gridRows_ - 1; i++)
	{
		for (int j=0; j<gridCols_ - 1; j++)
		{
			glBegin(GL_POLYGON);
				glTexCoord2fv(texturePoint_[i][j]);
				glVertex2fv(xy[i][j]);
				glTexCoord2fv(texturePoint_[i+1][j]);
				glVertex2fv(xy[i+1][j]);
				glTexCoord2fv(texturePoint_[i+1][j+1]);
				glVertex2fv(xy[i+1][j+1]);
				glTexCoord2fv(texturePoint_[i][j+1]);
				glVertex2fv(xy[i][j+1]);
			glEnd();
		}
	}

	// read from front buffer
	glReadBuffer(GL_BACK);

	switch (rasterType_)
	{
		case kGrayRaster:
			// read pixel data
			glReadPixels(0, 0, imgHeight_, imgHeight_, GL_LUMINANCE, GL_UNSIGNED_BYTE, imgOut1D_);
			break;
			
		case kRGBa32Raster:
			// read pixel data
			glReadPixels(0, 0, imgHeight_, imgHeight_, GL_RGBA, GL_UNSIGNED_BYTE, imgOut1D_);
			break;
			
		default:
			break;
	}
	
	glDisable(GL_TEXTURE_2D);	
}


int ImageWarper_GL::getEffectivePixelsPerRow(int nominalWidth) const
{
    int tempDim, loWidth, hiWidth, effLoWidth, effHiWidth;
    
    //  We find the largest power of two lower than or equal to the input width
    tempDim = nominalWidth;
    effLoWidth = 1;
    tempDim--;
    while (tempDim != 0)
    {
        tempDim /= 2;
        effLoWidth *= 2;
    }
    effHiWidth = 2*effLoWidth;

    #if URIVL_USE_QUICKTIME && URIVL_OS_MACOS
        hiWidth = effHiWidth - 4;
        loWidth = effLoWidth - 4;
    #else
        hiWidth = effHiWidth;
        loWidth = effLoWidth;
    #endif

    if (nominalWidth == loWidth)
        return effLoWidth;
    else if (nominalWidth == hiWidth)
        return effHiWidth;
    else
        return -1;
        
}
