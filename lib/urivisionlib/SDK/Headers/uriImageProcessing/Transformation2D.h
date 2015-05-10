/*  NAME:
        Transformation2D.h
 
    DESCRIPTION:
        Transformation2D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRANSFORMATION2D_H
#define URIVL_TRANSFORMATION2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

#include "ImagePoint_F.h"

namespace uriVL
{

    typedef enum TransformationQuality {
                                    kUnknown = -1,
                                    kDefinition = 0,
                                    kReliable,
                                    kUnreliable
    } TransformationQuality;
                                  
    
    /** Virtual base class for all 2D transformation that can be represented by
     *  a 3 by 3 homogenous matrix.
     * 
     *  @author     jean-yves herv&eaccute;
     *  @version    %I%, %G%
     */
    class URIVL_EXPORT Transformation2D
    {
        public:

			/**
             *  Constructor.
             *  Creates an identity transformation 3D.
			 *	@param	isRigid		indicates whether this is a rigid transformation
             */
            Transformation2D(bool isRigid=false);


            /**
             *  Constructor.
             *  Creates a 3D transformation based on the transformation matrix received
             *  as parameter
             *  @param  theMat  3 by 3 transformation matrix
             */
            Transformation2D(const float theMat[][3]);

            /**
             *  Constructor.
             *  Creates a 3D transformation based on the transformation matrix received
             *  as parameter
             *  @param  theMat  3 by 3 transformation matrix
             */
            Transformation2D(const float*const* theMat);

            /**
             *  Copy constructor.
             *  Creates a 3D transformation identical to the one received as parameter
             *  @param  theTransf   transformation to copy
             */
            Transformation2D(const Transformation2D* theTransf);

            /**
             *  Copy constructor.
             *  Creates a 3D transformation identical to the one received as parameter.
			 *
			 *	Not implemented yet while we try to settle issue regarding constness and
			 *	mutability of mat_ and related flags
             *  @param  theTransf   transformation to copy
             */
            Transformation2D(const Transformation2D& theTransf);

            /**
             *  Destructor
             */          
            virtual ~Transformation2D(void);
			
            /**
             *  Equal operator.
             *  Created so that gcc will stop complaining.
			 *
			 *	Not implemented yet while we try to settle issue regarding constness and
			 *	mutability of mat_ and related flags
             *  @param  theTransf       reference to the Transformation2D object to copy
             */
            const Transformation2D& operator = (const Transformation2D& theTransf);
			

            /** Assign a new matrix to this transformation.  The new matrix is now the reference
             *  for this transformation.
             *  @param  theMat  3x3 transformation matrix
             */
            virtual void setMatrix(const float theMat[][3]);


            /** Assign a new matrix to this transformation.  The new matrix is now the reference
             *  for this transformation.
             *  @param  theMat  3x3 transformation matrix
             */
            virtual void setMatrix(const float*const* theMat);
            
            
            /** Returns a read-only pointer to the transformation's matrix data.  
             *  If you want to have full read-write
             *  access to the data, call instead getMatrixRW.
             *  @return the matrice's private read-only 2D array of data
             */
            const float* const*const getMatrix(void) const;

            /** Returns a pointer to the transformation's's matrix data that allows to set new 
             *  values to the matrix's 
             *  elements.
             *  As a side effect, this function sets the matrix to become the reference for this 
             *  transformation.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return the matrix's private read-write 2D array of data
             */
            float* const*const getMatrix(ReadWriteStatus rwa);

            
            /** Applies this transformation to the image point received as parameter
             *  @param  pt  the image point to apply this transformation to
             *  @return a new ImagePoint_F object
             */
            ImagePoint_F *applyTo(const ImagePoint* pt);
            
            /** Applies this transformation to the image point received as parameter
             *  @param  pt  the image point to apply this transformation to
             *  @param  transfPt   the image point resulting from applying this transformation to pt
             */
            ImagePoint_F *applyInto(const ImagePoint* pt, const ImagePoint* transfPt);
            

            /** Indicates whether this transftormation is rigid (rotation + translation)
             */
            bool isRigidTransformation(void) const;

	
		protected:
		
			/**	Check whether the matrix is orthogonal
			 *	@return true if the matrix is orthogonal
			 */
			bool checkOrthogonality_(void) const;
			

        private:

            /** the 3x3 homogeneous matrix that stores the transformation.
             *  This matrix is the reference by default (unless explicitely changed by
             *  the user)
             */
            mutable float** mat_;
            
            /** Indicates whether this transftormation is rigid (rotation + translation)
             */
            bool isRigidTransformation_;
			
            /** Store an indicator of the transformation's quality. Where does it come from?
             *  Is it the result of a composition of good quality transformations, of
             *  registration calculations?
             */
            TransformationQuality quality_;
            
            
    };
}

#endif  //  TRANSFORMATION2D_H
