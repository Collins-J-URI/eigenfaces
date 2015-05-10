/*  NAME:
        Contour_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Contour_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>

#include "Contour_F.h"
#include "Contour_I.h"

using namespace std;
using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

Contour_F::Contour_F(const Contour_F & ct)
	try	:   Contour(ct),
			//
			contourList_(list <ImagePoint_F*>()),
			edgeList_(list <ImagePoint_F *>())
{
    list<ImagePoint_F *>::const_iterator iter0, iter1;
    list<float>::const_iterator iter2;
    if (!(ct.contourList_.empty()))
    {
	    iter0 = ct.contourList_.begin();
	    for( ; iter0!=ct.contourList_.end(); iter0++)
	        contourList_.push_back(new ImagePoint_F(*iter0));
	}
    if (!(ct.edgeList_.empty()))
    {
	    iter1 = ct.edgeList_.begin();
	    for( ; iter1!=ct.edgeList_.end(); iter1++)
	        edgeList_.push_back(new ImagePoint_F(*iter1));
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by Contour_F constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to Contour_F constructor");
}

Contour_F::Contour_F(void)
		:   Contour(),
			//
			contourList_(list <ImagePoint_F*>()),
			edgeList_(list <ImagePoint_F*>())
{
}

Contour_F::Contour_F(const ImagePoint_FList* ptsList)
		:   Contour(true),
			//
			contourList_(list <ImagePoint_F*>()),
			edgeList_(list <ImagePoint_F *>())
{
	try {
		for(ImagePoint_FList::const_iterator iter = ptsList->begin() ;iter!=ptsList->end(); iter++)
		{
			contourList_.push_back(new ImagePoint_F(*iter));
		}
	}
	catch (...)
	{
		FAIL_CONDITION( ptsList == NULL,
						kNullParameterError,
						"Null pointer to point list passed to Contour_F constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid parameter passed to Contour_F constructor");
	}

}


Contour_F::Contour_F(int theNumPts, const ImagePoint_F *const* theContourPts)
		:   Contour(true),
			//
			contourList_(list <ImagePoint_F*>()),
			edgeList_(list <ImagePoint_F *>())
{
	try {
		for(int i = 0;i < theNumPts; i++)
			contourList_.push_back(new ImagePoint_F(theContourPts[i]));
	}
	catch (...)
	{
		FAIL_CONDITION( theContourPts == NULL,
						kNullParameterError,
						"Null array passed to Contour_F constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid parameter passed to Contour_F constructor");
	}
}

Contour_F::Contour_F(int theNumPts)
		:   Contour(false),
			contourList_(list <ImagePoint_F*>()),
			edgeList_(list <ImagePoint_F *>())
{
    for(int i = 0;i < theNumPts; i++)
    {
        contourList_.push_back(new ImagePoint_F(0.f, 0.f));
    }
}


Contour_F::~Contour_F(void)
{	
	//	delete the point list
	list<ImagePoint_F*>::iterator	endPtList = contourList_.end();
	for(list<ImagePoint_F*>::iterator iter = contourList_.begin(); iter!=endPtList; iter++)
		delete (*iter);
	contourList_.clear();

	//	delete the edge list
	list<ImagePoint_F*>::iterator	endEdgeList = edgeList_.end();
	for(list<ImagePoint_F*>::iterator iter = edgeList_.begin(); iter!=endEdgeList; iter++)
		delete (*iter);
	edgeList_.clear();
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copying
//------------------------------------------------------
#endif


const Contour_F& Contour_F::operator = (const Contour_F& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Contour_F copy operator not implemented");
	return *this;
}



void Contour_F::copyInto(Contour* contourOut) const
{
	//  so far I only accept another Contour_F as a destination
	FAIL_CONDITION( typeid(*contourOut) != typeid(uriVL::Contour_F),
					kInvalidRasterType,
					"Contour_F::copyInto only accepts Contour_F contours as destination");

	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"not implemented yet");
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to geometric info
//------------------------------------------------------
#endif


const list<ImagePoint_F*> *Contour_F::getEdgeList(void)
{
	if (!edgeVectorsAreUpToDate_)
		computeEdgeVectorList_();
	
	return &edgeList_;
}




// This basically changes the value of a specific point
void Contour_F::setPoint(int pointIndex, float x, float y)
{
	if (static_cast<unsigned int>(pointIndex) <= contourList_.size())
	{
		list<ImagePoint_F*>::iterator iter = contourList_.begin();

		for(int i = 0; i <= pointIndex; i++)
		{
			iter++;
		}
		(*iter)->setCoordinates(x, y);
	}
}

//This just adds a point in any postion
void Contour_F::insertPoint(int pointIndex, float xVal, float yVal){

	ImagePoint_F  *newPoint = new ImagePoint_F(xVal, yVal);
	list<ImagePoint_F*>::iterator iter = contourList_.begin();
	for(int i = 0; i < pointIndex; i++){
		iter++;
	}
	contourList_.insert(iter, newPoint);
}

int Contour_F::getNbPoints(void) const {
	return static_cast<int>(contourList_.size());
}

const list <ImagePoint_F*> *Contour_F::getPoints(void) const 
{
	return &contourList_;
}

list <ImagePoint_F*> *Contour_F::getPoints(ReadWriteStatus rw){
	return &contourList_;
}




#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Geometric operations
//------------------------------------------------------
#endif

//CLIFF YOU PLAN TO USE THIS FOR THE CRUDE PRE-ALIGNMENT
void Contour_F::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( theTransf == NULL,
                    kNullParameterError,
                    "null Transformation2D passed to Contour_F::applyTransformation");
    if (theTransf->isRigidTransformation())
    {
        RigidTransformation2D   *rigidTransf = (RigidTransformation2D*) theTransf;
        
        rotate(rigidTransf->getAngle());
        translate(rigidTransf->getTransX(), rigidTransf->getTransY());
	}
    else
        FAIL_CONDITION( true,
                        kFunctionNotImplemented,
                        "Function not implemented yet");
    
    pointsHaveMoved();
}

void Contour_F::rotate(float angle, const ImagePoint* rotCenter){

	float	cosA = cosf(angle),
            sinA = sinf(angle);
	list<ImagePoint_F*>::iterator 	iter;

	for(iter = contourList_.begin(); iter != contourList_.end(); iter++){
		float 	copyOfCntourPtsX = (*iter)->getXF(),
				copyOfCntourPtsY = (*iter)->getY();
		(*iter)->setCoordinates(copyOfCntourPtsX * cosA - copyOfCntourPtsY * sinA,
							copyOfCntourPtsX * sinA + copyOfCntourPtsY * cosA);
	}
	
	pointsHaveMoved();
}


void Contour_F::translate(float tx, float ty ){
	list<ImagePoint_F*>::iterator	iter, 
							endList = contourList_.end();
	
	for(iter = contourList_.begin(); iter != endList; iter++)
		(*iter)->translate(tx, ty);
		
//TODO: should not have to recompute center of mass and rect:  only translate them as well
	pointsHaveMoved();
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Geometric tests
//------------------------------------------------------
#endif

bool Contour_F::contains(int x, int y) const
{
	return contains((float) x, (float) y);
}

bool Contour_F::contains(float x, float y) const
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"not implemented yet");
	return false;
}

bool Contour_F::contains(const ImagePoint* pt) const
{
    FAIL_CONDITION( pt == NULL,
                    kNullImagePointError,
                    "null ImagePoint in Contour_F::contains");
    return contains((float) (pt->getX()), (float) (pt->getY()));
}

bool Contour_F::contains(const ImagePoint_F* pt) const
{
    FAIL_CONDITION( pt == NULL,
                    kNullImagePointError,
                    "null ImagePoint in Contour_F::contains");
    return contains(pt->getXF(), pt->getYF());
}

bool Contour_F::contains(const Contour_I* contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_F::contains(const Contour_F* contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_F::isInside(const Contour_I* contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_F::isInside(const Contour_F* contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Computation of geometric information
//------------------------------------------------------
#endif



void Contour_F::computeCentroid_(ImagePoint_F& cent) const {
	list<ImagePoint_F*>::const_iterator iter = contourList_.begin();
	float 	maxX = -HUGE_VALF,
			minX = HUGE_VALF,
			maxY = -HUGE_VALF,
			minY = HUGE_VALF;

	for(iter = contourList_.begin(); iter != contourList_.end(); iter++){
		float	xi  = (*iter)->getXF(),
				yi = (*iter)->getYF();

		if (xi > maxX)
			maxX = xi;
		else if(xi < minX)
			minX = xi;

		if(yi > maxY)
			maxY = yi;
		else if(yi < minY)
			minY = yi;
	}

	cent.setCoordinates((maxX-minX)/2, (maxY-minY)/2);
}

void Contour_F::computeCenterOfMass_(ImagePoint_F& cent) const {

	int		theNumPts = 0;
	float 	averageX = 0.f, 
			averageY = 0.f;

	//iterate through the list and add the coordinate values.
	list<ImagePoint_F*>::const_iterator iter, endList = contourList_.end();
	for(iter = contourList_.begin(); iter != endList; iter++){
		theNumPts++;
		averageX += (*iter)->getXF();
		averageY += (*iter)->getYF();
	}
	averageX /= theNumPts;
	averageY /= theNumPts;

	cent.setCoordinates(averageX, averageY);
}

void Contour_F::computeInterPointDistances_(void) const{
	if (!interPointDistancesAreUpToDate_){
		if (!edgeVectorsAreUpToDate_)
			computeEdgeVectorList_();

		ptDistance_.clear();
		
		list<ImagePoint_F*>::const_iterator 	iter, itp1,
												firstList = contourList_.begin(),
												endList = contourList_.end();
		for(iter = firstList, itp1 = firstList; iter != endList; iter++){
			itp1++;

			if(itp1 == contourList_.end())
				itp1 = contourList_.begin();

			// calculate the distance from the current point to
			// the next
			float   dx = (*itp1)->getXF() - (*iter)->getXF(),
					dy = (*itp1)->getYF() - (*iter)->getYF();
					
			ptDistance_.push_back( sqrtf(dx*dx + dy*dy));
		}
		interPointDistancesAreUpToDate_ = true;
	}
}

void Contour_F::computeEdgeVectorList_(void) const{
	if (!edgeVectorsAreUpToDate_){
		ImagePoint_F *myPoint;

		edgeList_.clear();
		list<ImagePoint_F*>::const_iterator	iter, itp1,
								firstList = contourList_.begin(),
								endList = contourList_.end();
		for(iter = firstList, itp1 = firstList; iter != endList; iter++){            
			itp1++;

			if(itp1 == contourList_.end() )
			itp1 = contourList_.begin();

			myPoint = new ImagePoint_F((*itp1)->getXF() - (*iter)->getXF(), 
									(*itp1)->getYF() - (*iter)->getYF());
			edgeList_.push_back(myPoint);
		}
		edgeVectorsAreUpToDate_ = true;
	}
}


// This is just a quick and dirty min max alg to get the dimensions
// of a contour
void Contour_F::computeBoundingRect_(ImageRect& rect) const
{
	list<ImagePoint_F*>::const_iterator firstIter = contourList_.begin();
	float 	maxX = (*firstIter)->getXF(),
			minX = maxX,
			maxY = (*firstIter)->getYF(),
			minY = maxY;

	list<ImagePoint_F*>::const_iterator endIter = contourList_.end();
    for(list<ImagePoint_F*>::const_iterator iter = firstIter; iter != endIter; iter++)
	{
		float	xi  = (*iter)->getXF(),
				yi = (*iter)->getYF();

		if (xi > maxX)
			maxX = xi;
		else if(xi < minX)
			minX = xi;

		if(yi > maxY)
			maxY = yi;
		else if(yi < minY)
			minY = yi;
	}

	rect.setRect(static_cast<int>(minX), static_cast<int>(minY),
				static_cast<int>(maxX - minX + 1.5f), static_cast<int>(maxY - minY + 1.5f));
}

void Contour_F::computeValidRect_(ImageRect& rect) const
{	
	//	the valid rect is the same as the bounding rect.  If the bounding rect
	//	is up to date, use its dimensions, else recompute it first
	rect.setRect(getBoundRect());
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rendering
//------------------------------------------------------
#endif

void Contour_F::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						  float theZ, bool shifted, const ImagePoint* origin) const
{
	//--------------------------------------------------------------
	//	Render the contour
	//--------------------------------------------------------------
	//
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(scaledLeft, scaledTop, theZ);
	glScalef(getRenderingScaleX(), -getRenderingScaleY(), 1.f);

	glColor4fv(getColor());
	list<ImagePoint_F*>::const_iterator endList = contourList_.end();

	glBegin(GL_LINE_LOOP);
		for(list<ImagePoint_F*>::const_iterator iter = contourList_.begin(); iter != endList; iter++)
		{
			glVertex2f( (*iter)->getXF(), (*iter)->getYF());
		}
	glEnd();

	if(drawPoints_){

		glColor4fv(vertexColor_);
		
        for(list<ImagePoint_F*>::const_iterator iter = contourList_.begin(); iter != endList; iter++)
		{
            glPushMatrix();
            glTranslatef((*iter)->getXF(), (*iter)->getYF(), 0);
			glScalef(VERTEX_DOT_RADIUS, VERTEX_DOT_RADIUS, 1.f);
			uriVL::fillCircularDot();
            glPopMatrix();
        }
	}
	
	glPopMatrix();
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark contour initialization functions
//------------------------------------------------------
#endif



void Contour_F::initializeBox_(const ImageRect& rect, int padding)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");
					
}


void Contour_F::initializeBox_(int nbPts, const ImagePoint& upperLeft,
							   const ImagePoint& upperRight, const ImagePoint& lowerLeft,
							   const ImagePoint& lowerRight)
{
//	int	balloonXPtMin = rect->getLeft() + 2*padding + 1,
//		balloonYPtMin = rect->getTop() + 2*padding + 1,
//		balloonHeight = rect->getHeight() - 4*padding - 2,
//		balloonWidth = rect->getWidth() - 4*padding - 2;
//
//	//number of contour points
//	float 	lengthPlusWidth = (balloonWidth + balloonHeight),
//			doubleWidth = balloonWidth * 2,
//			perimeter = 2 * lengthPlusWidth,
//			distBetweenPts =  perimeter / getNbPoints();
//
//	//set the initial positions of contour points around frame evenly, counter clockwise.
//	//since the lower-left corner is the origin point, so all coordinates are positive
//	int i = 0;
//	
//	list<ImagePoint_F*>::iterator iter, endList = contourList_.end();
//	for(iter = contourList_.begin(); iter != endList; i++, iter++){
//		float arcLengthOfPerimeter = (i * distBetweenPts);
//
//		if (arcLengthOfPerimeter < balloonWidth){
//			(*iter)->setCoordinates(balloonXPtMin + arcLengthOfPerimeter,
//								static_cast<float>(balloonYPtMin));
//		}else if ((arcLengthOfPerimeter >= balloonWidth) && 
//				(arcLengthOfPerimeter < lengthPlusWidth)){
//			(*iter)->setCoordinates(static_cast<float>(balloonXPtMin + balloonWidth),
//								balloonYPtMin + arcLengthOfPerimeter - balloonWidth);
//
//		}else if ((arcLengthOfPerimeter >= lengthPlusWidth)
//				&& (arcLengthOfPerimeter < (doubleWidth + balloonHeight))){
//			(*iter)->setCoordinates(balloonXPtMin + doubleWidth + balloonHeight - arcLengthOfPerimeter,
//								static_cast<float>(balloonYPtMin + balloonHeight));
//		}else{
//			(*iter)->setCoordinates(static_cast<float>(balloonXPtMin),
//								balloonYPtMin + perimeter - arcLengthOfPerimeter);
//		}
//	}

	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");
					
}


void Contour_F::initializeOval_(int nbPts, const ImagePoint& center, float width, float height, 
								float angle)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");					
}






