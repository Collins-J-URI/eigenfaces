/*  NAME:
        ImagePyramid.h
 
    DESCRIPTION:
        ImagePyramid public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_PYRAMID_H
#define URIVL_IMAGE_PYRAMID_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"

namespace uriVL
{
    /** An ImagePyramid object stores an array of RasterImage objects that
     *  represent the image at different scales, scale 0 being a copy of the
     *  original image and scale i being a 2^i scaled down version.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class ImagePyramid
    {
            //
            //  Constructors and destructor 
            //
            ImagePyramid(unsigned char* theRaster, short theNbRows, short theNbCols,
                                eImageTypeCode theType);
            ImagePyramid(short theNbRows, short theNbCols, eImageTypeCode theType);
            ImagePyramid(float* theflImg, short theNbRows, short theNbCols, 
                                eFloatConvType convType);
            ImagePyramid(RasterImage* theImg);
            ImagePyramid(ImagePyramid *thePyramid);
            ImagePyramid(float* theRastX, float* theRastY, short theNbRows, short theNbCols);
            ~ImagePyramid(void);
            //
            //  Public methods
            //
    /*
            void deleteRasters(ImagePyramid *theImg);
            void copyInto(ImagePyramid *imgOut);
            void copyInto(ImagePyramid *imgOut, sSearchRect *theRect);
            void copyInto(ImagePyramid *imgOut, eImageTypeCode desiredType);
            void copyInto(ImagePyramid *imgOut, eImageTypeCode desiredType, sSearchRect *theRect);
    */
            short getNbRows(short theLevel);
            short getNbCols(short theLevel);
            //
            void    render(short theLevel);
            void    render(short theLevel, float Z);
            unsigned char* getRaster(short theLevel);   
            unsigned char* getRGBA(short theLevel);
            unsigned char* getRGB(short theLevel);
            unsigned char* getGray(short theLevel);
            float   *getFloat(short theLevel);
            //
            unsigned char** getGray2D(short theLevel);
            unsigned char** getRGB2D(short theLevel);
            unsigned char** getRGBA2D(short theLevel);
            float** getFloat2D(short theLevel);
            //
            unsigned char** getHSV(short theLevel);
            unsigned char** getHSV(short theLevel, sSearchRect *theRect);
            unsigned char*** getHSV2D(short theLevel);
            unsigned char*** getHSV2D(short theLevel, sSearchRect *theRect);
            //
            void getMinMax(short theLevel, unsigned char* theMin, unsigned char* theMax);
            void getMinMax(short theLevel, float* theMin, float* theMax);
            void getMinMax(short theLevel, float* theMinX, float* theMaxX, 
                                            float* theMinY, float* theMaxY);
            //

        protected:
            short       nbLevels;
            RasterImage** imgAtLevel;

    };
}

#endif  //  IMAGE_PYRAMID_H