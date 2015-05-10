/*  NAME:
        Pattern2DMatchRecord_part.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Pattern2DMatchRecord_part class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Pattern2DMatchRecord_part.h"

using namespace std;
using namespace uriVL;


Pattern2DMatchRecord_part::Pattern2DMatchRecord_part(void)
    :   Pattern2DMatchRecord()
{
    quadMatch_ = new Pattern2DMatchRecord*[4];
    for (int i=0; i<4; i++)
        quadMatch_[i] = new Pattern2DMatchRecord();

	recordType_ = "partial match record";
}


Pattern2DMatchRecord_part::~Pattern2DMatchRecord_part(void)
{
    for (int i=0; i<4; i++)
        delete quadMatch_[i];

    delete []quadMatch_;
}





//----------------------------------------------------------
//  public funcs
//----------------------------------------------------------

Pattern2DMatchRecord* Pattern2DMatchRecord_part::getMatchRecord(QuadrantIndex quad)
{
    return quadMatch_[quad];
}

float Pattern2DMatchRecord_part::getScore(QuadrantIndex quad)
{
    return quadMatch_[quad]->getScore();
}

ImagePoint* Pattern2DMatchRecord_part::getBestMatch(QuadrantIndex quad)
{
    return quadMatch_[quad]->getBestMatch();
}


void Pattern2DMatchRecord_part::setScore(QuadrantIndex quad, float score)
{
    quadMatch_[quad]->setScore(score);
}


void Pattern2DMatchRecord_part::setBestMatch(QuadrantIndex quad, int x, int y)
{
    quadMatch_[quad]->setBestMatch(x, y);
}

void Pattern2DMatchRecord_part::setBestMatch(QuadrantIndex quad, ImagePoint* bestMatch)
{
    quadMatch_[quad]->setBestMatch(bestMatch);
}



//----------------------------------------------------------
//  protected funcs
//----------------------------------------------------------

