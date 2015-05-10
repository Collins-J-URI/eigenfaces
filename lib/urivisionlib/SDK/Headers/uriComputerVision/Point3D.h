/*  NAME:
        Point3D.h
 
    DESCRIPTION:
        Point3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_POINT_3D_H
#define URIVL_POINT_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"

namespace uriVL
{
	/**	Stores the normal vector at a point
	 */
	typedef struct NormalVector {
						double nx;
						double ny;
						double nz;
	} NormalVector;

	/** Implements the Point3D class.
	 * 
	 *  A Point3D object stores the 3D coordinates of a point relative to a given
	 *  reference frame.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Point3D : public Entity3D
    {
        public:

        	/**
        	 *	Constructor.
        	 *	Creates a 3D point in the reference frame received as parameter
        	 *	@param	X		X coordinate of the point
        	 *	@param	Y		Y coordinate of the point
        	 *	@param	Z		Z coordinate of the point
        	 *  @param  theRef  the reference frame relative to which this point is defined
        	 */
            Point3D(double X, double Y, double Z, const ReferenceFrame3D *theRef);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D point in the reference frame received as parameter
        	 *	@param	theXYZ		coordinate array of the point
        	 *  @param  theRef      the reference frame relative to which this point is defined
        	 */
            Point3D(const double theXYZ[], const ReferenceFrame3D *theRef);

        	/**
        	 *	Clone constructor.
        	 *	Creates a clone of the Point3D object passed as parameter (including that
        	 *  point's reference frame).
        	 *	@param	thePt		pointer to the Point3D object to clone
        	 */
            Point3D(const Point3D *thePt);
 
        	/**
        	 *	Constructor.
        	 *	Creates a 3D point in the default reference frame.  This constructor should
        	 *  only be used by an object that stores a set of points when all these
        	 *  points are defiend relative to the object's internal reference frame.
        	 *  It would be dangerous to send such an unreferenced point out "in the wild."
        	 *	@param	theX		X coordinate of the point
        	 *	@param	theY		Y coordinate of the point
        	 *	@param	theZ		Z coordinate of the point
        	 */
            Point3D(double X, double Y, double Z);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D point in the default reference frame.  This constructor should
        	 *  only be used by an object that stores a set of points when all these
        	 *  points are defiend relative to the object's internal reference frame.
        	 *  It would be dangerous to send such an unreferenced point out "in the wild."
        	 *	@param	theXYZ		coordinate array of the point
        	 */
            Point3D(const double theXYZ[]);

            /**
             *	Destructor
             */          
            virtual ~Point3D(void);


			/**
			 *	Returns the point's X coordinate
			 *	@return		X coordinate of the point
			 */
            const double getX(void) const;

			/**
			 *	Returns the point's Y coordinate
			 *	@return		Y coordinate of the point
			 */
            const double getY(void) const ;

			/**
			 *	Returns the point's Z coordinate
			 *	@return		Z coordinate of the point
			 */
            const double getZ(void) const ;
            
            /**
             *  Returns the point's coordinates vector.
             *  @return     a pointer to the object's private coordinate vector.
             */
            const double* getXYZ(void) const;

			/**
			 *	Assigns new x coordinate to the point
			 *	@param	XYZ 	new coordinate array for the 3D point
			 */
            void setPosition(const double XYZ[]);

			/**
			 *	Assigns new x coordinate to the point
        	 *	@param	X		X coordinate of the point
        	 *	@param	Y		Y coordinate of the point
        	 *	@param	Z		Z coordinate of the point
			 */
            void setPosition(double X, double Y, double Z);

            /**
             *  Returns the distance between this point and the one received as parameter.
             *  @param  p   the point to which the distance must be computed
             *  @return the distnace between this point and p
             */
            double distanceTo(const Point3D *p) const;


        private:

        	/**
        	 *	The point's X, Y, and Z coordinates
        	 */
            double   XYZ_[3];
    };
    
     

}   

#endif  //  POINT_3D_H
