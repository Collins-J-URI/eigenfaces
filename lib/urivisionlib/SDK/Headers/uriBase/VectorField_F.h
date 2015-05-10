/*  NAME:
        vectorField_F.h

    DESCRIPTION:
        vectorField_F public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VECTOR_FIELD_F_H
#define URIVL_VECTOR_FIELD_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "VectorField.h"

namespace uriVL
{
    /** VectorField_F class
     *
     *  A VectorField_F is a "float" vector field defined over a 2d array.
     *  I haven't quite made up my mind on the opacity of this class.  For the
     *  moment, the getRaster method returns the data's master pointer.  In the
     *  future I may decide to clone the data and return the clone's master
     *  pointer.  In any case, this would not change the class's API.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT VectorField_F : public VectorField
    {
        public:

            /**
             *  Initializes a float vector field at the specified dimensions.
             *
             *  @param      nbRows     number of rows of the vector field
             *  @param      nbCols     number of columns of vector field
             */
            VectorField_F(int theNbRows, int theNbCols);

            /**
             *  Initializes a double vector field at the specified dimensions.
             *
             *  @param      theRect     rectangle at which to allocate the vector field
             */
            VectorField_F(const ImageRect* theRect);

            /**
             *  Cloner Constructor.
             *
             *  Initializes a float vector field at the dimensions and values of the
             *  VectorField received as parameter.
             *
             *  @param      theField   pointer to the VectorField object to clone
             */
            VectorField_F(VectorField* theField);

            /**
             *  Initializes a float vector field at the specified dimensions and values.
             *
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
             *  @param      theRastX   X component for the VectorField
             *  @param      theRastY   Y component for the VectorField
             */
            VectorField_F(int theNbRows, int theNbCols, const float* theRastX, 
                          const float* theRastY);

            /**
             *  Destructor.
             */
            ~VectorField_F(void);


            /** Copies this vector field into the one received as parameter.
             *  If the destination vector field does not have the same dimensions as
             *  this vector field (source), its rasters are reallocated.
             *  Only the data within the validRect_ rectangle need to be be copied.
             *  The bouding rectangle and valid data rectangle of the destination
             *  are equal to that of the source.
             *  @param  vectOut     destination vector field
             */
            void copyInto(VectorField* out) const;

            /** Copies this vector field into the one received as parameter using the
             *  specified copy rectangle.
             *  The call fails if the rectangle specified is not contained within the source.
             *  @param  vectOut     destination vector field
             *  @param  theRect     region of the vector field to copy
             */
            void copyInto(VectorField* out, const ImageRect* theRect) const;


            /** Returns the vector field's X component's 1D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's X component's 1D base raster
             */
            const void* getRasterX(void) const;

            /** Returns the vector field's X component's 1D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's X component's 1D base raster
             */
            void* getRasterX(ReadWriteStatus rw);

            /** Returns the vector field's Y component's 1D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's Y component's 1D base raster
             */
            const void* getRasterY(void) const;

            /** Returns the vector field's Y component's 1D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's Y component's 1D base raster
             */
            void* getRasterY(ReadWriteStatus rw);

            /** Returns the vector field's X component's 2D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's X component's 2D base raster
             */
            const void* getRasterX2D(void) const;

            /** Returns the vector field's X component's 2D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's X component's 2D base raster
             */
            void* getRasterX2D(ReadWriteStatus rw);

            /** Returns the vector field's Y component's 2D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's Y component's 2D base raster
             */
            const void* getRasterY2D(void) const;

            /** Returns the vector field's Y component's 2D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's Y component's 2D base raster
             */
            void* getRasterY2D(ReadWriteStatus rw);

            /** Returns the vector field's X component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @return     the vector field's X component's 2D "shifted" raster
             */
            const void* getShiftedRasterX2D(void) const;

            /** Returns the vector field's X component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's X component's 2D "shifted" raster
             */
            void* getShiftedRasterX2D(ReadWriteStatus rw);

            /** Returns the vector field's Y component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @return     the vector field's Y component's 2D "shifted" raster
             */
            const void* getShiftedRasterY2D(void) const;

            /** Returns the vector field's Y component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's Y component's 2D "shifted" raster
             */
            void* getShiftedRasterY2D(ReadWriteStatus rw);



            /** Calculates the min and max values of the vector field along its X and Y
             *  directions
             *  @param  theMinX pointer to min value along X direction
             *  @param  theMaxX pointer to max value along X direction
             *  @param  theMinY pointer to min value along Y direction
             *  @param  theMaxY pointer to max value along Y direction
             */
            void getMinMax(float* theMinX, float* theMaxX,
                           float* theMinY, float* theMaxY) const;

            /** Calculates the min and max values of the vector field along its X direction.
             *  @param  theMinX pointer to min value along X direction
             *  @param  theMaxX pointer to max value along X direction
             */
            void getMinMaxX(float* theMinX, float* theMaxX) const;

            /** Calculates the min and max values of the vector field along its Y direction.
             *  @param  theMinY pointer to min value along Y direction
             *  @param  theMaxY pointer to max value along Y direction
             */
            void getMinMaxY(float* theMinY, float* theMaxY) const;

            /** Applies a Transformation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            void applyTransformation(const Transformation2D* theTransf);

            /**	Rotates the object by the angle specified (in radians)
             *	If rotationCenter is null, then the centroid of the object is used
             *	as center of rotation
             *	@param	angle	the angle of the rotation (in radians)
             *	@param	rotCenter	the center of the rotation
             */
            void rotate(float angle, const ImagePoint* rotCenter = NULL);



        private:

            /** stores the x component of the vector field
             */
            float*	vx_;

            /** stores the y component of the vector field
             */
            float*	vy_;

            /** 2D array mapped over vx_
             */
            float**	vx2D_;

            /** 2D array mapped over vy_
             */
            float**	vy2D_;

            /** "shifted" 2D array mapped over vx_.  Uses the true values of the indices
             *  as given by the bounding and valid data rectangles
             */
            float**	shiftedVx2D_;

            /** "shifted" 2D array mapped over vyu_.  Uses the true values of the indices
             *  as given by the bounding and valid data rectangles
             */
            float**	shiftedVy2D_;

            /**
             *  Copy Constructor. Disabled
             *
             *  @param      theField   reference to the VectorField object to copy
             */
            VectorField_F(const VectorField_F& theField);

            /**
             *  Copy operator. Disabled
             *
             *  @param      theField   reference to the VectorField object to copy
             */
            const VectorField_F& operator = (const VectorField_F& theField);

            /** Computes vector field's norm as a gray-level raster image
             *  @param	norm	pointer to the vector field's private norm variable
             */
            void computeNorm_(RasterImage_gray_F* norm) const;

            /** Returns the vector field's squared norm as a gray-level raster image
             *  and stores it as an instance variable
             *  @param	normSq	pointer to the vector field's private norm squared variable
             */
            void computeNormSquared_(RasterImage_gray_F* normSq) const;

            /** Computes vector field's phase as gray-level raster image
             *  and stores it as an instance variable
             *  @param	phase	pointer to the vector field's private phase variable
             */
            void computePhase_(RasterImage_gray_F* phase) const;

            /** Computes vector field's norm and phase as gray-level raster images and
             *  stores them as instance variables
             *  @param	norm	pointer to the vector field's private norm variable
             *  @param	phase	pointer to the vector field's private phase variable
             */
            void computeNormAndPhase_(RasterImage_gray_F* norm, RasterImage_gray_F* phase) const;

            /** Computes vector field's norm squared and phase as gray-level raster images and
             *  stores them as instance variables
             *  @param	normSq	pointer to the vector field's private norm squared variable
             *  @param	phase	pointer to the vector field's private phase variable
             */
            void computeNormSquaredAndPhase_(RasterImage_gray_F* normSq, RasterImage_gray_F* phase) const;

            /** Render the vector field as a regularly sampled array of needles.
			 *	@param nVectRes		resolution of vector field's rendering
			 *	@param scaleVect	scale factor for vectors' needles
             */
            void renderNeedles_(int nVectRes, float scaleVect) const;

            /** Computes the shifting and scaling factors to properly render the 
             *  vectors as needles the next time render_ is called.
			 *	@param nVectRes		resolution of vector field's rendering
			 *	@param scaleVect	scale factor for vectors' needles (gets updated)
			 *	@param	normMax		max value of the vector field's norm (gets updated)
             */
            void prepareVectFieldForRendering_(int nVectRes, float& scaleVect,
												  float& normMax) const;

            /**
             *  Allocates the 2D base rasters.
             *  @param  nbRows		number of rows of the field
             *  @param  nbCols		number of columns of the field
             */
            void allocateRaster2D_(int nbRows, int nbCols);


            /**
             *  Deletes the 0-based 2D base rasters.
             */
            void deleteRaster2D_(void);


            /**
             *  Allocates the 2D shifted rasters.
             *  @param  jLow       X coordinate of the image's origin
             *  @param  iLow       Y coordinate of the image's origin
             *  @param  nbRows		number of rows of the field
             *  @param  nbCols		number of columns of the field
             */
            void allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow);


            /**
             *  Translates the 2D  shifted base rasters by the specified amount.
             *  @param  rowShift    displacement of the row index
             *  @param  colShift    displacement of the column index
             */
            void translateShiftedRaster2D_(int rowShoft, int colShift);

            /**
             *  Deletes the 2D shifted base raster.
             */
            void deleteShiftedRaster2D_(void);


           /** Initializes this VectorField's rasters with the arrays received as 
             *  parameters
             *  @param  rasterX     values for the vector field's x components
             *  @param  rasterY     values for the vector field's y components
             */
            void initializeRasters_(const float* rasterX, const float* rasterY);


		//	The parent class can call private functions of this class
		friend class VectorField;
		
    };

}   //  end uriVL

#endif  //  VECTOR_FIELD_F_H
