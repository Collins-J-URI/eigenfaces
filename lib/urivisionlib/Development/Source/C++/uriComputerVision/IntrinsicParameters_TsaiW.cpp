/*  NAME:
        IntrinsicParameters_TsaiW.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib IntrinsicParameters_TsaiW class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "IntrinsicParameters_TsaiW.h"

using namespace uriVL;

IntrinsicParameters_TsaiW::IntrinsicParameters_TsaiW(const IntrinsicParameters_TsaiW* theParams)
	try	:	IntrinsicParameters(theParams),
			IntrinsicParameters_CCD(theParams),
			IntrinsicParameters_pinhole(theParams),
			//
			sx_(theParams->sx_),
			kappa1_(theParams->kappa1_),
			f_(theParams->f_)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_TsaiW constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( theParams == NULL,
					kNullParameterError,
					"Null pointer passed to IntrinsicParameters_TsaiW constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_TsaiW constructor");
}


IntrinsicParameters_TsaiW::IntrinsicParameters_TsaiW(int theWidth, int theHeight, 
														double theF)
	try	:	IntrinsicParameters(theWidth, theHeight),
			IntrinsicParameters_CCD(theWidth, theHeight),
			IntrinsicParameters_pinhole(theWidth, theHeight, theF, theF),
			//
			sx_(1.L),
			kappa1_(0.L),
			f_(theF)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_TsaiW constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_TsaiW constructor");
}


IntrinsicParameters_TsaiW::IntrinsicParameters_TsaiW(int theWidth, int theHeight, int nbSensorsX, 
            							int nbSensorsY, double dx, double dy, double theF)
	try	:	IntrinsicParameters(theWidth, theHeight),
			IntrinsicParameters_CCD(theWidth, theHeight, nbSensorsX, nbSensorsY, dx, dy),
			IntrinsicParameters_pinhole(theWidth, theHeight, 0.5f * (theWidth - 1), 0.5f * (theHeight - 1), theF, theF),
			//
			sx_(1.L),
			kappa1_(0.L),
			f_(theF)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_TsaiW constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_TsaiW constructor");
}


IntrinsicParameters_TsaiW::IntrinsicParameters_TsaiW(const IntrinsicParameters_TsaiW* thePrecalibParams, double originX,
            							double originY, double theF, double theSx, double theK1)
	try	:	IntrinsicParameters(thePrecalibParams),
			IntrinsicParameters_CCD(thePrecalibParams),
			IntrinsicParameters_pinhole(thePrecalibParams->getWidth(), thePrecalibParams->getHeight(), 
										originX, originY, theF, theF),
			//
			sx_(theSx),
			kappa1_(theK1),
			f_(theF)			
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_TsaiW constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( thePrecalibParams == NULL,
					kNullParameterError,
					"Null pointer passed to IntrinsicParameters_TsaiW constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_TsaiW constructor");
}


IntrinsicParameters_TsaiW::IntrinsicParameters_TsaiW(int theWidth, int theHeight, int nbSensorsX, 
            							int nbSensorsY, double dx, double dy, double originX,
            							double originY, double theF, double theSx, double theK1)
	try	:	IntrinsicParameters(theWidth, theHeight),
			IntrinsicParameters_CCD(theWidth, theHeight, nbSensorsX, nbSensorsY, dx, dy),
			IntrinsicParameters_pinhole(theWidth, theHeight, originX, originY, theF, theF),
			//
			sx_(theSx),
			kappa1_(theK1),
			f_(theF)			
{
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_TsaiW constructor");
}


IntrinsicParameters_TsaiW::~IntrinsicParameters_TsaiW(void)
{
}

#if 0
#pragma mark -
#endif

bool IntrinsicParameters_TsaiW::hasDistortion(void) const
{
	return false;
}

bool IntrinsicParameters_TsaiW::hasUndistortion(void) const
{
	return false;
}


void IntrinsicParameters_TsaiW::setPixelShapeScale(double theSx)
{
	sx_ = theSx;
}

double IntrinsicParameters_TsaiW::getPixelShapeScale(void) const
{
	return sx_;
}

void IntrinsicParameters_TsaiW::setKappa1(double theK1)
{
	kappa1_ = theK1;
}

double IntrinsicParameters_TsaiW::getKappa1(void) const
{
	return kappa1_;
}

void IntrinsicParameters_TsaiW::setFocalLength(double theF)
{
	f_ = theF;
}

double IntrinsicParameters_TsaiW::getFocalLength(void) const
{
	return f_;
}

#if 0
#pragma mark -
#endif

//  Pixel distortion is not correct yet (role of sx_)
void IntrinsicParameters_TsaiW::distortPixels(double ux, double uy, double* dx, double* dy) const
{
    //  1.  move to the center
    double   cx = ux - Ox_;
    double   cy = uy - Oy_;
    
    //  2.  calculate scaling factor
    const double  ru2 = cx*cx + cy*cy;
    const double  p = 1.L / (kappa1_ * ru2);
    const double  p2 = p*p;
    const double  bigTerm = 9.L*p + sqrt(81.L*p2 + 12.L*p2*p);
    const double  mu = 1.f + double(  bigTerm * pow(18, -1.L/3.L) - 
                                    pow(2.L/3.L, 1.L/3.L) / bigTerm);
    
    //  3.  apply scaling
    *dx = mu * cx;
    *dy = mu * cy;
    
    //  4.  apply translation back
    *dx += Ox_;
    *dy += Oy_;
}

//  Pixel distortion is not correct yet (role of sx_)
void IntrinsicParameters_TsaiW::distortMetric(double ux, double uy, double* dx, double* dy) const
{
    //  the input and output coordinates are already centered.  No need
    //  to apply any translation

    //  1.  calculate scaling factor
    const double  ru2 = ux*ux + uy*uy;
    const double  p = 1.L / (kappa1_ * ru2);
    const double  p2 = p*p;
    const double  bigTerm = 9.L*p + sqrt(81.L*p2 + 12.L*p2*p);
    const double  mu = 1.f + double(  bigTerm * pow(18, -1.L/3.L) - 
                                    pow(2.L/3.L, 1.L/3.L) / bigTerm);
    
    //  2.  apply scaling
    *dx = mu * ux;
    *dy = mu * uy;
}

//  Pixel distortion is not correct yet (role of sx_)
void IntrinsicParameters_TsaiW::undistortPixel(double dx, double dy, double* ux, double* uy) const
{
    //  1.  move to the center
    double   cx = dx - Ox_;
    double   cy = dy - Oy_;
    
    //  2.  calculate scaling factor
    double   rd2 = cx*cx + cy*cy;
    double   mu = 1.f + (kappa1_ * rd2);
    
    //  3.  apply scaling
    *ux = mu * cx;
    *uy = mu * cy;
    
    //  4.  apply translation back
    *ux += Ox_;
    *uy += Oy_;
}

//  Pixel distortion is not correct yet (role of sx_)
void IntrinsicParameters_TsaiW::undistortMetric(double dx, double dy, double* ux, double* uy) const
{
    //  the input and output coordinates are already centered.  No need
    //  to apply any translation

    //  1.  calculate scaling factor
    double   rd2 = dx*dx + dy*dy;
    double   mu = 1.f + (kappa1_ * rd2);
    
    //  2.  apply scaling
    *ux = mu * dx;
    *uy = mu * dy;
}


void IntrinsicParameters_TsaiW::metricToPixel(double mx, double my, double* px, double* py) const
{
    //  1.  scaling ratios meter to pixel
    double   scaleX = 1.f / dpx_;
    double   scaleY = 1.f / dpy_;
    
    //  2.  apply scale
    *px = scaleX * mx;
    *py = scaleY * my;
    
    //	3.  apply image center translation
    *px += Ox_;
    *py += Oy_;
}

void IntrinsicParameters_TsaiW::pixelToMetric(double px, double py, double* mx, double* my) const
{
    //	1.  apply image center translation
//     double	x = px - Ox_;
//     double	y = py - Oy_; // unused variable. -CJC

    //  2.  apply scale
    *mx = dpx_ * px;
    *my = dpy_ * py;
}
