/*  NAME:
        Pattern2DMatchRecord.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Pattern2DMatchRecord class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Pattern2DMatchRecord.h"

using namespace std;
using namespace uriVL;


Pattern2DMatchRecord::Pattern2DMatchRecord(void)
{
	bestMatch_ = new ImagePoint(0, 0);
	score_ = 0.f;
	recordType_ = "generic record ";
}

Pattern2DMatchRecord::Pattern2DMatchRecord(const ImagePoint* bestMatch, float score)
{
	bestMatch_ = new ImagePoint(bestMatch);
	score_ = score;
	recordType_ = "generic record ";
}

Pattern2DMatchRecord::Pattern2DMatchRecord(int x, int y, float score)
{
	bestMatch_ = new ImagePoint(x, y);
	score_ = score;
	recordType_ = "generic record ";
}


Pattern2DMatchRecord::~Pattern2DMatchRecord(void)
{
	delete bestMatch_;
}




//----------------------------------------------------------
//  public funcs
//----------------------------------------------------------

float Pattern2DMatchRecord::getScore(void)
{
    return score_;
    
}

ImagePoint* Pattern2DMatchRecord::getBestMatch(void)
{
    return bestMatch_;
}

void Pattern2DMatchRecord::setScore(float score)
{
    score_ = score;
    
}

void Pattern2DMatchRecord::setBestMatch(int x, int y)
{
    if (bestMatch_ == NULL)
        bestMatch_ = new ImagePoint(x, y);
    else
        bestMatch_->setCoordinates(x, y);
        
}

void Pattern2DMatchRecord::setBestMatch(const ImagePoint* bestMatch)
{
    FAIL_CONDITION( bestMatch == NULL,
                    kNullImagePointError,
                    "null ImagePoint passed to Pattern2DMatchRecord::setBestMatch");
                    
    if (bestMatch_ == NULL)
        bestMatch_ = new ImagePoint(bestMatch);
    else
        bestMatch_->setCoordinates(bestMatch->getX(), bestMatch->getY());
        
}


const string Pattern2DMatchRecord::getRecordType(void) const
{
    return recordType_;
}


//----------------------------------------------------------
//  protected funcs
//----------------------------------------------------------

