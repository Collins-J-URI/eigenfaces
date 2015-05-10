/*  NAME:
        RigidTransformation3D.h
 
    DESCRIPTION:
        RigidTransformation3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_RIGID_TRASFORMATION_3D_H
#define URIVL_RIGID_TRASFORMATION_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Transformation3D.h"
#include "Translation3D.h"
#include "Rotation3D.h"

namespace uriVL
{
    class ReferenceFrame3D;
    
	/** A RigidTransformation is a transformation between reference frames that preserves
	 *  angles and lengths.
	 *
	 *  A RigidTransformation is composed of a rotation and a translation and can be represented
	 *  either by an object of each type or by a 4x4 homogeneous transformation matrix.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT RigidTransformation3D: public Transformation3D
    {
        public:

        	/**
        	 *	Constructor.
        	 *	Creates an identity transformation.
        	 */
            RigidTransformation3D(void);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	glMat	a flat, GL-style transformation matrix
        	 */
            RigidTransformation3D(const double* glMat);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	theMat	4 by 4 transformation matrix
        	 */
            RigidTransformation3D(const double theMat[][4]);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	theMat	homogeneous transformation matrix
        	 */
            RigidTransformation3D(const HomogeneousMatrix *theMat);

        	/**
        	 *	Clone constructor.
        	 *	Creates a 3D transformation identical to the one received as parameter
        	 *	@param	theTransf	transformation to clone
        	 */
            RigidTransformation3D(const RigidTransformation3D *theTransf);

        	/**
        	 *	Constructor.
        	 *	Creates the 3D rigid transformation betwen the two reference frames received as
        	 *  parameters.  Throws an exception if such a transformation cannot be defined.
        	 *  reference frame.
        	 *	@param	startFrame	reference frame relative to which the transformation is defined
        	 *	@param	endFrame	reference frame after the transformation
        	 */
            RigidTransformation3D(const ReferenceFrame3D *startFrame, const ReferenceFrame3D *endFrame);

            /** Creates a new RigidTransformation3D object specified by a GL-style matrix, a quaternion
             *  and a translation vector, a triplet of R-P-Y angles and a translation vector, or a triplet 
             *  of Euler angles and a translation vector.
             *  @param  rotDataType    indicates what kind of data is stored in the first double array
             *  @param  rotData     1D array of double storing a representation for the rotation
             *  @param  transData   1D array of double storing a representation for the translation.
             */
            RigidTransformation3D(RotationRef rotDataType, const double* rotData, const double* transData );
            

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the rotation and translation received as parameters
        	 *	@param	theRot	the rotation transformation
        	 *	@param	theTrans	the translation transformation
        	 */
            RigidTransformation3D(const Rotation3D *theRot, const Translation3D *theTrans);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the rotation and translation received as parameters
        	 *	@param	theRot	the rotation transformation
        	 */
            RigidTransformation3D(const Rotation3D *theRot);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the rotation and translation received as parameters
        	 *	@param	theTrans	the translation transformation
        	 */
            RigidTransformation3D(const Translation3D *theTrans);

            /**
             *	Destructor
             */          
            virtual ~RigidTransformation3D(void);

            /**  Returns a read-only version of the rotation component of the rigid transformation
             *  @return     the rotation component of the rigid transformation
             */
            const Rotation3D *getRotationComponent(void);
            
            /**  Returns a writable version of the rotation component of the rigid transformation
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return     the rotation component of the rigid transformation
             */
            Rotation3D *getRotationComponent(ReadWriteStatus rwa);
            
            /**  Returns a read-only version of the translation component of the rigid transformation
             *  @return     translation rotation component of the rigid transformation
             */
            const Translation3D *getTranslationComponent(void);

            /**  Returns a writable version of the translation component of the rigid transformation
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return     translation rotation component of the rigid transformation
             */
            Translation3D *getTranslationComponent(ReadWriteStatus rwa);

            /**
             *  composes a new rigid transformation with this rigid transformation.
             *  The transformation
             *  to apply must be defined relative to the current/moving frame.
             *  @param  theTransf   the transformation to compose with this transformation
             */
            void postComposeTransformation(const RigidTransformation3D* theTransf);
            
            /**
             *  composes a new rigid transformation with this rigid transformation.
             *  The transformation
             *  to apply must be defined relative to the current/moving frame.
             *  @param  theRot   the rotation to compose with this transformation
             */
            void postComposeTransformation(const Rotation3D* theRot);

			
            /**
             *  composes a new rigid transformation with this rigid transformation.
             *  The transformation
             *  to apply must be defined relative to the current/moving frame.
             *  @param  theTrans   the translation to compose with this transformation
             */
            void postComposeTransformation(const Translation3D* theTrans);


        private:


            /** Rotation component of the 3D transformation
             */
            Rotation3D      *rot_;

            /** Translation component of the 3D transformation
             */
            Translation3D   *trans_;

           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theField   reference to the object to copy
             */
            RigidTransformation3D(const RigidTransformation3D& theField);

            /**
             *  Copy operator. Disabled
             *
             *  @param      theField   reference to the object to copy
             */
            const RigidTransformation3D& operator = (const RigidTransformation3D& theField);


            /** Updates the transformation matrix if it was not the reference.
             *
             */ 
            void updateMatrix_(void) const;

            /** Ensures that the transformation matrix is still that of a rotation
             *
             */ 
            void orthogonalizeMatrix_(void);

        
    };
}   

#endif  //  RIGID_TRASFORMATION_3D_H
