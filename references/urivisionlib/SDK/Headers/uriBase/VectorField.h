/*  NAME:
        VectorField.h

    DESCRIPTION:
        VectorField public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VECTOR_FIELD_H
#define URIVL_VECTOR_FIELD_H

#include <cstdlib>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "DrawableObject2D.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"

namespace uriVL
{
    //---------------------------------------------------------------------
    //  Constants and macros
    //---------------------------------------------------------------------

    #define     kLengthNeedles  0.85f    //  length of vector "needles" wrt the grid


    /** VectorField class
     *
     *  A VectorField object stores a vector field defined over a 2d array.
     *  I haven't quite made up my mind on the opacity of this class.  For the
     *  moment, the getRaster method returns the data's master pointer.  In the
     *  future I may decide to clone the data and return the clone's master
     *  pointer.  In any case, this would not change the class's API.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT VectorField : public DrawableObject2D
    {
        public:

            /** Destructor
             */
            virtual ~VectorField(void);

            /** Copies this vector field into the one received as parameter.
             *  If the destination vector field does not have the same dimensions as
             *  this vector field (source), its rasters are reallocated.
             *  Only the data within the validRect_ rectangle need to be be copied.
             *  The bounding rectangle and valid data rectangle of the destination
             *  are equal to that of the source.
             *  @param  vectOut     destination vector field
             */
            void copyInto(VectorField* out) const;

            /** Copies this vector field into the one received as parameter using the
             *  specified copy rectangle.
             *  The call fails if the rectangle specified is not contained within the
             *  source.
             *  This function must be overridden (and called by the overriding function)
             *  by subclasses of VectorField.
             *  @param  vectOut     destination vector field
             *  @param  theRect     region of the vector field to copy
             */
            virtual void copyInto(VectorField* vectOut, const ImageRect* theRect) const;

            //virtual void writeInto(VectorField *out) = 0;
            //virtual void writeInto(VectorField *vectOut, ImageRect* theRect) = 0;

            /** Returns the vector field's X component's 1D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's X component's 1D base raster
             */
            virtual const void* getRasterX(void) const = 0;

            /** Returns the vector field's X component's 1D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's X component's 1D base raster
             */
            virtual void* getRasterX(ReadWriteStatus rw) = 0;

            /** Returns the vector field's Y component's 1D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's Y component's 1D base raster
             */
            virtual const void* getRasterY(void) const = 0;

            /** Returns the vector field's Y component's 1D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's Y component's 1D base raster
             */
            virtual void* getRasterY(ReadWriteStatus rw) = 0;

            /** Returns the vector field's X component's 2D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's X component's 2D base raster
             */
            virtual const void* getRasterX2D(void) const = 0;

            /** Returns the vector field's X component's 2D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's X component's 2D base raster
             */
            virtual void* getRasterX2D(ReadWriteStatus rw) = 0;

            /** Returns the vector field's Y component's 2D base raster
             *  Must be overridden by child classes.
             *  @return     the vector field's Y component's 2D base raster
             */
            virtual const void* getRasterY2D(void) const = 0;

            /** Returns the vector field's Y component's 2D base raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's Y component's 2D base raster
             */
            virtual void* getRasterY2D(ReadWriteStatus rw) = 0;

            /** Returns the vector field's X component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @return     the vector field's X component's 2D "shifted" raster
             */
            virtual const void* getShiftedRasterX2D(void) const = 0;

            /** Returns the vector field's X component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's X component's 2D "shifted" raster
             */
            virtual void* getShiftedRasterX2D(ReadWriteStatus rw) = 0;

            /** Returns the vector field's Y component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @return     the vector field's Y component's 2D "shifted" raster
             */
            virtual const void* getShiftedRasterY2D(void) const = 0;

            /** Returns the vector field's Y component's 2D "shifted" raster
             *  Must be overridden by child classes.
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     the vector field's Y component's 2D "shifted" raster
             */
            virtual void* getShiftedRasterY2D(ReadWriteStatus rw) = 0;

            /** Returns the vector field's norm as gray-level raster image.
             *
             *  <b>Important:</b> What is returned here is a a pointer to the
             *  vector field's private data. Do not delete it.  If you plan to
             *  modify this data, you are better off passing a RasterImage_gray_F
             *  to <code>getNormAsRasterImage</code> to fill in.
             *  @return     vector field's norm as a gray-level raster image
             *  @see    computeNormAndPhase
             */
            virtual const RasterImage_gray_F* getNormAsRasterImage(void) const;

            /** Returns the vector field's norm as gray-level raster image.
             *
             *  <b>Important:</b> What is returned here is a a pointer to the
             *  vector field's private data. Do not delete it.  If you plan to
             *  modify this data, you are better off passing a RasterImage_gray_F
             *  to <code>getNormAsRasterImage</code> to fill in.
             *  @return     vector field's norm as a gray-level raster image
             *  @see    computeNormAndPhase
             */
            virtual RasterImage_gray_F* getNormAsRasterImage(ReadWriteStatus rw);

            /** Returns the vector field's norm squared as gray-level raster image.
             *
             *  <b>Important:</b> What is returned here is a a pointer to the
             *  vector field's private data. Do not delete it.  If you plan to
             *  modify this data, you are better off passing a RasterImage_gray_F
             *  to <code>getNormSquaredAsRasterImage</code> to fill in.
             *  @return     vector field's norm squared as a gray-level raster image
             *  @see    computeNormSquaredAndPhase
             */
            virtual const RasterImage_gray_F* getNormSquaredAsRasterImage(void) const;

            /** Returns the vector field's norm squared as gray-level raster image.
             *
             *  <b>Important:</b> What is returned here is a a pointer to the
             *  vector field's private data. Do not delete it.  If you plan to
             *  modify this data, you are better off passing a RasterImage_gray_F
             *  to <code>getNormSquaredAsRasterImage</code> to fill in.
             *  @return     vector field's norm squared as a gray-level raster image
             *  @see    computeNormSquaredAndPhase
             */
            virtual RasterImage_gray_F* getNormSquaredAsRasterImage(ReadWriteStatus rw);

            /** Returns the vector field's phase as gray-level raster image.
             *  @return     vector field's norm as a gray-level raster image
             *
             *  If you plan to use this function in combination with <code>getNormAsRasterImage</code>
             *  (resp. <code>getNormSquaredAsRasterImage</code>) then you might get better
             *  performance by calling <code>computeNormAndPhase</code>
             *  (resp. <code>computeNormSquaredAndPhase</code>)first.  That way, if
             *  the norm (resp. norm squared) and/or phase either have not been allocated
             *  or have not been updated, they will be both computed in one pass.
             *  @see    computeNormAndPhase
             *  @see    computeNormSquaredAndPhase
             */
            virtual const RasterImage_gray_F* getPhaseAsRasterImage(void) const;

            /** Returns the vector field's phase as gray-level raster image.
             *  @return     vector field's norm as a gray-level raster image
             *
             *  If you plan to use this function in combination with <code>getNormAsRasterImage</code>
             *  (resp. <code>getNormSquaredAsRasterImage</code>) then you might get better
             *  performance by calling <code>computeNormAndPhase</code>
             *  (resp. <code>computeNormSquaredAndPhase</code>)first.  That way, if
             *  the norm (resp. norm squared) and/or phase either have not been allocated
             *  or have not been updated, they will be both computed in one pass.
             *  @see    computeNormAndPhase
             *  @see    computeNormSquaredAndPhase
             */
            virtual RasterImage_gray_F* getPhaseAsRasterImage(ReadWriteStatus rw);

            /** Returns the vector field's norm as gray-level raster image.
             *
             *  @param      normOut destination raster image for the norm of this vector field
             *  @see    getNormAndPhaseAsRasterImages
             */
            virtual void getNormAsRasterImage(RasterImage_gray* normOut);

            /** Returns the vector field's norm squared as gray-level raster image.
             *
             *  @param      norm2Out    destination raster image for the square of the norm of
             *                      this vector field
             *  @see    getNormSquaredAndPhaseAsRasterImages
             */
            virtual void getNormSquaredAsRasterImage(RasterImage_gray* norm2Out);

            /** Returns the vector field's phase as gray-level raster image.
             *
             *  If you plan to use this function in combination with <code>getNormAsRasterImage</code>
             *  (resp. <code>getNormSquaredAsRasterImage</code>) then you might get better
             *  performance by calling <code>computeNormAndPhase</code>
             *  (resp. <code>computeNormSquaredAndPhase</code>)first.  That way, if
             *  the norm (resp. norm squared) and/or phase either have not been allocated
             *  or have not been updated, they will be both computed in one pass.
             *  @param      phaseOut    destination raster image for the phase of this vector field
             */
            virtual void getPhaseAsRasterImage(RasterImage_gray* phaseOut);

            /** Returns the vector field's norm and phase as gray-level raster images.
             *  @param      normOut destination raster image for the norm of this vector field
             *  @param      phaseOut    destination raster image for the phase of this vector field
             */
            virtual void getNormAndPhaseAsRasterImages(RasterImage_gray* normOut,
                                                       RasterImage_gray* phaseOut);

            /** Returns the vector field's squared norm and phase as gray-level raster images.
             *  @param      norm2Out    destination raster image for the squared norm of this vector field
             *  @param      phaseOut    destination raster image for the phase of this vector field
             */
            virtual void getNormSquaredAndPhaseAsRasterImages(RasterImage_gray* norm2Out,
                                                              RasterImage_gray* phaseOut);

            /** Computes vector field's norm as a gray-level raster image
             *  and stores it as an instance variable
             */
            void computeNorm(void) const;

            /** Returns the vector field's squared norm as a gray-level raster image
             *  and stores it as an instance variable
             */
            void computeNormSquared(void) const;

            /** Computes vector field's phase as gray-level raster image
             *  and stores it as an instance variable
             */
            void computePhase(void) const;

            /** Computes vector field's norm and phase as gray-level raster images and
             *  stores them as instance variables
             */
            void computeNormAndPhase(void) const;

            /** Computes vector field's norm squared and phase as gray-level raster images and
             *  stores them as instance variables
             */
            void computeNormSquaredAndPhase(void);

            /** Sets the resolution of the vector field's rendering (distance between
             *  needles in pixel)
             */
            void setVectResolution(int nPts);

            /** true if the vector field has a float raster
             *  @return     true if the vector field has a float raster
             */
            bool hasFloatRaster(void) const;

            /** Forces an update of the raster (for rendering)
             *
             *  Should be called by users of the class who modify directly the raster
             *  (by accessing it through calls to getRaster or getShiftedRaster functions)
             */
            void forceRasterUpdate(void);

            /** Sets this vector field's base raster as the reference
             */
            void setRasterAsReference(void);

            /** Sets this vector field's norm and phase as the reference (fails
             *  either one was NULL)
             */
            void setNormAndPhaseAsReference(void);

            /** Sets this vector field's norm squared and phase as the reference (fails
             *  either one was NULL)
             */
            void setNormSquaredAndPhaseAsReference(void);


            /** Indicates whether or not the reference raster for this vector field has been modified.
             *
             *  When the reference raster is modified, other rasters stored by the vector field object
             *  my have to be modified.
             *
             *  @return     true if the reference raster has changed.
             *
             *  @see    referenceHasChanged_
             */
            bool referenceHasChanged(void) const;

			/**	To be called by a child class that has modified the raster or one
			 *	of the secondary rasters, or updated a secondary raster
			 */
			void setReferenceHasChanged(void) const;
			


		protected:
		
			/**	Radius of the circular dot used to render a vertex
			 */
			static float NEEDLE_DOT_RADIUS;

            /**
             *  Initializes a vector field at the specified dimensions and values.
             *
             *  @param      nbRows     number of rows of the vector field
             *  @param      nbCols     number of columns of vector field
			 *	@param		hasFloatRast	true if this object has a float raster
			 *	@param		drawableInfoType	by default we assume that we have a dense vector field
             */
            VectorField(int theNbRows, int theNbCols, bool hasFloatRast,
						DrawableInformationType drawableInfoType = kDense);

            /**
             *  Initializes a vector field at the specified dimensions.
             *
             *  @param      theRect     rectangle at which to allocate the vector field
			 *	@param		drawableInfoType	by default we assume that we have a dense vector field
             */
            VectorField(const ImageRect* theRect, bool hasFloatRast,
						DrawableInformationType drawableInfoType = kDense);

            /**
             *  Initializes a vector field at the specified dimensions.
             *
             *  @param      theRect     rectangle at which to allocate the vector field
			 *	@param		hasFloatRast	true if this object has a float raster
			 *	@param		drawableInfoType	by default we assume that we have a dense vector field
             */
            VectorField(const ImageRect& theRect, bool hasFloatRast,
						DrawableInformationType drawableInfoType = kDense);

            /**
             *  Copy Constructor.
             *
             *  Initializes a vector field at the dimensions and values of the
             *  VectorField received as parameter.
             *
             *  @param      theField   pointer to the VectorField object to copy
			 *	@param		hasFloatRast	true if this object has a float raster
			 *	@param		drawableInfoType	by default we assume that we have a dense vector field
             */
            VectorField(const VectorField* theField, bool hasFloatRast,
						DrawableInformationType drawableInfoType = kDense);

            /**
             *  Copy Constructor. Disabled
             *
             *  @param      theField   reference to the VectorField object to copy
             */
            VectorField(const VectorField& theField);

		
			/**	To be called by a child class that has modified the raster or one
			 *	of the secondary rasters, or updated a secondary raster
			 *	@param	hasChanged	new value to assign to referenceHasChanged_
			 */
			void setReferenceHasChanged_(bool hasChanged) const;
			

        private:

            /** resolution of vector field's rendering
             */
            mutable int   nVectRes_;

            /** Scale factor for vectors' needles
             */
            mutable float   scaleVect_;

            /** Stores the maximum value of the norm over the entire valid portion of the
             *   vector field
             */
            mutable float  normMax_;

            /** True if the reference raster has been modified since the last
             *  rendering
             */
            mutable bool    referenceHasChanged_;

            /** True if the vector field has been modified since the last time the
             *  norm was calculated.
             */
            mutable bool    normNeedsUpdate_;

            /** True if the vector field has been modified since the last time the
             *  norm squared was calculated.
             */
            mutable bool    normSquaredNeedsUpdate_;

            /** True if the vector field has been modified since the last time the
             *  phase was calculated.
             */
            mutable bool    phaseNeedsUpdate_;

            /** True if the VectorField is of a "float" subclass
             */
            bool    hasFloatRaster_;

            /** True if the VectorField's base raster is the reference
             */
            bool    baseRasterIsReference_;

            /** True if the VectorField's norm and phase rasters are the reference
             */
            bool    normAndPhaseIsReference_;

            /** True if the VectorField's norm squared and phase rasters are the reference
             */
            bool    normSquaredAndPhaseIsReference_;


            /** Stores a local copy of the vector field's norm.  This object is
             *  allocated and initialized by a call to computeNormAsRasterImage
             *  @see computeNormAsRasterImage
             */
            mutable RasterImage_gray_F* norm_;

            /** Stores a local copy of the vector field's norm squared.  This object is
             *  allocated and initialized by a call to computeNormSquaredAsRasterImage
             *  @see computeNormSquaredAsRasterImage
             */
            mutable RasterImage_gray_F* normSquared_;

            /** Stores a local copy of the vector field's phase.  This object is
             *  allocated and initialized by a call to computePhaseAsRasterImage
             *  @see computePhaseAsRasterImage
             */
            mutable RasterImage_gray_F* phase_;


            /**
             *  Copy operator. Disabled
             *
             *  @param      theField   reference to the VectorField object to copy
             */
            const VectorField& operator = (const VectorField& theField);


            /** Computes vector field's norm as a gray-level raster image
             *  @param	norm	pointer to the vector field's private norm variable
             */
            virtual void computeNorm_(RasterImage_gray_F* norm) const = 0;

            /** Returns the vector field's squared norm as a gray-level raster image
             *  and stores it as an instance variable
             *  @param	normSq	pointer to the vector field's private norm squared variable
             */
            virtual void computeNormSquared_(RasterImage_gray_F* normSq) const = 0;

            /** Computes vector field's phase as gray-level raster image
             *  and stores it as an instance variable
             *  @param	phase	pointer to the vector field's private phase variable
             */
            virtual void computePhase_(RasterImage_gray_F* phase) const = 0;

            /** Computes vector field's norm and phase as gray-level raster images and
             *  stores them as instance variables
             *  @param	norm	pointer to the vector field's private norm variable
             *  @param	phase	pointer to the vector field's private phase variable
             */
            virtual void computeNormAndPhase_(RasterImage_gray_F* norm, RasterImage_gray_F* phase) const = 0;

            /** Computes vector field's norm squared and phase as gray-level raster images and
             *  stores them as instance variables
             *  @param	normSq	pointer to the vector field's private norm squared variable
             *  @param	phase	pointer to the vector field's private phase variable
             */
            virtual void computeNormSquaredAndPhase_(RasterImage_gray_F* normSq, RasterImage_gray_F* phase) const = 0;


            /**
             *  Sets a new bounding rectangle for the vector field.
             *
             *  A vector field occupies a rectangle within the global image space.
             *  For example, a vector field could be defined as a subpart of another.
             *  This operation may require a reallocation of the vector field.
             *
             *  If the norm, norm squared, or phase have been allocated then their valid
             *  data rect must be modified too.
             *
             *  @param  x       X coordinate of the vector field's origin
             *  @param  y       Y coordinate of the vector field's origin
             *  @param  theWidth    desired width of the vector field
             *  @param  theHeight   desired height of the vector field
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Sets a new valid data rectangle for the vector field.
             *
             *  If the norm, norm squared, or phase have been allocated then their valid
             *  data rect must be modified too.
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            virtual void setValidRect_(int x, int y, int theWidth, int theHeight);


            /** Render the vector field as a subsampled array of arrows at the depth
             *  and location specified.
             *  @param  scaledLeft  x coordinate of the vector field's upper-left corner when rendered scaled
			 *	@param	scaledTop	y coordinate of the vector field's upper-left corner when rendered scaled
			 *	@param	scaledRight x coordinate of the vector field's lower-right corner when rendered scaled
			 *	@param	scaledBottom y coordinate of the vector field's lower-right corner when rendered scaled
             *  @param  theZ    Depth at which this vector field is to be rendered.
             *  @param  shifted     true if the vector field should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the vector field's upper-left corner in the rendering
             */
            virtual void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
								 float theZ, bool shifted, const ImagePoint* origin) const;


            /** Render the vector field as a regularly sampled array of needles.
			 *	@param nVectRes		resolution of vector field's rendering
			 *	@param scaleVect	scale factor for vectors' needles
             */
            virtual void renderNeedles_(int nVectRes, float scaleVect) const = 0;

            /** Computes the shifting and scaling factors to properly render the 
             *  vectors as needles the next time render_ is called.
			 *	@param nVectRes		resolution of vector field's rendering
			 *	@param scaleVect	scale factor for vectors' needles (gets updated)
			 *	@param	normMax		max value of the vector field's norm (gets updated)
             */
            virtual void prepareVectFieldForRendering_(int nVectRes, float& scaleVect,
														float& normMax) const = 0;

            /**
             *  Allocates the 2D base rasters.
             *  @param  nbRows		number of rows of the field
             *  @param  nbCols		number of columns of the field
             */
            virtual void allocateRaster2D_(int nbRows, int nbCols) = 0;


            /**
             *  Deletes the 0-based 2D base rasters.
             *  This virtual function must be implemented by child classes.
             */
            virtual void deleteRaster2D_(void) = 0;


            /**
             *  Allocates the 2D shifted rasters.
             *  @param  jLow       X coordinate of the image's origin
             *  @param  iLow       Y coordinate of the image's origin
             *  @param  nbRows		number of rows of the field
             *  @param  nbCols		number of columns of the field
             */
            virtual void allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow) = 0;


            /**
             *  Translates the 2D  shifted base rasters by the specified amount.
             *  This virtual function must be implemented by child classes.
             *  @param  rowShift    displacement of the row index
             *  @param  colShift    displacement of the column index
             */
            virtual void translateShiftedRaster2D_(int rowShoft, int colShift) = 0;

            /**
             *  Deletes the 2D shifted base raster.
             */
            virtual void deleteShiftedRaster2D_(void) = 0;


            /** Calculates the object's bounding rect
			 *	@param	rect	the bounding box to update
             */
            void computeBoundingRect_(ImageRect& rect) const;


            /** Calculates the object's valid box
			 *	@param	rect	the bounding box to update
             */
            void computeValidRect_(ImageRect& rect) const;


            /** Computes the contour's centroid.
			 *	@param	cent	the centroid point to update
             */
			void computeCentroid_(ImagePoint_F& cent) const;

            /** Computes the object's center of mass by calculating the  
             *  average point
			 *	@param	cent	the center of mass point to update
             */
            void computeCenterOfMass_(ImagePoint_F& cent) const;


//        friend class VectorField_I;
//        friend class VectorField_F;

    };

}   //  end uriVL

#endif  //  VECTOR_FIELD_H
