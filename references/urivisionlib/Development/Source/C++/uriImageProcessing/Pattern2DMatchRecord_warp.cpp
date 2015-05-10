/*  NAME:
        Pattern2DMatchRecord_warp.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Pattern2DMatchRecord_warp class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Pattern2DMatchRecord_warp.h"

using namespace std;
using namespace uriVL;


Pattern2DMatchRecord_warp::Pattern2DMatchRecord_warp(void)
    :   Pattern2DMatchRecord()
{
	isSubpixelMatch_ = true;
	revContour_ = NULL;
	grid_ = NULL;
}


Pattern2DMatchRecord_warp::~Pattern2DMatchRecord_warp(void)
{
/*
    if (revContour_ != NULL)
    	delete revContour_;
    	
    if (grid_ != NULL)
    {
    	for (int i=0; i<gridRows_; i++)
    		delete []grid_[i];
    	delete []grid_;
    }	

    if (revGrid_ != NULL)
    {
    	for (int i=0; i<gridRows_; i++)
    		delete []revGrid_[i];
    	delete []revGrid_;
    }	
*/
}





//----------------------------------------------------------
//  public funcs
//----------------------------------------------------------

void Pattern2DMatchRecord_warp::setWarpingGrid(float*** const grid)
{
	grid_ = grid;
}


float	***const Pattern2DMatchRecord_warp::getWarpingGrid(void)
{
	return grid_;
}

/*
float	***const Pattern2DMatchRecord_warp::getReverseWarpingGrid(void)
{
	return revGrid_;
}
*/

Contour_I *Pattern2DMatchRecord_warp::getReverseWarpContour(void)
{
	return revContour_;
}

OptimizerRecordND *Pattern2DMatchRecord_warp::getOptimizerRecordND(void)
{
	return optRecND_;
}

/*
void Pattern2DMatchRecord_warp::setReverseWarpingGrid(float	***const revGrid)
{
	revGrid_ = revGrid;
}
*/

void Pattern2DMatchRecord_warp::setReverseWarpContour(Contour_I *revContour)
{
	revContour_ = revContour;
}

//----------------------------------------------------------
//  protected funcs
//----------------------------------------------------------

