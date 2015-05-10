/*  NAME:
        Contour.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Contour class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Contour.h"

using namespace std;
using namespace uriVL;


#if 0
//----------------------------------------
#pragma mark Initialization of constants
//----------------------------------------
#endif
float Contour::VERTEX_DOT_RADIUS = 4.f;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

Contour::Contour(const Contour& ct)
		try	:   ImageFeature(ct),
				//
				ptDistance_(ct.ptDistance_),
				isFloatingPointContour_(ct.isFloatingPointContour_),
				interPointDistancesAreUpToDate_(ct.interPointDistancesAreUpToDate_),
				edgeVectorsAreUpToDate_(ct.edgeVectorsAreUpToDate_),
				drawPoints_(ct.drawPoints_)
{
    vertexColor_[0] = vertexColor_[1] = vertexColor_[2] = 0.f;
    vertexColor_[3] = 1.0f;
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by Contour constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid Contour reference parameter passed to Contour constructor");
}



Contour::Contour(bool isFloatingPoint)
        :   ImageFeature(),
			//
			ptDistance_(list<float>(0)),
			isFloatingPointContour_(isFloatingPoint),
			interPointDistancesAreUpToDate_(false),
			edgeVectorsAreUpToDate_(false),
			drawPoints_(false)
{
    vertexColor_[0] = vertexColor_[1] = vertexColor_[2] = 0.f;
    vertexColor_[3] = 1.0f;
}

Contour::~Contour(void)
{
	ptDistance_.clear();
}

const Contour& Contour::operator = (const Contour& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Contour copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Initialize contour 
//------------------------------------------------------
#endif

void Contour::initialize(int nbPts, ContourInitializer contourType,
				const ImageRect* rect, int padding)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");
}


void Contour::initialize(int nbPts, ContourInitializer contourType, const ImagePoint& center, 
						 float width, float height, float angle)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Set color 
//------------------------------------------------------
#endif


void Contour::setVertexColor(float red, float green, float blue)
{
    vertexColor_[0] = red;
    vertexColor_[1] = green;
    vertexColor_[2] = blue;
    vertexColor_[3] = 1.f;
}

void Contour::setVertexColor(const float theColor[])
{
    vertexColor_[0] = theColor[0];
    vertexColor_[1] = theColor[1];
    vertexColor_[2] = theColor[2];
    vertexColor_[3] = 1.f;
}

//float Contour::getContourDisplacement(void){
//	return contourDisplacement_;
//}

void Contour::setVertexColor(unsigned char red, unsigned char green, unsigned char blue)
{
    vertexColor_[0] = red/255.f;
    vertexColor_[1] = green/255.f;
    vertexColor_[2] = blue/255.f;
    vertexColor_[3] = 1.f;
}

void Contour::setVertexColor(const unsigned char theColor[])
{
    vertexColor_[0] = theColor[0]/255.f;
    vertexColor_[1] = theColor[1]/255.f;
    vertexColor_[2] = theColor[2]/255.f;
    vertexColor_[3] = 1.f;
}

void Contour::setDrawPoints(bool drawPointCircle)
{
	drawPoints_ = drawPointCircle;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to geometric info
//------------------------------------------------------
#endif


const list<float>* Contour::getInterPointDistances(void) const
{
    if (!interPointDistancesAreUpToDate_)
        computeInterPointDistances_();
    return &ptDistance_;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Update private change indicators
//------------------------------------------------------
#endif

void Contour::pointsHaveMoved(void)
{
    interPointDistancesAreUpToDate_ = false;
    edgeVectorsAreUpToDate_ = false;
	setCentroidIsUpToDate_(false);
	setCenterOfMassIsUpToDate_(false);
	setBoundingRectIsUpToDate_(false);
	setValidRectIsUpToDate_(false);
}

void Contour::setInterPointDistancesAreUpToDate_(void) const
{
    interPointDistancesAreUpToDate_ = true;
}

void Contour::setEdgeVectorsAreUpToDate_(void) const
{
    edgeVectorsAreUpToDate_ = true;
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Bounding and Valid rects functions
//------------------------------------------------------
#endif



void Contour::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");

    //	this function should clip the contour to the new dimensions of the bounding box.
    //	See Folley van-Damm for algorithm
}

void Contour::setValidRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}



