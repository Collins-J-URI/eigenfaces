/*  NAME:
        ContourForce.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ContourForce class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <list>
//
#include "ContourForce.h"

using namespace std;
using namespace uriVL;


#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Constructors and destructor
//-------------------------------------------
#endif



ContourForce::ContourForce(void)
        :   DrawableObject2D(),
			contourSubject2Force_(NULL),
			force_(list<Vector2D_F*>()),
			renderContour_(true)
{
    needleColor_[0] = needleColor_[1] = needleColor_[2] = 0.f;
    needleColor_[3] = 1.0f;
}

ContourForce::ContourForce(const ContourForce& obj)
        :   DrawableObject2D(),
			contourSubject2Force_(NULL),
			force_(list<Vector2D_F*>()),
			renderContour_(false)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"ContourForce copy constructor not implemented");
}


ContourForce::~ContourForce(void)
{
	list<Vector2D_F*>::const_iterator	endList = force_.end();
	//delete each point in the list
	for(list<Vector2D_F*>::iterator iter = force_.begin(); iter!=endList; iter++)
		delete (*iter);
	//and for good measure just clear the list
	force_.clear();
	delete contourSubject2Force_;
}

const ContourForce& ContourForce::operator = (const ContourForce& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"ContourForce copy operator not implemented");
	return *this;
}



#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Getter functions
//-------------------------------------------
#endif


const ImageRect* ContourForce::getBoundRect(void) const
{
    return contourSubject2Force_->getBoundRect();
}

const ImageRect* ContourForce::getValidRect(void) const
{
    return contourSubject2Force_->getValidRect();
}



const list<Vector2D_F*> *ContourForce::getForce(void) const
{
    return &force_;
}

list<Vector2D_F*> *ContourForce::getForce(ReadWriteStatus rw)
{
    return &force_;
}

const Contour_F* ContourForce::getContour(void) const
{
    return contourSubject2Force_;
}

//Contour_F* ContourForce::getContour(ReadWriteStatus rw)
//{
//    return contourSubject2Force_;
//}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Force computations
//------------------------------------------------------
#endif

void ContourForce::mapToContour(Contour_F* contourIn){

	contourSubject2Force_ = contourIn;
	int   numPts = contourIn->getNbPoints();

	// basically if there was a force prior, delete em
	if(force_.size() > 0)
	{
		list<Vector2D_F*>::const_iterator endList = force_.end();
		//delete each point in the list
		for(list<Vector2D_F*>::const_iterator	iter = force_.begin(); iter!=endList; iter++)
			delete (*iter);
		//and for good measure just clear the list
		force_.clear();
	}
	
	Vector2D_F* myForce;
	for(int i = 0;i < numPts; i++)
	{
		myForce = new Vector2D_F(0.f, 0.f);
		force_.push_back(myForce);
	}
}


void ContourForce::insertIntoForceVector(int pointIndex, float valueX, float valueY)
{

	Vector2D_F *newPointForce = new Vector2D_F(valueX, valueY);
	list<Vector2D_F*>::iterator iterF = force_.begin();
	for(int i = 0; i < pointIndex; i++){
		iterF++;
	}
	force_.insert(iterF, newPointForce);
}

void ContourForce::addForce(ContourForce* cForce2){

	int force1ContourPts = contourSubject2Force_->getNbPoints();
	int force2ContourPts = (cForce2->getContour())->getNbPoints();

	// double check and make sure they both have the same amount of pts
	// before you add force vectors
        FAIL_CONDITION( force1ContourPts != force2ContourPts,
                    kNullParameterError,
                    "Adding two forces which are not the same size");

	if(force1ContourPts == force2ContourPts){
		const list<Vector2D_F*> *force2 = cForce2->getForce();
		//list<ImagePoint_F*> force2 = cForce2->force_;
		list<Vector2D_F*>::const_iterator iterF2 = (*force2).begin();

		for(list<Vector2D_F*>::const_iterator iterF1 = force_.begin(); iterF1 != force_.end(); iterF1++, iterF2++){
			(*iterF1)->setCoordinates((*iterF1)->getXF() + (*iterF2)->getXF(), 
									  (*iterF1)->getYF() + (*iterF2)->getYF());
		}
	}
}

// trivial... just set all the points in the force vector to zero's
void ContourForce::setForcesToZero()
{
	for(list<Vector2D_F*>::iterator iterF1 = force_.begin(); iterF1 != force_.end(); iterF1++)
	{
	    (*iterF1)->setCoordinates(0.f, 0.f);
	}
}

bool ContourForce::applyForceToContour(void){

	float 	avgDistance = 0.f, 
			maxDisp = 0.f, 
			minDisp = 100.f, 
			stoppingThreshold = .80f, 
			maxDispThreshold = 2.f;
	int numPts = contourSubject2Force_->getNbPoints();
	bool convergence = false;

	const list<ImagePoint_F*> *contourList = contourSubject2Force_->getPoints();
	list<Vector2D_F*>::iterator iterF = force_.begin();  
	  
	for(list<ImagePoint_F*>::const_iterator iter = contourList->begin(); iter != contourList->end(); iterF++, iter++)
	{
		float distancePtMoved = 0.f;	
		distancePtMoved = sqrtf((*iterF)->getX()*(*iterF)->getX() + (*iterF)->getY()*(*iterF)->getY()) ; 
		avgDistance += distancePtMoved;

		if(distancePtMoved > maxDisp)
		{
			maxDisp = distancePtMoved;
		}
		
		if(distancePtMoved < minDisp)
		{
			minDisp = distancePtMoved;
		}

		(*iter)->translate((*iterF)->getX(), (*iterF)->getY());
	}

	avgDistance /= numPts;

	// bottoms out at around .82f
	if((avgDistance < stoppingThreshold) && (maxDisp < maxDispThreshold)){
		convergence = true;
	}
	
	contourSubject2Force_->pointsHaveMoved();
	return convergence;
}




#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rendering
//------------------------------------------------------
#endif

void ContourForce::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						   float theZ, bool shifted, const ImagePoint* origin) const
{
    //--------------------------------------------------------------
    //	Part 1: render the contour 
    //--------------------------------------------------------------
    contourSubject2Force_->render(theZ, shifted, origin);
    
    //--------------------------------------------------------------
    //	Part 2: render the contour force's needles
    //--------------------------------------------------------------
    glLoadIdentity();
    //
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(scaledLeft, scaledTop, theZ);
	glScalef(getRenderingScaleX(), -getRenderingScaleY(), 1.f);

    const list<ImagePoint_F*> *contourList = contourSubject2Force_->getPoints();
    list<ImagePoint_F*>::const_iterator endList = contourList->end();
	list<Vector2D_F*>::const_iterator iterF = force_.begin();

    for(list<ImagePoint_F*>::const_iterator iter = contourList->begin(); iter != endList; iterF++, iter++)
    {
        glPushMatrix();
        glTranslatef((*iter)->getX(), (*iter)->getY(), 0.f);

        glColor3f(needleColor_[0], needleColor_[1], needleColor_[2]);
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(((*iterF)->getX() * VECTORSCALE),
                   ((*iterF)->getY() * VECTORSCALE));
        glEnd();

        glPopMatrix();	//	restore the state that had been pushed
    }

    glPopMatrix();
}

void ContourForce::setRenderContour(bool isRendered)
{
	renderContour_ = isRendered;
}

void ContourForce::setColor(float red, float green, float blue)
{
    contourSubject2Force_->setColor(red, green, blue);
}

void ContourForce::setColor(const float theColor[])
{
    contourSubject2Force_->setColor(theColor);
}


void ContourForce::setColor(unsigned char red, unsigned char green, unsigned char blue)
{
    contourSubject2Force_->setColor(red, green, blue);
}

void ContourForce::setColor(const unsigned char theColor[])
{
    contourSubject2Force_->setColor(theColor);
}


void ContourForce::setNeedleColor(float red, float green, float blue)
{
    needleColor_[0] = red;
    needleColor_[1] = green;
    needleColor_[2] = blue;
    needleColor_[3] = 1.f;
}

void ContourForce::setNeedleColor(const float theColor[])
{
    needleColor_[0] = theColor[0];
    needleColor_[1] = theColor[1];
    needleColor_[2] = theColor[2];
    needleColor_[3] = 1.f;
}

void ContourForce::setNeedleColor(unsigned char red, unsigned char green, unsigned char blue)
{
    needleColor_[0] = red/255.f;
    needleColor_[1] = green/255.f;
    needleColor_[2] = blue/255.f;
    needleColor_[3] = 1.f;
}

void ContourForce::setNeedleColor(const unsigned char theColor[])
{
    needleColor_[0] = theColor[0]/255.f;
    needleColor_[1] = theColor[1]/255.f;
    needleColor_[2] = theColor[2]/255.f;
    needleColor_[3] = 1.f;
}

void ContourForce::setDrawPoints(bool drawPointCircle)
{
	contourSubject2Force_->setDrawPoints(drawPointCircle);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Computation of geometric information
//------------------------------------------------------
#endif



void ContourForce::computeCentroid_(ImagePoint_F& cent) const {

	cent.setCoordinates(contourSubject2Force_->getCentroid());
}

void ContourForce::computeCenterOfMass_(ImagePoint_F& cent) const {

	cent.setCoordinates(contourSubject2Force_->getCenterOfMass());
}



// This is just a quick and dirty min max alg to get the dimensions
// of a contour
void ContourForce::computeBoundingRect_(ImageRect& rect) const
{
	rect.setRect(contourSubject2Force_->getBoundRect());
}

void ContourForce::computeValidRect_(ImageRect& rect) const
{	
	//	the valid rect is the same as the bounding rect.  If the bounding rect
	//	is up to date, use its dimensions, else recompute it first
	rect.setRect(contourSubject2Force_->getBoundRect());
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Geometrix transformations
//------------------------------------------------------
#endif

void ContourForce::rotate(float angle, const ImagePoint* rotCenter)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void ContourForce::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}

void ContourForce::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}



void ContourForce::setValidRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}
 
