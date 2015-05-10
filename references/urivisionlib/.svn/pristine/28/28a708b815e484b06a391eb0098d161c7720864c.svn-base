/*  NAME:
        IntrinsicParameters_DLT.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib IntrinsicParameters_DLT class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "IntrinsicParameters_DLT.h"

using namespace uriVL;

IntrinsicParameters_DLT::IntrinsicParameters_DLT(const IntrinsicParameters_DLT *theParams)
	:	IntrinsicParameters(theParams),
        IntrinsicParameters_CCD(theParams, true, true),
        IntrinsicParameters_pinhole(theParams),
		//
		sigma_(theParams->sigma_),
		kappa1_(theParams->kappa1_)
{
}

IntrinsicParameters_DLT::IntrinsicParameters_DLT(int theWidth, int theHeight, 
												 double theFx, double theFy)
	:	IntrinsicParameters(theWidth, theHeight),
		IntrinsicParameters_CCD(theWidth, theHeight, true, true),
        IntrinsicParameters_pinhole(theWidth, theHeight, theFx, theFy),
		//
		sigma_(0.L),
		kappa1_(0.L)
{
}

IntrinsicParameters_DLT::IntrinsicParameters_DLT(int theWidth, int theHeight, int nbSensorsX, 
            							int nbSensorsY, double dx, double dy, double theFx,
										double theFy)
	:	IntrinsicParameters(theWidth, theHeight),
		IntrinsicParameters_CCD(theWidth, theHeight, nbSensorsX, nbSensorsY, dx, dy, true, true),
        IntrinsicParameters_pinhole(theWidth, theHeight, theFx, theFy),
		//
		sigma_(0.L),
		kappa1_(0.L)
{
}

IntrinsicParameters_DLT::IntrinsicParameters_DLT(const IntrinsicParameters_DLT *theParams, double originX,
            							double originY, double theFx, double theFy, double theSigma, 
										double theK1)
	:	IntrinsicParameters(theParams),
        IntrinsicParameters_CCD(theParams, true, true),
        IntrinsicParameters_pinhole(),
		//
		sigma_(theSigma),
		kappa1_(theK1)
{
	Ox_ = originX;
	Oy_ = originY;
	fx_ = theFx,
	fy_ = theFy;
}

IntrinsicParameters_DLT::IntrinsicParameters_DLT(int theWidth, int theHeight, int nbSensorsX, 
            							int nbSensorsY, double dx, double dy, double originX,
            							double originY, double theFx, double theFy, 
										double theSigma, double theK1)
	:	IntrinsicParameters(theWidth, theHeight),
		IntrinsicParameters_CCD(theWidth, theHeight, nbSensorsX, nbSensorsY, dx, dy, true, true),
        IntrinsicParameters_pinhole(theWidth, theHeight, originX, originY, theFx, theFy),
		//
		sigma_(theSigma),
		kappa1_(theK1)
{
}


IntrinsicParameters_DLT::~IntrinsicParameters_DLT(void)
{
}

#if 0
#pragma mark -
#endif

void IntrinsicParameters_DLT::setSkewing(double theSigma)

{
	sigma_ = theSigma;
}

double IntrinsicParameters_DLT::getSkewing(void) const

{
	return sigma_;
}


void IntrinsicParameters_DLT::setKappa1(double theK1)
{
	kappa1_ = theK1;
}

double IntrinsicParameters_DLT::getKappa1(void) const
{
	return kappa1_;
}

void IntrinsicParameters_DLT::setFocalLengthX(double theFx)
{
	fx_ = theFx;
}

void IntrinsicParameters_DLT::setFocalLengthY(double theFy)
{
	fy_ = theFy;
}

double IntrinsicParameters_DLT::getFocalLengthX(void) const
{
	return fx_;
}

double IntrinsicParameters_DLT::getFocalLengthY(void) const
{
	return fy_;
}

#if 0
#pragma mark -
#endif

//  Pixel distortion is not correct yet (role of sx_)
void IntrinsicParameters_DLT::distortPixels(double ux, double uy, double *dx, double *dy) const
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
void IntrinsicParameters_DLT::distortMetric(double ux, double uy, double *dx, double *dy) const
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
void IntrinsicParameters_DLT::undistortPixel(double dx, double dy, double *ux, double *uy) const
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
void IntrinsicParameters_DLT::undistortMetric(double dx, double dy, double *ux, double *uy) const
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


void IntrinsicParameters_DLT::metricToPixel(double mx, double my, double *px, double *py) const
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

void IntrinsicParameters_DLT::pixelToMetric(double px, double py, double *mx, double *my) const
{
    //	1.  apply image center translation
//     double	x = px - Ox_;
//     double	y = py - Oy_; // unused variable. -CJC

    //  2.  apply scale
    *mx = dpx_ * px;
    *my = dpy_ * py;
}
