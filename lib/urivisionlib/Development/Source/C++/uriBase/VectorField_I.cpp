/*  NAME:
        VectorField.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Vector_I class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstring>
//
#include "VectorField_I.h"
#include "VectorField_F.h"
#include "RasterImage_gray_F.h"

using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

//	This version of the constructor creates an empty image of the desired type
//	and at the specified dimensions
VectorField_I::VectorField_I(int theNbRows, int theNbCols)
		try	:	VectorField(theNbRows, theNbCols, false),
				//
				vx_(NULL),
				vy_(NULL),
				vx2D_(NULL),
				vy2D_(NULL),
				shiftedVx2D_(NULL),
				shiftedVy2D_(NULL)				
{
    allocateRaster2D_(theNbRows, theNbCols);
    allocateShiftedRaster2D_(theNbRows, theNbCols, 0, 0);
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField_I constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField_I constructor");
}


//	This version of the constructor creates an empty vector field
//  at the specified dimensions
VectorField_I::VectorField_I(const ImageRect* theRect)
		try	:	VectorField(theRect, false),
				//
				vx_(NULL),
				vy_(NULL),
				vx2D_(NULL),
				vy2D_(NULL),
				shiftedVx2D_(NULL),
				shiftedVy2D_(NULL)				
{
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField_I constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField_I constructor");
}


//	This version of the constructor creates a vector raster, from two separate arrays,
//	vx_ and vy_.
//
VectorField_I::VectorField_I(int theNbRows, int theNbCols, const int* theRastX, 
							 const int* theRastY)
		try	:	VectorField(theNbRows, theNbCols, false),
				//
				vx_(NULL),
				vy_(NULL),
				vx2D_(NULL),
				vy2D_(NULL),
				shiftedVx2D_(NULL),
				shiftedVy2D_(NULL)				
{
    FAIL_CONDITION(	(theRastX == NULL) || (theRastY==NULL),
                    kNullRasterError,
                    "NULL array passed as argument to VectorField_I constructor");

    allocateRaster2D_(theNbRows, theNbCols);
    allocateShiftedRaster2D_(theNbRows, theNbCols, 0, 0);

    try
	{
		initializeRasters_(theRastX, theRastY);
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Memory copy error in VectorField_I constructor");
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField_I constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField_I constructor");
}


//	This version of the constructor produces a clone of the VectorField_I object
//	received as parameter.
VectorField_I::VectorField_I(VectorField* theField)
		try	:	VectorField(theField, false),
				//
				vx_(NULL),
				vy_(NULL),
				vx2D_(NULL),
				vy2D_(NULL),
				shiftedVx2D_(NULL),
				shiftedVy2D_(NULL)				
{
    FAIL_CONDITION(	theField == NULL,
                    kNullVectorFieldError,
                    "NULL VectorField_I passed as argument to VectorField_I clone constructor");

    allocateRaster2D_(theField->getHeight(), theField->getWidth());
    allocateShiftedRaster2D_(theField->getHeight(), theField->getWidth(), theField->getTop(), theField->getLeft());

    try
	{
		theField->copyInto(this);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by VectorField_I constructor");
		throw e;
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField_I constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField_I constructor");
}



VectorField_I::VectorField_I(const VectorField_I& theField)
		try	:	VectorField(theField),
				//
				vx_(NULL),
				vy_(NULL),
				vx2D_(NULL),
				vy2D_(NULL),
				shiftedVx2D_(NULL),
				shiftedVy2D_(NULL)				
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"VectorField copy constructor not implemented");
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField_I constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField_I constructor");
}



VectorField_I::~VectorField_I(void)
{
    deleteShiftedRaster2D_();
    deleteRaster2D_( );
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif

const VectorField_I& VectorField_I::operator = (const VectorField_I& theField)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"VectorField_I copy operator not implemented");
	return *this;
}


void VectorField_I::copyInto(VectorField* vectOut, const ImageRect* theRect) const
{
    //	All the rectangle testing and resizing is done in the parent class' function
    VectorField::copyInto(vectOut, theRect);

	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();

    if (vectOut->hasFloatRaster())
    {
        float   **xOut = (float** ) (((VectorField_I *) vectOut)->getRasterX2D());
        float   **yOut = (float** ) (((VectorField_I *) vectOut)->getRasterY2D());

        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                xOut[i][j] = shiftedVx2D_[i][j];
                yOut[i][j] = shiftedVx2D_[i][j];
            }
        }
    }
    else
    {
        int   **xOut = (int** ) (((VectorField_I *) vectOut)->getRasterX2D());
        int   **yOut = (int** ) (((VectorField_I *) vectOut)->getRasterY2D());

        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                xOut[i][j] = shiftedVx2D_[i][j];
                yOut[i][j] = shiftedVx2D_[i][j];
            }
        }
    }

	vectOut->setReferenceHasChanged();
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the rasters
//------------------------------------------------------
#endif

const void *VectorField_I::getRasterX(void) const
{
    return (void *) vx_;
}

void *VectorField_I::getRasterX(ReadWriteStatus rw)
{
	setRasterAsReference();
	setReferenceHasChanged();
    return (void *) vx_;
}

const void *VectorField_I::getRasterY(void) const
{
    return (void *) vy_;
}

void *VectorField_I::getRasterY(ReadWriteStatus rw)
{
	setRasterAsReference();
	setReferenceHasChanged();
    return (void *) vy_;
}

const void *VectorField_I::getRasterX2D(void) const
{
    return (void *) vx2D_;
}

void *VectorField_I::getRasterX2D(ReadWriteStatus rw)
{
	setRasterAsReference();
	setReferenceHasChanged();
    return (void *) vx2D_;
}

const void *VectorField_I::getRasterY2D(void) const
{
    return (void *) vy2D_;
}

void *VectorField_I::getRasterY2D(ReadWriteStatus rw)
{
	setRasterAsReference();
	setReferenceHasChanged();
    return (void *) vy2D_;
}

const void *VectorField_I::getShiftedRasterX2D(void) const
{
    //  if the vector field has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
        return (void *) shiftedVx2D_;
    else
        return (void *) vx2D_;

}

void *VectorField_I::getShiftedRasterX2D(ReadWriteStatus rw)
{
	setRasterAsReference();
	setReferenceHasChanged();

    //  if the vector field has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
        return (void *) shiftedVx2D_;
    else
        return (void *) vx2D_;

}

const void *VectorField_I::getShiftedRasterY2D(void) const
{
    //  if the vector field has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
        return (void *) shiftedVy2D_;
    else
        return (void *) vy2D_;

}

void *VectorField_I::getShiftedRasterY2D(ReadWriteStatus rw)
{
	setRasterAsReference();
	setReferenceHasChanged();

    //  if the vector field has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
        return (void *) shiftedVy2D_;
    else
        return (void *) vy2D_;

}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Norm and phase
//------------------------------------------------------
#endif

//  This private method is called after all the data validity testing has been
//  done.  I know that the rasters I need exist and have the proper dimensions
void VectorField_I::computeNorm_(RasterImage_gray_F* norm) const
{
	const ImageRect* vRect = getValidRect();
    const int	iLow  = vRect->getTop(),
        		iHigh = vRect->getBottom(),
        		jLow = vRect->getLeft(),
        		jHigh = vRect->getRight();
    float*const* flNorm = norm->getShiftedGrayF2D(R_W_ACCESS);
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            flNorm[i][j] = sqrtf(vx[i][j]*vx[i][j] + vy[i][j]*vy[i][j]);

}

//  This private method is called after all the data validity testing has been
//  done.  I know that the rasters I need exist and have the proper dimensions
void VectorField_I::computeNormSquared_(RasterImage_gray_F* normSq) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    float*const* flNorm2 = normSq->getShiftedGrayF2D(R_W_ACCESS);
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            flNorm2[i][j] = vx[i][j]*vx[i][j] + vy[i][j]*vy[i][j];

}


//  This private method is called after all the data validity testing has been
//  done.  I know that the rasters I need exist and have the proper dimensions
void VectorField_I::computePhase_(RasterImage_gray_F* phase) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    float*const* flPhase = phase->getShiftedGrayF2D(R_W_ACCESS);
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            flPhase[i][j] = atan2f(vy[i][j], vx[i][j]);

}

//  This private method is called after all the data validity testing has been
//  done.  I know that the rasters I need exist and have the proper dimensions
void VectorField_I::computeNormAndPhase_(RasterImage_gray_F* norm, RasterImage_gray_F* phase) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    float*const* flNorm = norm->getShiftedGrayF2D(R_W_ACCESS);
    float*const* flPhase = phase->getShiftedGrayF2D(R_W_ACCESS);
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());

    for (int i=iLow; i<=iHigh; i++)
    {
	    for (int j=jLow; j<=jHigh; j++)
        {
            flNorm[i][j] = sqrtf(vx[i][j]*vx[i][j] + vy[i][j]*vy[i][j]);

            flPhase[i][j] = atan2f(vy[i][j], vx[i][j]);
        }
	}
}

//  This private method is called after all the data validity testing has been
//  done.  I know that the rasters I need exist and have the proper dimensions
void VectorField_I::computeNormSquaredAndPhase_(RasterImage_gray_F* normSq, RasterImage_gray_F* phase) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    float*const* flPhase = phase->getShiftedGrayF2D(R_W_ACCESS);
    float*const* flNorm2 = normSq->getShiftedGrayF2D(R_W_ACCESS);
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());

    for (int i=iLow; i<=iHigh; i++)
    {
	    for (int j=jLow; j<=jHigh; j++)
        {
            flNorm2[i][j] = vx[i][j]*vx[i][j] + vy[i][j]*vy[i][j];

            flPhase[i][j] = atan2f(vy[i][j], vx[i][j]);
        }
	}
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Other access fonctions
//------------------------------------------------------
#endif

void VectorField_I::getMinMax(int* theMinX, int* theMaxX, int* theMinY,
                              int* theMaxY) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());
    int	myMinX = shiftedVx2D_[iLow][jLow], myMaxX = myMinX;
    int	myMinY = shiftedVy2D_[iLow][jLow], myMaxY = myMinY;

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if (vx[i][j] < myMinX)
                myMinX = vx[i][j];
            if (vx[i][j] > myMaxX)
                myMaxX = vx[i][j];
            //
            if (vy[i][j] < myMinY)
                myMinY = vy[i][j];
            if (vy[i][j] > myMaxY)
                myMaxY = vy[i][j];
        }

    *theMinX = myMinX;
    *theMaxX = myMaxX;
    *theMinY = myMinY;
    *theMaxY = myMaxY;
}

void VectorField_I::getMinMaxX(int* theMinX, int* theMaxX) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    const int* const* vx = static_cast<const int* const*>(getShiftedRasterX2D());
    int	myMinX = shiftedVx2D_[iLow][jLow], myMaxX = myMinX;

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if (vx[i][j] < myMinX)
                myMinX = vx[i][j];
            if (vx[i][j] > myMaxX)
                myMaxX = vx[i][j];
        }

    *theMinX = myMinX;
    *theMaxX = myMaxX;
}

void VectorField_I::getMinMaxY(int* theMinY, int* theMaxY) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    const int* const* vy = static_cast<const int* const*>(getShiftedRasterY2D());
    int	myMinY = shiftedVy2D_[iLow][jLow], myMaxY = myMinY;

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if (vy[i][j] < myMinY)
                myMinY = vy[i][j];
            if (vy[i][j] > myMaxY)
                myMaxY = vy[i][j];
        }

    *theMinY = myMinY;
    *theMaxY = myMaxY;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Transformations
//------------------------------------------------------
#endif

void VectorField_I::rotate(float angle, const ImagePoint* rotCenter)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void VectorField_I::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rendering functions
//------------------------------------------------------
#endif


void VectorField_I::renderNeedles_(int nVectRes, float scaleVect) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    float	vx, vy;
    const int* const* vx2D = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy2D = static_cast<const int* const*>(getShiftedRasterY2D());

    const int   iRes = (iHigh-iLow+1) - (nVectRes)*((iHigh-iLow+1)/nVectRes - 1),
                jRes = (jHigh-jLow+1) - (nVectRes)*((jHigh-jLow+1)/nVectRes - 1);
    const int   iFirst = iLow + iRes/2,
                jFirst = jLow + jRes/2;

    glTranslatef(jFirst, iFirst, 0.f);
    for (int i = iFirst; i <= iHigh; i += nVectRes)
    {
        //	save the row's leftmost point
        glPushMatrix();
        for (int j = jFirst; j <= jHigh; j += nVectRes)
        {
            //	draw the vector proper as a line segment
            vx = vx2D[i][j] * scaleVect;
            vy = vy2D[i][j] * scaleVect;
            //
            glBegin(GL_LINES);
            glVertex3f(0.f, 0.f, 0);
            glVertex3f(vx, vy, 0);
            glEnd();

            //	draw a small disk
			glPushMatrix();
				glScalef(NEEDLE_DOT_RADIUS, NEEDLE_DOT_RADIUS, 1.f);
				uriVL::fillCircularDot();
			glPopMatrix();
			
            //	Move to the next rendering spot
            glTranslatef(nVectRes, 0.f, 0.f);
        }
        //	return to the row's leftmost point
        glPopMatrix();
        //	and go down to the next row
        glTranslatef(0.f, nVectRes, 0.f);
    }
}


void VectorField_I::prepareVectFieldForRendering_(int nVectRes, float& scaleVect,
												  float& normMax) const
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
    float vx, vy, vVal;
    const int* const* vx2D = static_cast<const int* const*>(getShiftedRasterX2D());
    const int* const* vy2D = static_cast<const int* const*>(getShiftedRasterY2D());

    //	Determine the maximum Euclidean norm of the vector field over a
    //	uniformly-spaced grid of image points
    normMax = 0.f;
    const int   iRes = (iHigh-iLow+1) - (nVectRes)*((iHigh-iLow+1)/nVectRes - 1),
                jRes = (jHigh-jLow+1) - (nVectRes)*((jHigh-jLow+1)/nVectRes - 1);
    const int   iFirst = iLow + iRes/2,
                jFirst = jLow + jRes/2;

    for (int i = iFirst; i <= iHigh; i += nVectRes)
    {
        for (int j = jFirst; j <= jHigh; j += nVectRes)
        {
            vx = vx2D[i][j];
            vy = vy2D[i][j];

            vVal = vx * vx + vy * vy;

            if (vVal > normMax)
                normMax = vVal;

        }
    }

    //	Knowing the maximum length of the vector over the grid, we can compute a scale
    //	factor for proper rendering.
    if (normMax > 0)
    {
        normMax = sqrtf(normMax);
        scaleVect = kLengthNeedles * nVectRes / normMax;
    }
    else
        scaleVect = 0.f;

}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif

void VectorField_I::initializeRasters_(const int* theRastX, const int* theRastY)
{
    const int   nbRows = getHeight(),
                nbCols = getWidth();
    unsigned long    nbBytes = nbRows * nbCols * sizeof(int);

    memcpy(vx_, theRastX, nbBytes);
    memcpy(vy_, theRastY, nbBytes);
}

void VectorField_I::allocateRaster2D_(int nbRows, int nbCols)
{
    long    nbPixels = nbRows*nbCols;

    vx_ = vy_ = NULL;
    vx2D_ = vy2D_ = NULL;

    vx_ = new int[nbPixels];
    FAIL_CONDITION(	vx_ == NULL,
                    kRasterAllocationFailure,
                    "float vector raster allocation failed");
    vy_ = new int[nbPixels];
    FAIL_CONDITION(	vy_ == NULL,
                    kRasterAllocationFailure,
                    "float vector raster allocation failed");

    vx2D_ = new int*[nbRows];
    FAIL_CONDITION(	vx2D_ == NULL,
                    kRasterAllocationFailure,
                    "2D float vector raster allocation failed");

    vy2D_ = new int*[nbRows];
    FAIL_CONDITION(	vx2D_ == NULL,
                    kRasterAllocationFailure,
                    "2D float vector raster allocation failed");

    vx2D_[0] = vx_;
    vy2D_[0] = vy_;
    for (int i = 0; i < nbRows-1; i++)
    {
        vx2D_[i+1] = vx2D_[i] + nbCols;
        vy2D_[i+1] = vy2D_[i] + nbCols;
    }
}


void VectorField_I::deleteRaster2D_(void)
{
    if (vx2D_ != NULL)
        delete []vx2D_;

    if (vy2D_ != NULL)
        delete []vy2D_;

    if (vx_ != NULL)
        delete []vx_;
    if (vy_ != NULL)
        delete []vy_;

    vx_ = vy_ = NULL;
    vx2D_ = vy2D_ = NULL;
}


void VectorField_I::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the vector field has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1;

        shiftedVx2D_ = shiftedVy2D_ = NULL;

        shiftedVx2D_ = new int*[nbRows];
        FAIL_CONDITION(	shiftedVx2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D integer vector raster allocation failed");

        shiftedVy2D_ = new int*[nbRows];
        FAIL_CONDITION(	shiftedVy2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D integer vector raster allocation failed");

        shiftedVx2D_ -= iLow;
        shiftedVy2D_ -= iLow;
        //
        shiftedVx2D_[iLow] = vx_ - jLow;
        shiftedVy2D_[iLow] = vy_ - jLow;
        for (int i = iLow; i < iHigh; i++)
        {
            shiftedVx2D_[i+1] = shiftedVx2D_[i] + nbCols;
            shiftedVy2D_[i+1] = shiftedVy2D_[i] + nbCols;
        }
    }
    else
    {
        //	I do this to unearth parts of the library where I access the rasters
        //	directly rather than through the access functions.
        //    	shiftedVx2D_ = vx2D_;
        //    	shiftedVy2D_ = vy2D_;
        shiftedVx2D_ = NULL;
        shiftedVy2D_ = NULL;
    }
}


void VectorField_I::translateShiftedRaster2D_(int rowShift, int colShift)
{
    int   iLow  = getTop(),
          iHigh = getBottom();

    for (int i = iLow; i < iHigh; i++)
    {
        shiftedVx2D_[i+1] -= colShift;
        shiftedVy2D_[i+1] -= colShift;
    }
    shiftedVx2D_ -= rowShift;
    shiftedVy2D_ -= rowShift;

    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "Function temporarily out of  service");

}

void VectorField_I::deleteShiftedRaster2D_(void)
{
    if ((getTop() != 0) || (getLeft() != 0))
    {
        int   iLow  = getTop();

        if (shiftedVx2D_ != NULL)
        {
            shiftedVx2D_ += iLow;
            delete []shiftedVx2D_;
        }
        if (shiftedVy2D_ != NULL)
        {
            shiftedVy2D_ += iLow;
            delete []shiftedVy2D_;
        }
    }
    shiftedVx2D_ = shiftedVy2D_ = NULL;
}

