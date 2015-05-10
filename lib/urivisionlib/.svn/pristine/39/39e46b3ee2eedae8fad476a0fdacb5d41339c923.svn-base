/*  NAME:
        GlUtils::.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PatternMatcher2D_warp class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "PatternMatcher2D_warp.h"
#include "Pattern2DMatchRecord_warp.h"

using namespace std;
using namespace uriVL;

#define DEBUG	1
//----------------------------------------------------------
//  Static members
//----------------------------------------------------------
const float PatternMatcher2D_warp::OPT_TOL = static_cast<float>(3.0e-02);


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

PatternMatcher2D_warp::PatternMatcher2D_warp(BasePatternMatcher2DType matcherType,
            		                         PowellMethodCode powellCode,
        			                         ImageWarperType warperType,
            		                         int gridRows, 
            		                         int gridCols,
        			                         InnerWarpingMode innerMode, 
            		                         BorderWarpingMode outerMode)
            : PatternMatcher2D()
{
    isSubpixelMatcher_ = true;
    wcf_ = new WarpMatchScoreFunctionND(matcherType, warperType, gridRows, gridCols,
        			                    innerMode, outerMode);

    optN_ = new NonlinearOptimizerND_conjDir_Powell(wcf_, powellCode);
    nDim_ = optN_->getDimension();
    optRecND_ = new OptimizerRecordND(nDim_);

	xyStart_ = wcf_->newVarArray();
	xyMin_ = wcf_->newVarArray();
	xyMax_ = wcf_->newVarArray();
	
	gridRows_ = gridRows;
	gridCols_ = gridCols;
	
	gridInit_ = NULL;
	gridRev_ = NULL;
	rwContour_ = NULL;
	baseMatcherType_ = PATTERN_MATCHER_2D_NON_BASIC;
}


PatternMatcher2D_warp::PatternMatcher2D_warp(BasePatternMatcher2DType matcherType,
            		                         PowellMethodCode powellCode,
			                                 ImageWarper *warper)
            : PatternMatcher2D()
{
    isSubpixelMatcher_ = true;

    wcf_ = new WarpMatchScoreFunctionND(matcherType, warper);

    optN_ = new NonlinearOptimizerND_conjDir_Powell(wcf_, powellCode);
    nDim_ = optN_->getDimension();
    optRecND_ = new OptimizerRecordND(nDim_);

	xyStart_ = wcf_->newVarArray();
	xyMin_ = wcf_->newVarArray();
	xyMax_ = wcf_->newVarArray();
	
	gridRows_ = warper->getNbGridRows();
	gridCols_ = warper->getNbGridCols();
	
	gridInit_ = NULL;
	gridRev_ = NULL;
	rwContour_ = NULL;
	baseMatcherType_ = PATTERN_MATCHER_2D_NON_BASIC;
}


PatternMatcher2D_warp::PatternMatcher2D_warp(PatternMatcher2D* matcher,
            		                         PowellMethodCode powellCode,
        			                         ImageWarperType warperType,
            		                         int gridRows, 
            		                         int gridCols,
        			                         InnerWarpingMode innerMode, 
            		                         BorderWarpingMode outerMode)
            : PatternMatcher2D()
{
    isSubpixelMatcher_ = true;

    wcf_ = new WarpMatchScoreFunctionND(matcher, warperType, gridRows, gridCols,
        			                    innerMode, outerMode);

    optN_ = new NonlinearOptimizerND_conjDir_Powell(wcf_, powellCode);
    nDim_ = optN_->getDimension();
    optRecND_ = new OptimizerRecordND(nDim_);

	xyStart_ = wcf_->newVarArray();
	xyMin_ = wcf_->newVarArray();
	xyMax_ = wcf_->newVarArray();
	
	gridRows_ = gridRows;
	gridCols_ = gridCols;
	
	gridInit_ = NULL;
	gridRev_ = NULL;
	rwContour_ = NULL;
	baseMatcherType_ = PATTERN_MATCHER_2D_NON_BASIC;
}

PatternMatcher2D_warp::PatternMatcher2D_warp(PatternMatcher2D* matcher,
            		                         PowellMethodCode powellCode,
			                                 ImageWarper *warper)
            : PatternMatcher2D()
{
    isSubpixelMatcher_ = true;

    wcf_ = new WarpMatchScoreFunctionND(matcher, warper);

    optN_ = new NonlinearOptimizerND_conjDir_Powell(wcf_, powellCode);
    nDim_ = optN_->getDimension();
    optRecND_ = new OptimizerRecordND(nDim_);

	xyStart_ = wcf_->newVarArray();
	xyMin_ = wcf_->newVarArray();
	xyMax_ = wcf_->newVarArray();
	
	gridRows_ = warper->getNbGridRows();
	gridCols_ = warper->getNbGridCols();
	
	gridInit_ = NULL;
	gridRev_ = NULL;
	rwContour_ = NULL;
	baseMatcherType_ = PATTERN_MATCHER_2D_NON_BASIC;
}




PatternMatcher2D_warp::~PatternMatcher2D_warp(void)
{
	delete optN_;
	delete optRecND_;
	delete wcf_;
	
	if (rwContour_ != NULL)
	    delete rwContour_;
	    
	deleteGrids_();
}


#if 0
#pragma mark -
#endif
//----------------------------------------------------------
//	set the image and pattern
//----------------------------------------------------------


void PatternMatcher2D_warp::setPattern(const Pattern2D* pat)
{
    FAIL_CONDITION(	pat == NULL,
                    kNullParameterError,
                    "NULL pattern sent as parameter to PatternMatcher2D_warp::setPattern");
    setPattern(pat->getPattern(), pat->getBlob());
}

void PatternMatcher2D_warp::setPattern(const RasterImage* patImg, const Blob* patBlob)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL image sent as parameter to PatternMatcher2D_warp::setPattern");
    
    BaseRasterType patRasterType = patImg->getBaseRasterType();
    FAIL_CONDITION(	(patRasterType != kGrayRaster) && (patRasterType != kRGBa32Raster),
                    kInvalidParameterError,
                    "PatternMatcher2D_corr::setPattern only takes gray and RGBa images");

    wcf_->setPattern(patImg, patBlob);
	optN_->setFunction(wcf_);

    patImg_ = patImg;
    patBlob_ = patBlob;
    patRasterType_ = patImg->getBaseRasterType();
}

void PatternMatcher2D_warp::setImage(const RasterImage* img, const ImagePoint* matchPoint)
{
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL image sent as parameter to PatternMatcher2D_corr::setImage");
    
    FAIL_CONDITION(	img->getBaseRasterType() != patRasterType_,
                    kInvalidParameterError,
                    "image type must match pattern type (so far)");

    wcf_->setImage(img, matchPoint);
	optN_->setFunction(wcf_);

    img_ = img;
    computeGridBounds_();
}


void PatternMatcher2D_warp::getImageSuitableDimensions(const int imgWidth, 
                                            const int imgHeight,
                                            int* lowImgWidth, int* lowImgHeight,
                                            int* highImgWidth, int* highImgHeight)
{
    wcf_->getImageSuitableDimensions(imgWidth, imgHeight, lowImgWidth, lowImgHeight,
                                        highImgWidth, highImgHeight);
}


RasterImage* PatternMatcher2D_warp::getWarpedImage(void)
{
    return wcf_->getWarpedImage();
}

float*** const PatternMatcher2D_warp::getInitGrid(void)
{
    return gridInit_;
}

#if 0
#pragma mark -
#endif
//----------------------------------------------------------
//	match functions
//----------------------------------------------------------

Pattern2DMatchRecord* PatternMatcher2D_warp::match(const Pattern2D* patt2D,
												   const ImagePoint* matchPoint,
												   const RasterImage* img)
{
	Pattern2DMatchRecord_warp *matchRecord = new Pattern2DMatchRecord_warp();
	
	match(patt2D, matchPoint, img, matchRecord);
	
	return matchRecord;
}

void PatternMatcher2D_warp::match(const Pattern2D* patt2D,
								  const ImagePoint* matchPoint,
								  const RasterImage* img,
								  Pattern2DMatchRecord* matchRecord)
{
    FAIL_CONDITION( !isProperMatchRecordType_(matchRecord),
                    kInvalidParameterError,
                    "Pattern2DMatchRecord_warp required");
    Pattern2DMatchRecord_warp *warpMatchRecord = (Pattern2DMatchRecord_warp*) matchRecord;;

    setPattern(patt2D);
    setImage(img, matchPoint);

	switch (img->getBaseRasterType())
	{
		case kGrayRaster:
			match_((RasterImage_gray* ) patt2D->getPattern(), patt2D->getBlob(), NULL,
				   (RasterImage_gray* ) img, (Pattern2DMatchRecord_warp*) matchRecord);
			break;
			
		case kRGBa32Raster:
			match_((RasterImage_RGBa* ) patt2D->getPattern(), patt2D->getBlob(), NULL, 
			       (RasterImage_RGBa* ) img, (Pattern2DMatchRecord_warp*) matchRecord);
			break;
			
		default:
			break;
	}

    warpMatchRecord->setWarpingGrid(wcf_->getWarpingGrid());

	//-----------------------------
	//	reverse warping & get rwContour
	//-----------------------------
	reverseWarp_(patt2D, matchPoint);
//    warpMatchRecord->setReverseWarpingGrid(gridRev_);
	warpMatchRecord->setReverseWarpContour(rwContour_);
}

Pattern2DMatchRecord* PatternMatcher2D_warp::match(const RasterImage* pat,
								  				   const ImagePoint* matchPoint,
								  				   const RasterImage* img)
{
	Pattern2DMatchRecord* matchRecord = newPattern2DMatchRecord_();
	
	match(pat, matchPoint, img, matchRecord);
	
	return matchRecord;
}

void PatternMatcher2D_warp::match(const RasterImage* pat,
								  const ImagePoint* matchPoint,
								  const RasterImage* img,
								  Pattern2DMatchRecord* matchRecord)
{
    FAIL_CONDITION( !isProperMatchRecordType_(matchRecord),
                    kInvalidParameterError,
                    "Pattern2DMatchRecord_warp required");

    setPattern(pat, NULL);
    setImage(img, matchPoint);

	switch (img->getBaseRasterType())
	{
		case kGrayRaster:
			match_(static_cast<const RasterImage_gray*>(pat), NULL, 
				   static_cast<const RasterImage_gray*>(img), 
			       (Pattern2DMatchRecord_warp*) matchRecord);
			break;
			
		case kRGBa32Raster:
			match_(static_cast<const RasterImage_RGBa*>(pat), NULL, 
				   static_cast<const RasterImage_RGBa*>(img), 
			       (Pattern2DMatchRecord_warp*) matchRecord);
			break;
			
		default:
			break;
	}
}



#if 0
#pragma mark -
#endif

//----------------------------------------------------------
//  Protected matching functions
//----------------------------------------------------------

void PatternMatcher2D_warp::match_(const RasterImage_RGBa* pat, 
                                   const ImageRect* searchRect,
                                   const RasterImage_RGBa* img,
                                   Pattern2DMatchRecord* matchRecord)
{
	//float score = wcf_->func(xyStart_);
    optN_->minimize(xyStart_, xyMin_, xyMax_, OPT_TOL, optRecND_);
    
	//-------------------------
    //	get matchRecord
	//-------------------------
    Pattern2DMatchRecord_warp *warpMatchRecord = (Pattern2DMatchRecord_warp*) matchRecord;
    matchRecord->setScore(static_cast<float>(optRecND_->optVal));
    warpMatchRecord->optRecND_ = optRecND_;
}


void PatternMatcher2D_warp::match_(const RasterImage_gray* pat,
								   const ImageRect* searchRect,
								   const RasterImage_gray* img,
								   Pattern2DMatchRecord* matchRecord)
{
    optN_->minimize(xyStart_, xyMin_, xyMax_, OPT_TOL, optRecND_);
    
	//-------------------------
    //	get matchRecord
	//-------------------------
    Pattern2DMatchRecord_warp *warpMatchRecord = (Pattern2DMatchRecord_warp*) matchRecord;
    matchRecord->setScore(static_cast<float>(optRecND_->optVal));
    warpMatchRecord->optRecND_ = optRecND_;
}

void PatternMatcher2D_warp::match_(const RasterImage_RGBa* pat, const Blob* patBlob, 
                                   const ImageRect* searchRect,
                                   const RasterImage_RGBa* img,
                                   Pattern2DMatchRecord* matchRecord)
{
    optN_->minimize(xyStart_, xyMin_, xyMax_, OPT_TOL, optRecND_);
    
	//-------------------------
    //	get matchRecord
	//-------------------------
    Pattern2DMatchRecord_warp *warpMatchRecord = (Pattern2DMatchRecord_warp*) matchRecord;
    matchRecord->setScore(static_cast<float>(optRecND_->optVal));
    warpMatchRecord->optRecND_ = optRecND_;
}

void PatternMatcher2D_warp::match_(const RasterImage_gray* pat, const Blob* patBlob, 
                                   const ImageRect* searchRect,
                                   const RasterImage_gray* img,
                                   Pattern2DMatchRecord* matchRecord)
{
    optN_->minimize(xyStart_, xyMin_, xyMax_, OPT_TOL, optRecND_);
    
	//-------------------------
    //	get matchRecord
	//-------------------------
    Pattern2DMatchRecord_warp *warpMatchRecord = (Pattern2DMatchRecord_warp*) matchRecord;
    matchRecord->setScore(static_cast<float>(optRecND_->optVal));
    warpMatchRecord->optRecND_ = optRecND_;
}

#if 0
#pragma mark -
#endif

void PatternMatcher2D_warp::reverseWarp_(const Pattern2D* patt2D, const ImagePoint* matchPoint)
{
	double	*warpXY = optRecND_->optX;
	float	x1, x2, x3, x4, y1, y2, y3, y4,
			dx, dx1, dx2, dy, dy1, dy2,
			ratio1, ratio2,
			a, b, c,
			hRatio, vRatio,
			hSol1, hSol2, vSol1, vSol2,
			temp1, temp2, //temp3, // unused variable. -CJC
			xFactor1, xFactor2, xFactor3, xFactor4, xFactor5,
			yFactor1, yFactor2, yFactor3, yFactor4, yFactor5;;
	int		i, j, x, y, newX, newY;
			
	
	//	get gridRev_
	int k=0;
	for (i=0; i<gridRows_; i++)
	{
		for (j=0; j<gridCols_; j++)
		{

			//	simple version of reverse warping
			if ((i>0) && (i<gridRows_-1) && (j>0) && (j<gridCols_-1))
			{
				gridRev_[i][j][0] = static_cast<float>(warpXY[k++]);
				gridRev_[i][j][1] = static_cast<float>(warpXY[k++]);
			}
			else
			{
				gridRev_[i][j][0] = gridInit_[i][j][0];
				gridRev_[i][j][1] = gridInit_[i][j][1];
			}
		}
	}

	//	compute the contour's updated positions after reverse warping
	const Contour_I *ct = patt2D->getBlob()->getContour();
	if (rwContour_ != NULL)
		delete rwContour_;
	rwContour_ = new Contour_I();

	const list<ImagePoint*> *pointList = ct->getPoints();
	list<ImagePoint*>::const_iterator endIter = pointList->end(), ptListIter;
	list<ImagePoint* > *ptList = rwContour_->getPoints(R_W_ACCESS);

	//	iterate each point of contour
	for (list<ImagePoint*>::const_iterator iter=pointList->begin(); iter!=endIter; iter++)
	{
		//	translate x, y to start from the matchPoint in img
		x = (*iter)->getX() + matchPoint->getX();
		y = (*iter)->getY() + matchPoint->getY();

		bool found = false;
		for (i=0; i<gridRows_-1 && !found; i++)
		{
			int ip1 = i + 1;
			
			for (j=0; j<gridCols_-1 && !found; j++)
			{
				int jp1 = j + 1;
				
				x1 = gridRev_[i][j][0];
				x2 = gridRev_[i][jp1][0];
				x3 = gridRev_[ip1][j][0];
				x4 = gridRev_[ip1][jp1][0];
				y1 = gridRev_[i][j][1];
				y2 = gridRev_[i][jp1][1];
				y3 = gridRev_[ip1][j][1];
				y4 = gridRev_[ip1][jp1][1];
				
				//	compute whether the point overlap with one of the four vertices
				if (x==x1 && y==y1)
				{
					newX = int(gridInit_[i][j][0]);
					newY = int(gridInit_[i][j][1]);
					
					ptList->push_back(new ImagePoint(newX, newY));
					found = true;
				}
				else if (x==x2 && y==y2)
				{
					newX = int(gridInit_[i][jp1][0]);
					newY = int(gridInit_[i][jp1][1]);
					
					ptList->push_back(new ImagePoint(newX, newY));
					found = true;
				}
				else if (x==x3 && y==y3)
				{
					newX = int(gridInit_[ip1][j][0]);
					newY = int(gridInit_[ip1][j][1]);
					
					ptList->push_back(new ImagePoint(newX, newY));
					found = true;
				}
				else if (x==x4 && y==y4)
				{
					newX = int(gridInit_[ip1][jp1][0]);
					newY = int(gridInit_[ip1][jp1][1]);
					
					ptList->push_back(new ImagePoint(newX, newY));
					found = true;
				}
				//	compute whether inside either of the two triangles of this square
				else
				{
					bool inside = false;
					
					//	try first triangle
					dx = x - x3;
					dx1 = x1 - x3;
					dx2 = x4 - x3;
					dy = y - y3;
					dy1 = y1 - y3;
					dy2 = y4 - y3;
					ratio1 = dx*dy1 - dy*dx1;
					ratio2 = dx*dy2 - dy*dx2;
					//	if on one of the 2 borders of the first triangle
					if (ratio1 == 0)
					{
						vRatio = dy / dy1;
						newY = int(vRatio * gridInit_[i][j][1] + (1 - vRatio) * gridInit_[ip1][j][1]);
						newX = int(gridInit_[i][j][0]);
					
						ptList->push_back(new ImagePoint(newX, newY));
						found = true;
					}
					//	if on the other of the 2 borders of the first triangle
					else if (ratio2 == 0)
					{
						hRatio = dx / dx2;
						newX = int(hRatio * gridInit_[ip1][jp1][0] + (1 - hRatio) * gridInit_[ip1][j][0]);
						newY = int(gridInit_[ip1][j][1]);

						ptList->push_back(new ImagePoint(newX, newY));
						found = true;
					}
					//	on neither border of the first triangle
					else
					{
						ratio1 /= (dx2*dy1 - dy2*dx1);
						ratio2 /= -(dx2*dy1 - dy2*dx1);
						if (ratio1 > 0 && ratio2 > 0 && (ratio1+ratio2) <= 1)
							inside = true;
						//	if not inside the first triangle, then try the second triangle
						if (!inside)
						{
							dx = x - x2;
							dx2 = x1 - x2;
							dx1 = x4 - x2;
							dy = y - y2;
							dy2 = y1 - y2;
							dy1 = y4 - y2;
							ratio1 = dx*dy1 - dy*dx1;
							ratio2 = dx*dy2 - dy*dx2;
							//	if on one of the 2 borders of the second triangle
							if (ratio1 == 0)
							{
								vRatio = dy / dy1;
								newY = int(vRatio * gridInit_[ip1][jp1][1] + (1 - vRatio) * gridInit_[i][jp1][1]);
								newX = int(gridInit_[ip1][jp1][0]);

								ptList->push_back(new ImagePoint(newX, newY));
								found = true;
							}
							//	if on the other of the 2 borders of the second triangle
							else if (ratio2 == 0)
							{
								hRatio = dx / dx2;
								newX = int(hRatio * gridInit_[i][j][0] + (1 - hRatio) * gridInit_[i][jp1][0]);
								newY = int(gridInit_[i][jp1][1]);

								ptList->push_back(new ImagePoint(newX, newY));
								found = true;
							}
							//	on neither border of the second triangle
							else
							{
								ratio1 /= (dx2*dy1 - dy2*dx1);
								ratio2 /= -(dx2*dy1 - dy2*dx1);
								if (ratio1 > 0 && ratio2 > 0 && (ratio1+ratio2) <= 1)
									inside = true;
							}
						}								
						
						//	found the right square, compute the point's new coordinate
						if (inside)
						{
							xFactor1 = x1 - x;
							xFactor2 = x1 - x2;
							xFactor3 = x1 - x3;
							xFactor4 = x3 - x4;
							xFactor5 = x3 - x;

							//	compute hRatio
							a = xFactor4 * (y1 - y2) - xFactor2 * (y3 - y4);
							b = (xFactor4 - xFactor2) * y - (xFactor5 + xFactor4) * y1
								+ xFactor5 * y2 + (xFactor1 + xFactor2) * y3 - xFactor1 * y4;
							c = xFactor3 * y + xFactor5 * y1 - xFactor1 * y3;

							if (a==0)
								hRatio = -c / b;
							else
							{
								temp1 = 2.f*a;
								temp2 = sqrtf(b*b - 4.f*a*c + 1.f);

								hSol1 = (-b + temp2) / temp1;
								hSol2 = (-b - temp2) / temp1;
								hRatio = (hSol1 >= 0 && hSol1 <= 1) ? hSol1 : hSol2;
							}
							//newX = round((1-hRatio)*gridInit_[i][j][0] + hRatio*gridInit_[i][jp1][0]);
							newX = int((1-hRatio)*gridInit_[i][j][0] + hRatio*gridInit_[i][jp1][0] + 0.5f);
							
							yFactor1 = y1 - y;
							yFactor2 = y1 - y3;
							yFactor3 = y1 - y2;
							yFactor4 = y2 - y4;
							yFactor5 = y2 - y;

							//	compute hRatio
							a = yFactor4 * (x1 - x3) - yFactor2 * (x2 - x4);
							b = (yFactor4 - yFactor2) * x - (yFactor5 + yFactor4) * x1
								+ yFactor5 * x3 + (yFactor1 + yFactor2) * x2 - yFactor1 * x4;
							c = yFactor3 * x + yFactor5 * x1 - yFactor1 * x2;

							if (a==0)
								vRatio = -c / b;
							else
							{
								temp1 = 2.f*a;
								temp2 = sqrtf(b*b - 4.f*a*c + 1.f);

								vSol1 = (-b + temp2) / temp1;
								vSol2 = (-b - temp2) / temp1;
								vRatio = (vSol1 >= 0 && vSol1 <= 1) ? vSol1 : vSol2;
							}
							//newX = round((1-hRatio)*gridInit_[i][j][0] + hRatio*gridInit_[i][jp1][0]);
							newY = int((1-vRatio)*gridInit_[i][j][1] + vRatio*gridInit_[ip1][j][1] + 0.5f);
/*
							//	compute vRatio
							temp3 = -xFactor3 + (xFactor2 - xFactor4) * hRatio;
							#if DEBUG
							if (temp1 == 0)
								int dbg1 = temp1;
							if (temp3 == 0)
								int dbg3 = temp3;
							#endif
							vRatio = (-xFactor1 + xFactor2 * hRatio) / temp3;
							if (vRatio > 1)
								vRatio = 1;
							if (vRatio < 0)
								vRatio = 0;
							//newY = round((1-vRatio)*gridInit_[i][j][1] + vRatio*gridInit_[ip1][j][1]);
							newY = int((1-vRatio)*gridInit_[i][j][1] + vRatio*gridInit_[ip1][j][1]);
*/
							
							//	get reverse warped contour
							bool samePoint = false;
							if (!ptList->empty())
							{
								ptListIter = ptList->end();
								ptListIter--;
								if ((*ptListIter)->getX() == newX && (*ptListIter)->getY() == newY)
									samePoint = true;
							}
							if (!samePoint)
								ptList->push_back(new ImagePoint(newX, newY));
							
							found = true;
						}
					}
				}
			}
		}
	}
}
#if 0
#pragma mark -
#endif

bool PatternMatcher2D_warp::isProperMatchRecordType_(const Pattern2DMatchRecord* matchRecord) const
{
//    return (typeid(*matchRecord) == typeid(uriVL::Pattern2DMatchRecord_warp));

    string s1 = matchRecord->getRecordType();
    return (s1.compare("warp match record") == 0);
}


Pattern2DMatchRecord* PatternMatcher2D_warp::newPattern2DMatchRecord_(void)
{
    return new Pattern2DMatchRecord_warp();
}

void PatternMatcher2D_warp::deleteGrids_(void)
{
	if (gridInit_ != NULL)
	{
	    for (int i=0; i<gridRows_; i++)
	        delete []gridInit_[i];
	    delete []gridInit_;
	    gridInit_ = NULL;
	}

	if (gridRev_ != NULL)
	{
	    for (int i=0; i<gridRows_; i++)
	        delete []gridRev_[i];
	    delete []gridRev_;
	    gridRev_ = NULL;
	}
	
	if (xyStart_ != NULL)
	{
	    delete []xyStart_;
	    xyStart_ = NULL;
	}

	if (xyMin_ != NULL)
	{
	    delete []xyMin_;
	    xyMin_ = NULL;
	}

	if (xyMax_ != NULL)
	{
	    delete []xyMax_;
	    xyMax_ = NULL;
	}
}

void PatternMatcher2D_warp::computeGridBounds_(void)
{
//     	ImageRect* rect = img_->getValidRect();  // unused variable. -CJC  
//     	int	imgHeight = rect->getHeight(),  // unused variable. -CJC  
//     		imgWidth = rect->getWidth(),       // unused variable. -CJC
    	int t = 0;
    		
//--------------------------------------------------------------------------------
//  all this is only valid when the warping modes are INNER_BOUNDS & BORDER_FIXED
//--------------------------------------------------------------------------------
		//	initialize the grid
		if (gridInit_ == NULL)
		    gridInit_ = wcf_->newWarpingGrid();
		if (gridRev_ == NULL)
		    gridRev_ = wcf_->newWarpingGrid();

		t = 0;
		for (int i=1; i<gridRows_-1; i++)
			for (int j=1; j<gridCols_-1; j++)
			{
				xyStart_[t++] = gridInit_[i][j][0];
				xyStart_[t++] = gridInit_[i][j][1];
			}
		
		//	distance in x and y between neighbor grid points	
		int xDist = static_cast<int>(gridInit_[0][1][0] - gridInit_[0][0][0]),
			yDist = static_cast<int>(gridInit_[1][0][1] - gridInit_[0][0][1]);
			
/*
		int xRange = xDist / 2,
			yRange = yDist / 2;
*/
int xRange = 2*xDist/3,
	yRange = 2*yDist/3;
		int xRange23 = 2*xDist/3,
			yRange23 = 2*yDist/3;
			
		//	initialize xyMin_ and xyMax_
		t = 0;
		//	corner 0 -- 0 of the inner grid
		xyMin_[t] = xyStart_[t] - xRange23;
		xyMax_[t] = xyStart_[t] + xRange;
		t++;
		xyMin_[t] = xyStart_[t] - yRange23;
		xyMax_[t] = xyStart_[t] + yRange;
		t++;
		//  top row of the inner grid
		for (int j=2; j<gridCols_-2; j++)
		{
			xyMin_[t] = xyStart_[t] - xRange;
			xyMax_[t] = xyStart_[t] + xRange;
			t++;
			xyMin_[t] = xyStart_[t] - yRange23;
			xyMax_[t] = xyStart_[t] + yRange;
			t++;
		}
		//	corner 0 -- cols-1 of the inner grid
		xyMin_[t] = xyStart_[t] - xRange;
		xyMax_[t] = xyStart_[t] + xRange23;
		t++;
		xyMin_[t] = xyStart_[t] - yRange23;
		xyMax_[t] = xyStart_[t] + yRange;
		t++;
		
		for (int i=2; i<gridRows_-2; i++)
		{
			//	border i -- 0 of the inner grid
			xyMin_[t] = xyStart_[t] - xRange23;
			xyMax_[t] = xyStart_[t] + xRange;
			t++;
			xyMin_[t] = xyStart_[t] - yRange;
			xyMax_[t] = xyStart_[t] + yRange;
			t++;

			for (int j=2; j<gridCols_-2; j++)
			{
				xyMin_[t] = xyStart_[t] - xRange;
				xyMax_[t] = xyStart_[t] + xRange;
				t++;
				xyMin_[t] = xyStart_[t] - yRange;
				xyMax_[t] = xyStart_[t] + yRange;
				t++;
			}
			//	border i -- cols-1 of the inner grid
			xyMin_[t] = xyStart_[t] - xRange;
			xyMax_[t] = xyStart_[t] + xRange23;
			t++;
			xyMin_[t] = xyStart_[t] - yRange;
			xyMax_[t] = xyStart_[t] + yRange;
			t++;
		}

		//	corner rows-1 -- 0 of the inner grid
		xyMin_[t] = xyStart_[t] - xRange23;
		xyMax_[t] = xyStart_[t] + xRange;
		t++;
		xyMin_[t] = xyStart_[t] - yRange;
		xyMax_[t] = xyStart_[t] + yRange23;
		t++;
		for (int j=2; j<gridCols_-2; j++)
		{
			xyMin_[t] = xyStart_[t] - xRange;
			xyMax_[t] = xyStart_[t] + xRange;
			t++;
			xyMin_[t] = xyStart_[t] - yRange;
			xyMax_[t] = xyStart_[t] + yRange23;
			t++;
		}
		//	corner rows-1 -- cols-1
		xyMin_[t] = xyStart_[t] - xRange;
		xyMax_[t] = xyStart_[t] + xRange23;
		t++;
		xyMin_[t] = xyStart_[t] - yRange;
		xyMax_[t] = xyStart_[t] + yRange23;

}
