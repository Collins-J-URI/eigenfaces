/*  NAME:
        LabelRaster.h
 
    DESCRIPTION:
        LabelRaster public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_LABEL_RASTER_H
#define     URIVL_LABEL_RASTER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"

namespace uriVL
{
    /** Base label raster class.
     * 
     *  LabelRaster is inspired from LabelRaster (LabelRaster_gray in particular).
     *  it simply stores an array of 8-bit labels and provides acces functions
     *  to this array.
     *
     *  @author     Fang Han and jean-yves herv&eaccute;
     *  @version    %I%, %G%
     */
    class URIVL_EXPORT LabelRaster
    {
        public:
            /**
             *  Allocates an 8-bit label raster image at the specified dimensions.
             *
             *  Allocates an 8-bit label raster at the specified dimensions and 
             *  initializes this raster to zero values.
             *
             *  @param      nbRows     number of rows of the label raster
             *  @param      nbCols     number of columns of the label raster
             */
            LabelRaster(int nbRows, int nbCols);

            /**
             *  Allocates a label raster at the specified dimensions.
             *
             *  Initializes some instance variables (in particular the ImageRect).
             *
             *  @param      theRect   image rectangle indicating the desired dimensions
             */
            LabelRaster(const ImageRect* theRect);


            /**	Copy Constructor.
             *
             *  @param     label    the LabelRaster object to copy
             */
            LabelRaster(const LabelRaster* label);


            /**
             *  Class destructor.
             *
             *  Standard destructor (nothing fancy).
             *
             */
            ~LabelRaster(void);

            /** Copies this label raster into the one received as parameter.
             *
             *  The destination label raster is resized if needed
             *
             *  @param  label   the destination LabelRaster object
             */
            void copyInto(LabelRaster* label) const;
            
            
            /**
             *  Returns the 8-bit base label raster as a read-only 1D array.
             *
             *  @return     1D base raster for this image
             */
            const char* getLabel(void) const;

            /**
             *  Returns the 8-bit base label raster as a read-write 1D array.
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            char* getLabel(ReadWriteStatus rw) const;

            /**
             *  Returns the 8-bit base label raster as a read-only 2D array.
             *
             *  @return     2D base raster for this label raster
             */
            const char* const* getLabel2D(void) const;

            /**
             *  Returns the 8-bit base label raster as a read-write 2D array.
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this label raster
             */
            char** getLabel2D(ReadWriteStatus rw) const;

            /**
             *  Returns the label raster's read-only shifted 2D array.
             *
             *  @return     shifted 2D raster for this label raster
             */
            const char* const* getShiftedLabel2D(void) const;

            /**
             *  Returns the label raster's read-write shifted 2D array.
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D raster for this label raster
             */
            char** getShiftedLabel2D(ReadWriteStatus rw) const;

            /**
             *  Indicates whether this raster's bounding rectangle and that of the label
             *  raster received as parameter overlap.
             *  @param   img    the label raster to test agains this image
             *  @return  true if the two rectangles overlap, false otherwise
             */
            bool overlaps(const LabelRaster* img) const;
            
            /**
             *  Indicates whether this raster's bounding rectangle and that of the image
             *  received as parameter coincide.
             *  @param   img    the image to test agains this image
             *  @return  true if the two rectangles coincide, false otherwise
             */
            bool coincidesWith(const LabelRaster* img) const;
            
            /**
             *  Indicates whether this image's bounding rectangle contains that of the
             *  image received as parameter.
             *  @param   img    the image to test agains this image
             *  @return  true if theRect is contained within this rectangle, false otherwise
             */
            bool contains(const LabelRaster* img) const;
            
            /**
             *  Indicates whether this image's bounding rectangle is contained by that 
             *  of the image received as parameter.
             *  @param   img    the image to test agains this image
             *  @return  true if this image contains img, false otherwise
             */
            bool isContainedBy(const LabelRaster* img) const;

            /**
             *  Returns the number of rows for the image.
             *
             *  @return     number of rows for the image
             */
            int getHeight(void) const;

            /**
             *  Returns the number of columns for the image.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of columns for the image
             */
            int getWidth(void) const;
            
            /**
             *  Returns the label raster's bounding rectangle.
             *
             *  Returns a pointer to the label raster's bounding rectangle.
             *  
             *  Reminder: the ImageRect class uses the pixel-as-rectangle convention.
             *
             *  @return     pointer to the label raster's bouding rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getValidRect
             */
            const ImageRect* getBoundRect(void) const;

            /**
             *  Returns the label raster's valid data rectangle.
             *
             *  Returns a pointer to the label raster's valid data rectangle.
             *  
             *  Reminder: the ImageRect class uses the pixel-as-rectangle convention.
             *
             *  @return     pointer to the label raster's valid data rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getBoundRect
             *  @see    getShiftedLabel2D
             */
            const ImageRect* getValidRect(void) const;

            /**
             *  Sets a new origin for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a LabelRaster could be defined as a subimage of another
             *  image.  This operation does not result in a reallocation of the image.
             *
             *  @param  x   X coordinate of the image's origin
             *  @param  y   Y coordinate of the image's origin
             */
            void setOrigin(int x = 0, int y = 0);

            /**
             *  Translates the raster image by the specified amount.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a LabelRaster could be defined as a subimage of another
             *  image.  This operation does not result in a reallocation of the image.
             *
             *  @param  dx   horizontal displacement of the image's origin
             *  @param  dy   vertical displacement of the image's origin
             */
            void translate(int dx = 0, int dy = 0);


            /**
             *  Sets a new bounding rectangle for the label raster.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function should be overridden by child classes that have their
             *  own raster (eg.  HSV, gray_F, etc.)
             *
             *  @param  theRect new bounding rectangle for the image
             */
            void setRect(const ImageRect* theRect);

            /**
             *  Sets a new bounding rectangle for the label raster.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the label raster.
             *
             *  @param  x       X coordinate of the image's origin
             *  @param  y       Y coordinate of the image's origin
             *  @param  nbRows  number of rows of the gray-level raster image
             *  @param  nbCols  number of columns of the gray-level raster image
             */
            void setRect(int x, int y, int theWidth, int theHeight);

            /**
             *  Sets a new valid data rectangle for the label raster.
             *
             *  A raster image or vector field occupies a rectangle within the global
             *  image space.
             *  For example, a vector field could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *  The function call will fail if the new valid data rect is not contained 
             *  by the label raster's bounding rectangle
             *
             *  @param  theRect new valid data rectangle for the label raster
             */
            void setValidRect(const ImageRect* theRect);

            /**
             *  Sets a new valid data rectangle for the label raster.
             *
             *  A raster image or vector field occupies a rectangle within the global
             *  image space.
             *  For example, a vector field could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *  The function call will fail if the new valid data rect is not contained 
             *  by the label raster's bounding rectangle
             *
             *  @param  x       X coordinate of the valid data upper left corner
             *  @param  y       Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect(int x, int y, int theWidth, int theHeight);


        protected:

            /**
             *  Allocates the label rasters.
             */
            void allocateRasters_(void);
 
 
            /**
             *  Allocates the 2D label raster.
             */
            void allocateShiftedRaster2D_(void);
 
 
            /**
             *  1D label raster.
             */
            char*	label_;

            /**
             *  2D array mapped on the 1D label raster.
             */
            char**	label2D_;

            /**
             *  2D shifted array mapped on the 1D label raster.
             */
            char** shiftedLabel2D_;

			//======================================================
			//	Don't change the order of the next two declarations
			//======================================================

            /**
             *  Bounding rectangle for this label raster.
             *
             *  This rectangle defines the dimensions of the space actually allocated
             *  to store this label raster.  The upper left corner of the rectangle
             *  is not necessarily at (0,0).
             *
             *  @see    validRect_
             *  @see    getBoundRect
             *  @see    getValidRect
             *  @see    getShiftedRaster2D
             */
            ImageRect*	boundRect_;

            /**
             *  Bounding rectangle of the valid data region for this label raster.
             *
             *  This rectangle defines the subregion of the bounding rectangle (boundRect_)
             *  that contains valid data.
             *
             *  @see    boundRect_
             *  @see    getBoundRect
             *  @see    getValidRect
             *  @see    getShiftedRaster2D
             */
            ImageRect*	validRect_;
			
			
			/**	Copy constructor.
             *  Allocates a label raster at the specified dimensions.
			 *	Not implemented.  Just added so that Xcode will not gove warnings.
             *
             *  Initializes some instance variables (in particular the ImageRect).
             *
             *  @param     label    the LabelRaster object to copy
             */
            LabelRaster(const LabelRaster& label);

			/**	Copy operator.
			 *	Not implemented.  Just added so that Xcode will not gove warnings.
             *
             *  @param     label    the LabelRaster object to copy
             */
            const LabelRaster& operator = (const LabelRaster& label);

    };
}

#endif  //  LABEL_RASTER_H
