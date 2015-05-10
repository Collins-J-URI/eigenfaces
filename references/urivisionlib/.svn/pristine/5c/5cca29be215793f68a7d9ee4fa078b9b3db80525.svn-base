/*  NAME:
        Contour_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Contour_I class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>
//
#include "Contour_I.h"
#include "Contour_F.h"

using namespace std;
using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

Contour_I::Contour_I(const Contour_I& ct)
	try	:   Contour(ct),
			//
			contourList_(list <ImagePoint*>(0)),
			edgeList_(list <ImagePoint* >(0))
{
    list<ImagePoint* >::const_iterator iter0, iter1;
    list<float>::const_iterator iter2;
    if (!(ct.contourList_.empty()))
    {
	    iter0 = ct.contourList_.begin();
	    for( ; iter0!=ct.contourList_.end(); iter0++)
	        contourList_.push_back(new ImagePoint(*iter0));
	}
    if (!(ct.edgeList_.empty()))
    {
	    iter1 = ct.edgeList_.begin();
	    for( ; iter1!=ct.edgeList_.end(); iter1++)
	        edgeList_.push_back(new ImagePoint(*iter1));
	}
	if (!(ct.ptDistance_.empty()))
	{
	    iter2 = ct.ptDistance_.begin();
	    for( ; iter2!=ct.ptDistance_.end(); iter2++)
	        ptDistance_.push_back(*iter2);
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by Contour_I constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to Contour_I constructor");
}

Contour_I::Contour_I(void)
		:   Contour(),
			//
			contourList_(list <ImagePoint*>()),
			edgeList_(list <ImagePoint* >())
{
}

Contour_I::Contour_I(const ImagePointList* ptsList)
		:   Contour(false),
			//
			contourList_(list <ImagePoint*>()),
			edgeList_(list <ImagePoint* >())
{
	try {
		for(ImagePointList::const_iterator iter = ptsList->begin() ;iter!=ptsList->end(); iter++)
		{
			contourList_.push_back(new ImagePoint(*iter));
		}
	}
	catch (...)
	{
		FAIL_CONDITION( ptsList == NULL,
						kNullParameterError,
						"Null pointer to point list passed to Contour_I constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid parameter passed to Contour_I constructor");
	}
}

Contour_I::Contour_I(int theNumPts, const ImagePoint*const* theContourPts)
		:   Contour(false),
			contourList_(list <ImagePoint*>()),
			edgeList_(list <ImagePoint* >())
{
	try {
		for(int i = 0;i < theNumPts; i++)
		{
			contourList_.push_back(new ImagePoint(theContourPts[i]));
		}
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

Contour_I::Contour_I(int theNumPts)
		:   Contour(false),
			contourList_(list <ImagePoint*>()),
			edgeList_(list <ImagePoint* >())
{
    for(int i = 0;i < theNumPts; i++)
    {
        contourList_.push_back(new ImagePoint(0, 0));
    }
}


Contour_I::~Contour_I()
{
	//	delete the point list
	list<ImagePoint*>::iterator	endPtList = contourList_.end();
	for(list<ImagePoint*>::iterator iter = contourList_.begin(); iter!=endPtList; iter++)
		delete (*iter);
	contourList_.clear();

	//	delete the edge list
	list<ImagePoint*>::iterator	endEdgeList = edgeList_.end();
	for(list<ImagePoint*>::iterator iter = edgeList_.begin(); iter!=endEdgeList; iter++)
		delete (*iter);
	edgeList_.clear();
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copying
//------------------------------------------------------
#endif


const Contour_I& Contour_I::operator = (const Contour_I& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Contour_I copy operator not implemented.");

	return *this;
}

void Contour_I::copyInto(Contour* contourOut) const
{
    //  so far I only accept another Contour_F as a destination
    FAIL_CONDITION( typeid(*contourOut) != typeid(uriVL::Contour_I),
                    kInvalidRasterType,
                    "Contour_I::copyInto only accepts Contour_I contours as destination");

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

const list<ImagePoint*> *Contour_I::getEdgeList(void)
{
    if (!edgeVectorsAreUpToDate_)
        computeEdgeVectorList_();
    return &edgeList_;
}


void Contour_I::setPoint(int pointNumber, int x, int y)
{
    if (static_cast<unsigned int>(pointNumber) < contourList_.size())
    {
        list<ImagePoint*>::iterator iter = contourList_.begin();

        for(int i = 0; i <= pointNumber; i++)
             iter++;

        (*iter)->setCoordinates(x, y);
    }
}

void Contour_I::setPoint(int pointNumber, float x, float y)
{
    if (static_cast<unsigned int>(pointNumber) < contourList_.size())
    {
        list<ImagePoint*>::iterator iter = contourList_.begin();

        for(int i = 0; i <= pointNumber; i++)
             iter++;

        (*iter)->setCoordinates((int) round(x), (int) round(y));
    }
}



int Contour_I::getNbPoints(void) const
{
    return static_cast<int>(contourList_.size());
}


const list <ImagePoint*> *Contour_I::getPoints(void) const 
{
	return &contourList_;
}

list <ImagePoint*> *Contour_I::getPoints(ReadWriteStatus rw){
	return &contourList_;
}





#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Geometric operations
//------------------------------------------------------
#endif


void Contour_I::applyTransformation(const Transformation2D* theTransf)
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

void Contour_I::translate(int tx, int ty )
{
    list<ImagePoint*>::iterator iter, endList = contourList_.end();
    for(iter = contourList_.begin(); iter != endList; iter++)
        (*iter)->translate(tx, ty);

}

void Contour_I::translate(float tx, float ty )
{
    const int   itx = static_cast<int>(round(tx)),
                ity = static_cast<int>(round(ty));
            
    list<ImagePoint*>::iterator iter, endList = contourList_.end();
    for(iter = contourList_.begin(); iter != endList; iter++)
        (*iter)->translate(itx, ity);

}

void Contour_I::rotate(float angle, const ImagePoint* rotCenter)
{
    float	cosA = cosf(angle),
            sinA = sinf(angle);

    list<ImagePoint*>::iterator iter, endList = contourList_.end();
    for(iter = contourList_.begin(); iter != endList; iter++)
    {
        int 	copyOfCntourPtsX = (*iter)->getX(),
                copyOfCntourPtsY = (*iter)->getY();
        (*iter)->setCoordinates((int)(copyOfCntourPtsX * cosA - copyOfCntourPtsY * sinA),
                                (int)(copyOfCntourPtsX * sinA + copyOfCntourPtsY * cosA));
    }

	pointsHaveMoved();
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Geometric tests
//------------------------------------------------------
#endif

bool Contour_I::contains(float x, float y) const
{
    return contains((int) x, (int) y);
}

bool Contour_I::contains(int x, int y) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_I::contains(const ImagePoint* contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_I::contains(const ImagePoint_F *contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}


bool Contour_I::contains(const Contour_I *contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_I::contains(const Contour_F *contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_I::isInside(const Contour_I *contour) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
    return false;
}

bool Contour_I::isInside(const Contour_F *contour) const
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


void Contour_I::computeCentroid_(ImagePoint_F& cent) const {
	ImageRect bRect;
	
	computeBoundingRect_(bRect);

	cent.setCoordinates(0.5f*(bRect.getLeft() + bRect.getRight()), 
						0.5f*(bRect.getTop() + bRect.getBottom()));

}

void Contour_I::computeCenterOfMass_(ImagePoint_F& cent) const {

	int		theNumPts = 0;
	float 	averageX = 0.f, 
			averageY = 0.f;

	//iterate through the list and add the coordinate values.
	list<ImagePoint*>::const_iterator iter, endList = contourList_.end();
	for(iter = contourList_.begin(); iter != endList; iter++){
		theNumPts++;
		averageX += (*iter)->getX();
		averageY += (*iter)->getY();
	}
	averageX /= theNumPts;
	averageY /= theNumPts;

	cent.setCoordinates(averageX, averageY);
}

void Contour_I::computeInterPointDistances_(void) const
{
    if (!interPointDistancesAreUpToDate_)
    {
        if (!edgeVectorsAreUpToDate_)
            computeEdgeVectorList_();
        
        ptDistance_.clear();
        list<ImagePoint*>::const_iterator iter, itp1,
                                    firstList = contourList_.begin(),
                                    endList = contourList_.end();
        for(iter = firstList, itp1 = firstList; iter != endList; iter++)
        {
            itp1++;

            if(itp1 == contourList_.end() )
                itp1 = contourList_.begin();

			// calculate the distance from the current point to
			// the next
            float   dx = (*itp1)->getX() - (*iter)->getX(),
                    dy = (*itp1)->getY() - (*iter)->getY();
                    
	        ptDistance_.push_back(sqrtf(dx*dx + dy*dy));
        }
        interPointDistancesAreUpToDate_ = true;
    }
}

void Contour_I::computeEdgeVectorList_(void) const
{
    if (!edgeVectorsAreUpToDate_)
    {
        ImagePoint* myPoint;

        edgeList_.clear();
        list<ImagePoint*>::const_iterator endList = contourList_.end();
        for(list<ImagePoint*>::const_iterator iter = contourList_.begin(), itp1 = iter; iter != endList; iter++)
        {
            itp1++;

            if(itp1 == contourList_.end() )
                itp1 = contourList_.begin();

            myPoint = new ImagePoint((*itp1)->getX() - (*iter)->getX(), 
                                     (*itp1)->getY() - (*iter)->getY());
            edgeList_.push_back(myPoint);
        }
        edgeVectorsAreUpToDate_ = true;
    }
}


void Contour_I::computeBoundingRect_(ImageRect& rect) const
{
	list<ImagePoint*>::const_iterator firstIter = contourList_.begin();
	int 	maxX = (*firstIter)->getX(),
			minX = maxX,
			maxY = (*firstIter)->getY(),
			minY = maxY;

	list<ImagePoint*>::const_iterator endIter = contourList_.end();
    for(list<ImagePoint*>::const_iterator iter = firstIter; iter != endIter; iter++)
    {
        int	xi  = (*iter)->getX(),
			yi = (*iter)->getY();

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
				static_cast<int>(maxX - minX + 1), static_cast<int>(maxY - minY + 1));

}

void Contour_I::computeValidRect_(ImageRect& rect) const
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


void Contour_I::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						  float theZ, bool shifted, const ImagePoint* origin) const
{
	const int	left = getLeft(),
				top = getTop();

	//--------------------------------------------------------------
	//	Render the contour
	//--------------------------------------------------------------
	//
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(scaledLeft, scaledTop, theZ);
	glScalef(getRenderingScaleX(), -getRenderingScaleY(), 1.f);

	glColor4fv(getColor());
	list<ImagePoint*>::const_iterator endList = contourList_.end();

	glBegin(GL_LINE_LOOP);
		for(list<ImagePoint*>::const_iterator iter = contourList_.begin(); iter != endList; iter++)
		{
			glVertex2f( (*iter)->getX() - left, (*iter)->getY() - top);
		}
	glEnd();

	if(drawPoints_)
	{
		glColor4fv(vertexColor_);
		
        for(list<ImagePoint*>::const_iterator iter = contourList_.begin(); iter != endList; iter++)
		{
            glPushMatrix();
            glTranslatef((*iter)->getX() - left, (*iter)->getY() - top, 0);
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
#pragma mark Contour initialization functions
//------------------------------------------------------
#endif


void Contour_I::initializeBox_(const ImageRect& rect, int padding)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");
					
}



void Contour_I::initializeBox_(int nbPts, const ImagePoint& upperLeft,
							   const ImagePoint& upperRight, const ImagePoint& lowerLeft,
							   const ImagePoint& lowerRight)
{
//    int balloonXPtMin = rect->getLeft() + 2*padding + 1,
//        balloonYPtMin = rect->getTop() + 2*padding + 1,
//        balloonHeight = rect->getHeight() - 4*padding - 2,
//        balloonWidth = rect->getWidth() - 4*padding - 2;
//
//    //number of contour points
//    float 	lengthPlusWidth = (balloonWidth + balloonHeight),
//            doubleWidth = balloonWidth * 2,
//            perimeter = 2 * lengthPlusWidth,
//            distBetweenPts =  perimeter / getNbPoints();
//
//    //set the initial positions of contour points around frame evenly, counter clockwise.
//    //since the lower-left corner is the origin point, so all coordinates are positive
//    int i = 0;
//    list<ImagePoint*>::const_iterator endList = contourList_.end();
//    for(list<ImagePoint*>::iterator iter = contourList_.begin(); iter != endList; i++, iter++)
//    {
//        float arcLengthOfPerimeter = (i * distBetweenPts);
//
//        if (arcLengthOfPerimeter < balloonWidth)
//            (*iter)->setCoordinates((int) (balloonXPtMin + arcLengthOfPerimeter),
//                                    balloonYPtMin);
//
//        else if ((arcLengthOfPerimeter >= balloonWidth) && 
//                 (arcLengthOfPerimeter < lengthPlusWidth))
//            (*iter)->setCoordinates(balloonXPtMin + balloonWidth,
//                                    (int)(balloonYPtMin + arcLengthOfPerimeter - balloonWidth));
//
//        else if (arcLengthOfPerimeter >= lengthPlusWidth
//                 && (arcLengthOfPerimeter < (doubleWidth + balloonHeight)))
//            (*iter)->setCoordinates((int)(balloonXPtMin + doubleWidth + balloonHeight - 
//                                        arcLengthOfPerimeter),
//                                    balloonYPtMin + balloonHeight);
//        else
//            (*iter)->setCoordinates(balloonXPtMin,
//                                    (int)(balloonYPtMin + perimeter - arcLengthOfPerimeter));
//    }

	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");
					
}




void Contour_I::initializeOval_(int nbPts, const ImagePoint& center, float width, float height, 
								float angle)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"This contour intializer is not implemented yet.");					
}
