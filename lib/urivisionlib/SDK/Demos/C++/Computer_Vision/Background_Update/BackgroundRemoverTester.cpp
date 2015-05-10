/*  NAME:
        BackgroundRemoverTester.cpp
 
    DESCRIPTION:
        BackgroundRemoverTester source file

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "BackgroundRemoverTester.h"

using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructors
//----------------------------------------------------------
BackgroundRemoverTester::BackgroundRemoverTester(int nbRects, RasterImage *firstBackground)
{
	firstBackground_ = firstBackground;
	
	remover_ = new BackgroundRemover_dynamic_MW();
	nbRects_ = nbRects;
	rectStep_ = 1;
	top_ = 0;
	left_ = 0;
	height_ = 0;
	width_ = 0;
	dimX_ = 0;
	dimY_ = 0;
	diffImg_ = NULL;
	diffData_ = NULL;
	estimateImg_ = NULL;
	frameCount_ = 0;
	rectSize_ = 10;
	
	thresholder_	= new GrayThresholdLabeler();
	grayParams_		= new ColorSegParams;
	label_			= NULL;
	blobFinder_		= new BlobFinder();
	target_			= new char[1];
	blobMinSize_	= 5;

	minThreshold_ 	= 64;
	noiseSize_		= 30;

    grayParams_->nbModes = 1;
    grayParams_->colorMode = false;
    grayParams_->colorReg = NULL;
    grayParams_->grayReg = new GrayRegion[grayParams_->nbModes];
    grayParams_->fillMode = kOrigColorFill;
    grayParams_->grayReg[0].labelStr =  "experimental difference";
    grayParams_->grayReg[0].vMin = minThreshold_;
    grayParams_->grayReg[0].vMax = 255;
    grayParams_->grayReg[0].fillColor[0] = (unsigned char) 255;
    grayParams_->grayReg[0].fillColor[1] = 0;
    grayParams_->grayReg[0].fillColor[2] = (unsigned char) 0;
    grayParams_->grayReg[0].label = (char) 1;			//  must begin from n>0
    target_[0] = grayParams_->grayReg[0].label;

	blobList_	= new BlobList[1];
	
	testCase_ = THRESHOLD_NO_OBJECT;
	
	objectMatched_ = false;
	nbObjectMissed_ = 0;
	nbIncreaseRect_ = 0;
}

BackgroundRemoverTester::~BackgroundRemoverTester(void)
{
	delete remover_;
	delete estimateImg_;

	int i;
	for(i=0; i<dimX_; i++)
		delete []diffData_[i];
	delete []diffData_;

	delete diffImg_;
	rectList_.clear();
	
	delete label_;
		
	delete []grayParams_->grayReg;
	delete grayParams_;
	delete thresholder_;
	delete blobFinder_;
	delete []target_;
	blobList_[0].clear();
	delete []blobList_;
}



//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

int **BackgroundRemoverTester::getDiffData(void)
{
	return diffData_;
}

int ***BackgroundRemoverTester::getDiffData3D(void)
{
	return diffData3D_;
}

RasterImage_gray *BackgroundRemoverTester::getDiffImg(void)
{
	return diffImg_;
}

double BackgroundRemoverTester::getTotalIntensity(void)
{
	return totalIntensity_;
}

float BackgroundRemoverTester::getCoveredPercent(void)
{
	return coveredPercent_;
}

RasterImage *BackgroundRemoverTester::getFirstBackground(void)
{
	return firstBackground_;
}
RasterImage *BackgroundRemoverTester::getBackground(void)
{
	return estimateImg_;
}

int BackgroundRemoverTester::getRightThreshold(void)
{
    FAIL_CONDITION( diffImg_ == NULL,
                    kImageProcessingError,
                    "Should firstly call test before BackgroundRemoverTester::getRightThreshold");

	for(minThreshold_=1; minThreshold_<255; minThreshold_++)
	{
		if(label_ == NULL)
			label_	= new LabelRaster(diffImg_->getValidRect());
	
		grayParams_->grayReg[0].vMin = minThreshold_;
		thresholder_->applyInto(diffImg_, grayParams_, label_);
		
		blobFinder_->findAllBlobs(label_, grayParams_->nbModes,
									target_, blobMinSize_, blobList_);
									
		if(blobList_[0].size() == 0)		// already get the right minThreshold_
			break;
	}
	
	return minThreshold_;
}

int BackgroundRemoverTester::getDimX(void)
{
	return dimX_;
}

int BackgroundRemoverTester::getDimY(void)
{
	return dimY_;
}

int BackgroundRemoverTester::getXUnit(void)
{
	return (remover_->xUnit_-1);
}

int BackgroundRemoverTester::getYUnit(void)
{
	return (remover_->yUnit_-1);
}

float **BackgroundRemoverTester::getRatio(void)
{
	return intensityRatio_;
}

RasterImage_gray_F *BackgroundRemoverTester::getRatioAsRasterImage(void)
{
	return grayRatio_;
}

RasterImage_gray_F *BackgroundRemoverTester::getFittedRatioAsRasterImage(void)
{
	return fittedGrayRatio_;
}

float *BackgroundRemoverTester::getCoveredPercNoSmooth(void)
{
	return coveredPercNoSmooth_;
}

int BackgroundRemoverTester::getNbIncreaseRect(void)
{
	return nbIncreaseRect_;
}

void BackgroundRemoverTester::test(RasterImage *img, BkgdTestCase testCase_,
									int rectStep // = 1
									)
{
	rectStep_ = rectStep;
	
	switch (testCase_)
	{
		case INTENSITY_RATIO:
			test0_(img);
			break;
	
		case THRESHOLD_NO_OBJECT:
			test1_(img);
			break;
			
		case PERCENTAGE_NO_SMOOTHING:
			test21_(img);
			break;
		
		case PERCENTAGE_NO_OBJECT:
			test22_(img);
			break;
			
		case ALL_OBJECT:
			test3_(img);
			break;
			
		case MISS_SOME_OBJECT:
			test4_(img);
			break;
			
		default:	// this should not happen
			break;
	}
}

//----------------------------------------------------------
//  protected & private functions
//----------------------------------------------------------

void BackgroundRemoverTester::test0_(RasterImage *img)
{
	int i,j;
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's rt
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's bt
	double	a0	= (remover_->a0_[0] + remover_->a0_[1] + remover_->a0_[2]) / 3,
			a1	= (remover_->a1_[0] + remover_->a1_[1] + remover_->a1_[2]) / 3,
			a2	= (remover_->a2_[0] + remover_->a2_[1] + remover_->a2_[2]) / 3,
			a3	= (remover_->a3_[0] + remover_->a3_[1] + remover_->a3_[2]) / 3,
			a4	= (remover_->a4_[0] + remover_->a4_[1] + remover_->a4_[2]) / 3,
			a5	= (remover_->a5_[0] + remover_->a5_[1] + remover_->a5_[2]) / 3,
			a6	= (remover_->a6_[0] + remover_->a6_[1] + remover_->a6_[2]) / 3,
			a7	= (remover_->a7_[0] + remover_->a7_[1] + remover_->a7_[2]) / 3,
			a8	= (remover_->a8_[0] + remover_->a8_[1] + remover_->a8_[2]) / 3,
			a9	= (remover_->a9_[0] + remover_->a9_[1] + remover_->a9_[2]) / 3;
	
	//	set first video-image as first background
	//	memory allocation & initialization only once
	if(frameCount_ == 0)
	{
		//	remover_->xUnit_, yUnit_ are both set in setBackground method
		remover_->setBackground(firstBackground_);
		
		intensityBg_		= new float*[remover_->yUnit_-1];// array of firstBG squares' internal intensity
		intensityImg_		= new float*[remover_->yUnit_-1];// array of currImg squares' internal intensity
		intensityRatio_		= new float*[remover_->yUnit_-1];// array of ratio value
		grayRatio_			= new RasterImage_gray_F(firstBackground_->getBoundRect());
		grayRatio_->setFloatConversionMode(kRangeFloat);
		fittedGrayRatio_ = new RasterImage_gray_F(firstBackground_->getBoundRect());
		fittedGrayRatio_->setFloatConversionMode(kRangeFloat);
		
		for(i=0; i<(remover_->yUnit_-1); i++)
		{
			intensityBg_[i]		= new float[remover_->xUnit_-1];
			intensityImg_[i]	= new float[remover_->xUnit_-1];
			//fittedIntensityImg_[i]	= new float[remover_->xUnit_-1];
			intensityRatio_[i]	= new float[remover_->xUnit_-1];
			//fittedIntensityRatio_[i] = new float[remover_->xUnit_-1];
		}
		
		firstBg2D_ = (remover_->firstBackground_)->getShiftedRaster2D();

		//	1st frame, compute squares' intensity in static first background
		//	outer loop, for all squares inside image except for those having border outside
		//  (that's why unit-1)
		for(i=0; i<(remover_->yUnit_-1); i++)
		{
			ySqTp = i*(remover_->size_);
			ySqBt = ySqTp + (remover_->size_)-1;
		
			for(j=0; j<remover_->xUnit_-1; j++)
			{
				intensityBg_[i][j] = 0;		
				xSqLf = j*(remover_->size_);
				xSqRt = xSqLf + (remover_->size_)-1;

			    int p, q;
			    //	inner loop within each square
				for(p=ySqTp; p<=ySqBt; p++)
				{
					long l = 4*xSqLf;
					for(q=xSqLf; q<=xSqRt; q++)
					{
						float temp = 0.f;
						// AVERAGE INTENSITY
						int r;
						for(r=0; r<3; r++, l++){
							temp += firstBg2D_[p][l];
						temp /= 3;
						}
						l++;
						
						// MAX INTENSITY
						//int r;
						//for(r=0; r<3; r++, l++)
						//{
						//	temp = max(temp, (float)(firstBg2D_[p][l]));		
						//}
						//l++;
									
						intensityBg_[i][j]	+= temp;		
					}
				}
			}
		}
	}
	
	//	2nd frame on, compute squares' intensity in current image
	else{
		unsigned char **currImg2D = img->getShiftedRaster2D();
		float	**ratio = grayRatio_->getShiftedGrayF2D(),
				**fittedRatio = fittedGrayRatio_->getShiftedGrayF2D();
		
		//	outer loop, for all squares inside image except for those having border outside(that's why size-1)
		for(i=0; i<(remover_->yUnit_-1); i++)
		{
			ySqTp = i*(remover_->size_);
			ySqBt = ySqTp + (remover_->size_)-1;
		
			for(j=0; j<remover_->xUnit_-1; j++)
			{
				intensityImg_[i][j]	= 0;		
				xSqLf = j*(remover_->size_);
				xSqRt = xSqLf + (remover_->size_)-1;

			    int p, q;
			    //	inner loop within each square
			    //	compute intensityImg_ & fittedIntensityImg_
				for(p=ySqTp; p<=ySqBt; p++)
				{
					long l = 4*xSqLf;
					for(q=xSqLf; q<=xSqRt; q++)
					{
						float	temp	= 0.f;
														
						// AVERAGE INTENSITY
						int r;
						for(r=0; r<3; r++, l++){
							temp += currImg2D[p][l];
						temp /= 3;
						l++;
						}
						
						// MAX INTENSITY
						//int r;
						//for(r=0; r<3; r++, l++)
						//{
						//	temp = max(temp, (float)(currImg2D[p][l]));							
						//}
						//l++;

                        fittedRatio[p][q] = (a0*p*p*p
											+ a1*q*q*q
											+ a2*p*p*q
											+ a3*p*q*q
											+ a4*p*p
											+ a5*q*q
											+ a6*p*q
											+ a7*p
											+ a8*q
											+ a9);
									
						intensityImg_[i][j]	+= temp;
						
					}
				}
				
				//	get ratio of each square
				//cout << "intensityImg_[i][j]: " << intensityImg_[i][j] << endl;
				//cout << "intensityBg_[i][j]: " << intensityBg_[i][j] << endl;
				intensityRatio_[i][j] = intensityImg_[i][j] / intensityBg_[i][j];
				
				for(p=ySqTp; p<=ySqBt; p++)
					for(q=xSqLf; q<=xSqRt; q++)
					{
					    ratio[p][q] = intensityRatio_[i][j];
					}
			}
		}
		
		//	produce grayRatio_ as a gray_F
		grayRatio_->setValidRect(0, 0, (remover_->xUnit_-1)*remover_->size_,
		                                (remover_->yUnit_-1)*remover_->size_);
		grayRatio_->requestRasterUpdate();
		
		//	produce fittedRatio_ as a gray_F
		fittedGrayRatio_->setValidRect(0, 0, (remover_->xUnit_-1)*remover_->size_,
		                                (remover_->yUnit_-1)*remover_->size_);
		fittedGrayRatio_->requestRasterUpdate();
	}
	
	//	update background
	rectList_.push_back(NULL);
	remover_->updateBackground(img, rectList_);		
	rectList_.clear();	

	frameCount_++;
}

void BackgroundRemoverTester::test1_(RasterImage *img)
{
	int i, j, rt, bt;
	
	//--------------------------------------------
	//	set first video-image as first background
	//	memory allocation & initialization only once
	//--------------------------------------------
	if(frameCount_ == 0)
	{
		remover_->setBackground(firstBackground_);
		dimX_ = img->getWidth();
		dimY_ = img->getHeight();
		diffImg_ = new RasterImage_gray(firstBackground_->getBoundRect());
		label_	= new LabelRaster(firstBackground_->getBoundRect());
		
		rt = img->getBoundRect()->getRight();
		bt = img->getBoundRect()->getBottom();
		
	}
	
	//--------------------------------------------
	//	make estimation, compare, and then update
	//--------------------------------------------
	//	get the comparison result from 2nd frame on
	if(frameCount_ > 0)
	{
		estimateImg_ = remover_->getBackground();
		remover_->difference(estimateImg_, img, diffImg_);	

		// get the right threshold such that no blobs can be found
		for(minThreshold_=1; minThreshold_<255; minThreshold_++)
		{			
		
			grayParams_->grayReg[0].vMin = minThreshold_;
			thresholder_->applyInto(diffImg_, grayParams_, label_);
			
			blobFinder_->findAllBlobs(label_, grayParams_->nbModes,
										target_, blobMinSize_, blobList_);
										
			// already get the right minThreshold_, break the loop
			// case 1, no blobs found
			if(blobList_[0].size() == 0)
				break;
			// case 2, blobs are all considered noise
			else
			{
				bool needBreak = true;
				BlobList::iterator iter = blobList_[0].begin();
				// if all blobs are noise, we break the loop
				// of finding minThreshold_
				for( ; iter!=blobList_[0].end(); iter++)
				{
					if((*iter)->getNbPixels() >= noiseSize_)
					{
						needBreak = false;
						break;	
					}
				}
				if(needBreak)
					break;
			}
		}
	}
	
	int xEntry, yEntry;
	remover_->initializedByTester_ = true;
	for(i=0; i<remover_->yUnit_ ;i++)
	    for(j=0; j<remover_->xUnit_; j++)
	        remover_->goodSquare_[i][j] = true;	// initialize goodSquare_
	
	// random squares moving at the speed of 1 squareSize/frame
	for(i=0; i<nbRects_; i++)
	{
		srand(i);
		
		// in case of overlap, use a loop to produce a new square
		do
		{
			//yEntry = rand() % remover_->yUnit_;
			//xEntry = rand() % remover_->xUnit_;
			yEntry = (rand()+frameCount_) % remover_->yUnit_;
			xEntry = (rand()+frameCount_) % remover_->xUnit_;
		} while (remover_->goodSquare_[yEntry][xEntry]==false);
		
		//	set the square to be invalid
		remover_->goodSquare_[yEntry][xEntry] = false;
		//cout << "yEntry:" << yEntry << endl;
	}

///*	
	// a bad region of nbRects_*nbRects_ in the image
	// moving at the speed of 1 squareSize/frame
	yEntry = 6;
	xEntry = 6;
	int xTemp, yTemp;
	//	set the square to be invalid
	remover_->goodSquare_[yEntry][xEntry] = false;
	for(i=0; i<nbRects_; i++)
	{
		yEntry++;
		xEntry = 6;
		for(j=0; j<nbRects_; j++)
		{
			xEntry++;
			xTemp  = xEntry%remover_->xUnit_;
			yTemp  = yEntry%remover_->yUnit_;
			//xTemp  = (xEntry+frameCount_)%remover_->xUnit_;
			//yTemp  = (yEntry+frameCount_)%remover_->yUnit_;
			remover_->goodSquare_[yTemp][xTemp] = false;
		}
	}
//*/	
	
	//	update background
	if(rectList_.empty())
		rectList_.push_back(NULL);
	remover_->updateBackground(img, rectList_);		
	rectList_.clear();	
	
	frameCount_++;
}

void BackgroundRemoverTester::test21_(RasterImage *img)
{
	int i, j, index=0, rt, bt;
	remover_->noSmoothing_ = true;
	
	//--------------------------------------------
	//	set first video-image as first background
	//	memory allocation & initialization only once
	//--------------------------------------------
	if(frameCount_ == 0)
	{
		remover_->setBackground(firstBackground_);
		dimX_ = img->getWidth();
		dimY_ = img->getHeight();
		totalRects_ = remover_->yUnit_*remover_->xUnit_;

		rt = img->getBoundRect()->getRight();
		bt = img->getBoundRect()->getBottom();
		
		diffImg_ = new RasterImage_gray(firstBackground_->getBoundRect());
		
		nbIncreaseRect_ = (totalRects_-10)/rectStep_;
		coveredPercNoSmooth_ = new float[nbIncreaseRect_];

		diffData3D_ = new int**[nbIncreaseRect_];
		for (index=0; index<nbIncreaseRect_; index++)
		{
		
			diffData3D_[index] = new int*[dimY_];
			for(i=0; i<dimY_; i++)
			{
				diffData3D_[index][i] = new int[dimX_];
			}
		}
			
		for(i=0; i<nbIncreaseRect_; i++)
		for(nbRects_=rectStep_,i=0; i<nbIncreaseRect_; i++,nbRects_ += rectStep_)
			coveredPercNoSmooth_[i] = ((float)nbRects_)/ totalRects_;
	}
	
	//--------------------------------------------
	//	make estimation, compare, and then update
	//--------------------------------------------	
	
	//	get the comparison result from 2nd frame on
	if(frameCount_ > 0)
	{
		//	add all rectangles, using goodSquare_ in remover_
		int xEntry, yEntry;
		remover_->initializedByTester_ = true;
	
		//	compute totalIntensity_
		totalIntensity_ = 0.L;
		unsigned char **currImg = img->getShiftedRaster2D();
		for(i=0; i<dimY_; i++)
			for(j=0; j<dimX_; j++)
			{
				totalIntensity_ += currImg[i][j];
			}

		//	each frame, keep increasing nbRects till >=10 good square left
		for(nbRects_=rectStep_;nbRects_<totalRects_-10;nbRects_ += rectStep_)
		{
			estimateImg_ = remover_->getBackground();
			//delete diffImg_;
			//diffImg_ = new RasterImage_gray(estimateImg_->getBoundRect());
			remover_->difference(estimateImg_, img, diffImg_);
			
			unsigned char **temp;
			temp = diffImg_->getShiftedGray2D();
			for(i=0; i<dimY_; i++)
				for(j=0; j<dimX_; j++)
				{
					diffData3D_[index][i][j] = temp[i][j];
				}

			for(i=0; i<remover_->yUnit_ ;i++)
			    for(j=0; j<remover_->xUnit_; j++)
			        remover_->goodSquare_[i][j] = true;	// initialize goodSquare_
			// random squares moving at the speed of 1 squareSize/frame
			for(i=0; i<nbRects_; i++)
			{
				srand(i);
				
				// in case of overlap, use a loop to produce a new square
				do
				{
					//yEntry = rand() % remover_->yUnit_;
					//xEntry = rand() % remover_->xUnit_;
					yEntry = (rand()+frameCount_) % remover_->yUnit_;
					xEntry = (rand()+frameCount_) % remover_->xUnit_;
				} while (remover_->goodSquare_[yEntry][xEntry]==false);
				
				//	set the square to be invalid
				remover_->goodSquare_[yEntry][xEntry] = false;
				//cout << "yEntry:" << yEntry << endl;
			}
				
			//	update background
			if(rectList_.empty())
				rectList_.push_back(NULL);
			remover_->updateBackground(img, rectList_);		
			rectList_.clear();	
			
			index++;
		}
	}
	
	frameCount_++;
}

void BackgroundRemoverTester::test22_(RasterImage *img)
{
	int i, j, rt, bt;

	//--------------------------------------------
	//	set first video-image as first background
	//	memory allocation & initialization only once
	//--------------------------------------------
	if(frameCount_ == 0)
	{
		remover_->setBackground(firstBackground_);
		dimX_ = img->getWidth();
		dimY_ = img->getHeight();
		totalRects_ = remover_->yUnit_*remover_->xUnit_;

		rt = img->getBoundRect()->getRight();
		bt = img->getBoundRect()->getBottom();
		
		diffData_ = new int*[dimY_];
		for(i=0; i<dimX_; i++)
			diffData_[i] = new int[dimX_];
		//cout << "dimX: " << dimX_ << endl;
		//cout << "dimY: " << dimY_ << endl;
		diffImg_ = new RasterImage_gray(firstBackground_->getBoundRect());
	}
	
	//--------------------------------------------
	//	make estimation, compare, and then update
	//--------------------------------------------	
	//	get the comparison result from 2nd frame on
	if(frameCount_ > 0)
	{
		estimateImg_ = remover_->getBackground();
		//delete diffImg_;
		//diffImg_ = new RasterImage_gray(estimateImg_->getBoundRect());
		remover_->difference(estimateImg_, img, diffImg_);
		
		unsigned char **temp;
		temp = diffImg_->getShiftedGray2D();
		for(i=0; i<dimY_; i++)
			for(j=0; j<dimX_; j++)
			{
				diffData_[i][j] = temp[i][j];
				//cout << "diffData[" << i << "][" << j << "]:"
				//	<< diffData_[i][j] << endl;
			}

		//	add all rectangles, using goodSquare_ in remover_
		int xEntry, yEntry;

		remover_->initializedByTester_ = true;

		for(i=0; i<remover_->yUnit_ ;i++)
		    for(j=0; j<remover_->xUnit_; j++)
		        remover_->goodSquare_[i][j] = true;	// initialize goodSquare_
		// random squares moving at the speed of 1 squareSize/frame
		for(i=0; i<nbRects_; i++)
		{
			srand(i);
			
			// in case of overlap, use a loop to produce a new square
			do
			{
				//yEntry = rand() % remover_->yUnit_;
				//xEntry = rand() % remover_->xUnit_;
				yEntry = (rand()+frameCount_) % remover_->yUnit_;
				xEntry = (rand()+frameCount_) % remover_->xUnit_;
			} while (remover_->goodSquare_[yEntry][xEntry]==false);
			
			//	set the square to be invalid
			remover_->goodSquare_[yEntry][xEntry] = false;
			//cout << "yEntry:" << yEntry << endl;
		}

		coveredPercent_ = ((float)nbRects_) / totalRects_;
		cout << "coveredPercent_: " << coveredPercent_ << endl;
		cout << "nbRects_: " << nbRects_ << endl;
		//	because we need at least 10 equations for LSE, so at least
		//	10 rects should be reserved as good square in remover_
		if(nbRects_ < (totalRects_-23-rectStep_))
			nbRects_ += rectStep_;
	}
	
	
	//	update background
	if(rectList_.empty())
		rectList_.push_back(NULL);
	remover_->updateBackground(img, rectList_);		
	rectList_.clear();	
	
	//	compute totalIntensity_
	totalIntensity_ = 0.L;
	unsigned char **currImg = img->getShiftedRaster2D();
	for(i=0; i<dimY_; i++)
		for(j=0; j<dimX_; j++)
		{
			totalIntensity_ += currImg[i][j];
		}
	
	frameCount_++;
}


void BackgroundRemoverTester::test3_(RasterImage *img)
{
	int /* i,  unused var. -CJC */ rt, bt;
	
	//--------------------------------------------
	//	set pointed image as first background (only once)
	//--------------------------------------------
	if(frameCount_ == 0)
	{
		remover_->setBackground(firstBackground_);
		dimX_ = img->getWidth();
		dimY_ = img->getHeight();
		diffImg_ = new RasterImage_gray(firstBackground_->getBoundRect());
		
		rt = img->getBoundRect()->getRight();
		bt = img->getBoundRect()->getBottom();
	}
	
	//--------------------------------------------
	//	make estimation, compare, and then update
	//--------------------------------------------
	//	get the comparison result from 1st frame on
	estimateImg_ = remover_->getBackground();
	//diffImg_ = new RasterImage_gray(estimateImg_->getBoundRect());
	remover_->difference(estimateImg_, img, diffImg_);		

	// test whether detected blobs found are same with real ones in image
	if(label_ == NULL)
		label_	= new LabelRaster(diffImg_->getValidRect());
	grayParams_->grayReg[0].vMin = minThreshold_;
	thresholder_->applyInto(diffImg_, grayParams_, label_);
	blobFinder_->findAllBlobs(label_, grayParams_->nbModes,
									target_, blobMinSize_, blobList_);
	
	// here we only check number, we may also test other state of the blobs						
	objectDetectList_.clear();
	if(static_cast<int>(blobList_[0].size()) == nbRects_)
		objectMatched_ = true;
	objectDetectList_.push_back(objectMatched_);
	objectMatched_ = false;
	if (objectMatched_)
		cout << "match? yes" << endl;
	else
		cout << "match? no" << endl;
	
	//	get all rectangles
	BlobList::iterator iter = blobList_[0].begin();
	for( ; iter!=blobList_[0].end(); iter++)
	{
		rectList_.push_back((*iter)->getBoundRect());
	}
	
	//	update background
	if(rectList_.empty())
		rectList_.push_back(NULL);
	remover_->updateBackground(img, rectList_);		
	rectList_.clear();	
	
	frameCount_++;
}


void BackgroundRemoverTester::test4_(RasterImage *img)
{
	int i, rt, bt;

	//--------------------------------------------
	//	set pointed image as first background (only once)
	//--------------------------------------------
	if(frameCount_ == 0)
	{
		remover_->setBackground(firstBackground_);
		dimX_ = img->getWidth();
		dimY_ = img->getHeight();
		diffImg_ = new RasterImage_gray(firstBackground_->getBoundRect());
		
		rt = img->getBoundRect()->getRight();
		bt = img->getBoundRect()->getBottom();
		
	}
	
	//--------------------------------------------
	//	make estimation, compare, and then update
	//--------------------------------------------
	//	get the comparison result from 1st frame on
	estimateImg_ = remover_->getBackground();
	//diffImg_ = new RasterImage_gray(estimateImg_->getBoundRect());
	remover_->difference(estimateImg_, img, diffImg_);		

	// test whether detected blobs found are same with real ones in image
	if(label_ == NULL)
		label_	= new LabelRaster(diffImg_->getValidRect());
	grayParams_->grayReg[0].vMin = minThreshold_;
	thresholder_->applyInto(diffImg_, grayParams_, label_);
	blobFinder_->findAllBlobs(label_, grayParams_->nbModes,
									target_, blobMinSize_, blobList_);
	
	// here we only check number, we may also test other state of the blobs						
	if(static_cast<int>(blobList_[0].size()) == nbRects_)
		objectMatched_ = true;
	objectDetectList_.push_back(objectMatched_);
	objectMatched_ = false;
	if (objectMatched_)
		cout << "match? yes" << endl;
	else
		cout << "match? no" << endl;
	
	//	get all rectangles, and put some of them into rectList_ for later update
	BlobList::iterator iter = blobList_[0].begin();
	for( ; iter!=blobList_[0].end(); iter++)
	{
		rectList_.push_back((*iter)->getBoundRect());
	}
	for(i=0; i<nbObjectMissed_; i++)
		rectList_.pop_back();
	
	//	update background
	if(rectList_.empty())
		rectList_.push_back(NULL);
	remover_->updateBackground(img, rectList_);		
	rectList_.clear();	
	
	frameCount_++;
}

