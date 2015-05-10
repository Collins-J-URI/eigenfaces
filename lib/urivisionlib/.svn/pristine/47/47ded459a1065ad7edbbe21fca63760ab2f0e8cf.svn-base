/*  NAME:
        Disparity.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Disparity class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <typeinfo>
//
#include "Disparity.h"
#include "ImagePoint_F.h"

using namespace uriVL;


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors, destructors, operators
//----------------------------------------------------------
#endif

Disparity::Disparity(bool isSubpixelDisp)
		:	disparity_(new Vector2D_F(0.f, 0.f)),
			isSubpixelDisparity_(isSubpixelDisp),
			score_(0.f)
{
}

Disparity::Disparity(const Disparity* disp)
	try	:	disparity_(new Vector2D_F(disp->disparity_)),
			isSubpixelDisparity_(disp->isSubpixelDisparity_),
			score_(disp->score_)
{
	FAIL_CONDITION( disp == NULL,
                    kNullImagePointError,
                    "NULL ImagePoint passed as parameter to Disparity constructor");

}
catch (ErrorReport& e) {
	e.appendToMessage("called by Disparity constructor");
	throw e;
}


Disparity::Disparity(const Disparity& disp)
	try	:	disparity_(new Vector2D_F(disp.disparity_)),
			isSubpixelDisparity_(disp.isSubpixelDisparity_),
			score_(disp.score_)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by Disparity constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid ImagePoint passed as parameter to Disparity constructor");
}


Disparity::Disparity(const Vector2D* disp, float score)
	try	:	disparity_(new Vector2D_F(disp)),
			isSubpixelDisparity_(false),
			score_(score)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by Disparity constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( disp == NULL,
                    kNullImagePointError,
                    "NULL ImagePoint passed as parameter to Disparity constructor");

	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid ImagePoint passed as parameter to Disparity constructor");
}

Disparity::Disparity(int dx, int dy, float score)
		:	disparity_(new Vector2D_F(dx, dy)),
			isSubpixelDisparity_(false),
			score_(score)
{
}


Disparity::Disparity(float dx, float dy, float score, bool isSubpixelDisp)
		:	disparity_(new Vector2D_F(dx, dy)),
			isSubpixelDisparity_(isSubpixelDisp),
			score_(score)
{
}


Disparity::~Disparity(void)
{
    if (disparity_ != NULL)
        delete disparity_;
        
}

const Disparity& Disparity::operator = (const Disparity& obj)
{
	disparity_ = obj.disparity_;
	score_ = obj.score_;
	isSubpixelDisparity_ = obj.isSubpixelDisparity_;
	
	return *this;
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif

const Vector2D_F* Disparity::getDisparity(void) const
{
	return disparity_;
}

Vector2D_F* Disparity::getDisparity(ReadWriteStatus rw)
{
	return disparity_;
}

void Disparity::setDisparity(const Vector2D* disp)
{

	try {
		if (typeid(*disp) == typeid(uriVL::ImagePoint_F))
			setDisparity(static_cast<const Vector2D_F*>(disp));
		else
		{
			disparity_->setCoordinates(disp->getX(), disp->getY());
			isSubpixelDisparity_ = true;
		}
	}
	catch(...) {
		FAIL_CONDITION( disp == NULL,
						kNullImagePointError,
						"NULL ImagePoint passed as parameter to Disparity::setDisparity");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid coordinate pointer passed to ImagePoint_F::setCoordinates.");
	}

}

void Disparity::setDisparity(const Vector2D_F* disp)
{

	try {
		disparity_->setCoordinates(disp->getXF(), disp->getYF());

		isSubpixelDisparity_ = true;
	}
	catch(...) {
		FAIL_CONDITION( disp == NULL,
						kNullImagePointError,
						"NULL ImagePoint passed as parameter to Disparity::setDisparity");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid coordinate pointer passed to ImagePoint_F::setCoordinates.");
	}

}

void Disparity::setDisparity(int x, int y)
{
	disparity_->setCoordinates(x, y);
	isSubpixelDisparity_ = false;
}

void Disparity::setDisparity(float x, float y, bool isSubpixelDisp)
{
	disparity_->setCoordinates(x, y);
	isSubpixelDisparity_ = isSubpixelDisp;
}


float Disparity::getScore(void)
{
	return score_;
}    		
    		
void Disparity::setScore(float conf)
{
	score_ = conf;
}

bool Disparity::isSubpixelDisparity(void) const
{
    return isSubpixelDisparity_;
}

