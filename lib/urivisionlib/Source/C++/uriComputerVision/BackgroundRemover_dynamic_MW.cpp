/*  NAME:
        BackgroundRemover_dynamic_MW.cpp
 
    DESCRIPTION:
        BackgroundRemover_dynamic_MW source file

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <fstream>
#include <cmath>
//
#include "BackgroundRemover_dynamic_MW.h"

using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


BackgroundRemover_dynamic_MW::BackgroundRemover_dynamic_MW(void)
    
        :   BackgroundRemover_dynamic(),
            //
            size_(10),
            xUnit_(0),
            yUnit_(0),
            initializedByTester_(false),
            noSmoothing_(false),
            //
            a_(new double*[10]),
            a0_(NULL),
            a1_(NULL),
            a2_(NULL),
            a3_(NULL),
            a4_(NULL),
            a5_(NULL),
            a6_(NULL),
            a7_(NULL),
            a8_(NULL),
            a9_(NULL),
            //
            goodSquare_(NULL),
            intensityBack_(NULL),
            //
            list0_(NULL),
            list1_(NULL),
            list2_(NULL),
            list3_(NULL),
            list4_(NULL),
            list5_(NULL),
            list6_(NULL),
            list7_(NULL),
            list8_(NULL),
            list9_(NULL),
            //
            lls_(NULL)            
{
	for (int i=0; i<10; i++)
	{
		a_[i] = new double[3];
		
		a_[i][0] = 0.L;
		a_[i][1] = 0.L;
		a_[i][2] = 0.L;
	}
	a0_ = a_[0];
	a1_ = a_[1];
	a2_ = a_[2];
	a3_ = a_[3];
	a4_ = a_[4];
	a5_ = a_[5];
	a6_ = a_[6];
	a7_ = a_[7];
	a8_ = a_[8];
	a9_ = a_[9];

	w_[0] = 0.9L;
	w_[1] = 0.9L;
	w_[2] = 0.9L;	
}

BackgroundRemover_dynamic_MW::~BackgroundRemover_dynamic_MW(void)
{
    deleteAll_();
}



//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

void BackgroundRemover_dynamic_MW::setBackground(RasterImage* bkgd)
{
    try {
        BackgroundRemover_dynamic::setBackground(bkgd);
    }
    catch (ErrorReport& e) {
        e.appendToMessage("called by BackgroundRemover_dynamic_MW::setBackground.");
        throw e;
    }


	int type = bkgd->getBaseRasterType();
    bool	inFloat	= bkgd->hasFloatRaster(),
            inFloatRef = bkgd->floatRasterIsReference();
    
    switch (type)
    {
    	case kGrayRaster:
    		if (inFloat && inFloatRef)
    			setBackground_((RasterImage_gray_F*) bkgd);
    		else
    			setBackground_((RasterImage_gray*) bkgd);
    		break;
		
		case kRGB24Raster:
                        // Compiler warnings go away (for good reasons) when
                        // cases that declare stack variables wrap them in a 
                        // code block. -CJC
                        {
			RasterImage_RGBa* temp = new RasterImage_RGBa(bkgd);
			setBackground_(temp);
			delete temp;
			break;
                        }
    		
    	case kRGBa32Raster:
    		if (inFloat && inFloatRef)
    			setBackground_((RasterImage_RGBa_F*) bkgd);
    		else
    			setBackground_((RasterImage_RGBa*) bkgd);
    		break;
    		
    	default:	// this should not happen
    		break;
    }
}



void BackgroundRemover_dynamic_MW::updateBackground(RasterImage* img, ConstRectList rectList)
{
#if 0   //  May 2010

	//-------------------------------------------------------
	//	Verification
	//-------------------------------------------------------
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in BackgroundRemover_dynamic_MW::updateBackground");

	//	empty RectList
	if (rectList.empty())
		return;
		/*
		FAIL_CONDITION( true,
	        			kInvalidParameterError,
	        			"rectList is empty in BackgroundRemover_dynamic_MW::updateBackground");		
		*/

	//	check the input image's type, must be consistant with firstBackground_
	bool flag = (img->getBaseRasterType() != getFirstBackground_()->getBaseRasterType());
	//cout << "img type : " << img->getBaseRasterType() << endl;
	//cout << "firstBackground_ type: " << firstBackground_->getBaseRasterType() << endl;
	FAIL_CONDITION( flag,
        			kInvalidParameterError,
        			"InputRasterImage's type invalid in BackgroundRemover_dynamic_MW::updateBackground");
	/*
	flag = (img->getBaseRasterType() == kGrayRaster) 
			&& 
			( (firstBackground_->hasFloatRaster() && firstBackground_->floatRasterIsReference() && !img->hasFloatRaster())
			  ||
			  (img->hasFloatRaster() && img->floatRasterIsReference() && !firstBackground_->hasFloatRaster()) );
	FAIL_CONDITION( flag,
        			kInvalidParameterError,
        			"InputRasterImage's type invalid in BackgroundRemover_dynamic_MW::updateBackground");
	*/
	
	//	check the input image's type, 
	//	make firstBackground_ & background_ consistant with it
	int type1 = img->getBaseRasterType();
// 		type2 = firstBackground_->getBaseRasterType(); // unused var. -CJC
    bool	inFloat1	= img->hasFloatRaster(),
            inFloatRef1 = img->floatRasterIsReference(),
            inFloat2	= getFirstBackground_()->hasFloatRaster(),
            inFloatRef2 = getFirstBackground_()->floatRasterIsReference();
    
    //	img and firstBackground_ should always have the same base raster type
	switch (type1)
	{
		case kGrayRaster:
		    //  if either image has a float raster as reference
			if ((inFloat1 && inFloatRef1) || (inFloat2 && inFloatRef2))
	        {
	            //	if img has a float raster as reference
	            if (inFloat1 && inFloatRef1){
	            	//	firstBackground_ also has a float raster as reference
	                if (inFloat2 && inFloatRef2){
//wth???
//	                	firstBackground_	= (RasterImage_gray_F*) firstBackground_;
//	                	background_			= (RasterImage_gray_F*) background_;
	                }
		            //	firstBackground_ is an integer raster
	                else{
//wth???
//                		delete firstBackground_;
//                		delete background_;
//	                	firstBackground_	= new RasterImage_gray_F(firstBackground_);
//	                	background_			= new RasterImage_gray_F(background_);
	                }
	                
	                 updateBackground_((RasterImage_gray_F*) img, rectList);
	            }

	            //	else only firstBackground_ have a float raster as reference
	            else{
//wth???
//	             	firstBackground_	= (RasterImage_gray_F*) firstBackground_;
//	                background_			= (RasterImage_gray_F*) background_;
//	                RasterImage_gray_F *currImg	= new RasterImage_gray_F(img);

//			        updateBackground_(currImg, rectList);	    
//			        delete	currImg;
	            }
	        }
	        //	Otherwise both are RasterImage_gray
	        else
	        {
//wth???
//	        	firstBackground_	= (RasterImage_gray*) firstBackground_;
//	            background_			= (RasterImage_gray*) background_;
		        updateBackground_((RasterImage_gray*) img, rectList);	        
	        }
	        
	        break;
	    	
	    case kRGBa32Raster:
		    //  if either image has a float raster as reference
			if ((inFloat1 && inFloatRef1) || (inFloat2 && inFloatRef2))
	        {
	            //	if img has a float raster as reference
	            if (inFloat1 && inFloatRef1){
	            	//	firstBackground_ also has a float raster as reference
	                if (inFloat2 && inFloatRef2){
//wth?	                	firstBackground_	= (RasterImage_RGBa_F*) firstBackground_;
//	                	background_			= (RasterImage_RGBa_F*) background_;
	                }
		            //	firstBackground_ is an integer raster
	                else{
//                		delete firstBackground_;
//                		delete background_;
//	                	firstBackground_	= new RasterImage_RGBa_F(firstBackground_);
//	                	background_			= new RasterImage_RGBa_F(background_);
	                }
	                
	                 updateBackground_((RasterImage_RGBa_F*) img, rectList);
	            }

	            //	else only firstBackground_ have a float raster as reference
	            else{
	             	firstBackground_	= (RasterImage_RGBa_F*) firstBackground_;
	                background_			= (RasterImage_RGBa_F*) background_;
	                RasterImage_RGBa_F *currImg	= new RasterImage_RGBa_F(img);

			        updateBackground_(currImg, rectList);	    
			        delete	currImg;
	            }
	        }
	        //	Otherwise both are RasterImage_RGBa
	        else
	        {
//	        	firstBackground_	= (RasterImage_RGBa*) firstBackground_;
//	            background_			= (RasterImage_RGBa*) background_;
		        updateBackground_((RasterImage_RGBa*) img, rectList);
	        }
	        
	        break;
	    	
	    default:	//	this should not happen
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in BackgroundRemover_dynamic_MW::updateBackground");

	    	break;
	}
#endif
}


double** BackgroundRemover_dynamic_MW::getCoefficients(void)
{
	return a_;
}

//----------------------------------------------------------
//  protected & private functions
//----------------------------------------------------------
void BackgroundRemover_dynamic_MW::setBackground_(RasterImage_gray_F *bkgd)
{
	const float	*const* firstBg	= bkgd->getShiftedGrayF2D();
	const ImageRect* rect = bkgd->getBoundRect();
			
	const int   iHigh = rect->getBottom(),
			    jHigh = rect->getRight(),
			    nbCols = rect->getWidth(),
			    nbRows = rect->getHeight();
	xUnit_	= (nbCols + 3*size_/4) / size_;		// unit of square on x
	yUnit_	= (nbRows + 3*size_/4) / size_;		// unit of square on y
		
    goodSquare_ = new bool *[yUnit_];
    for (int i=0; i<yUnit_; i++)
	{
        goodSquare_[i] = new bool[xUnit_];
	}
    for (int j=0; j<3; j++)
    {
	    intensityBack_[j] = new double*[yUnit_];
	    for (int i=0; i<yUnit_; i++)
		{
	        intensityBack_[j][i] = new double[xUnit_];
	        
	        for (int k=0; k<xUnit_; k++)
	        {
	        	intensityBack_[j][i][k] = 0.L;
	        }
		}
    }
    
	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's jHigh
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's iHigh
    for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		
		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			// compute intensity
			for (int p=ySqTp; p<=ySqBt; p++)
				for (int q=xSqLf; q<=xSqRt; q++)
					intensityBack_[0][i][j] += firstBg[p][q];
		}
    }    
}

void BackgroundRemover_dynamic_MW::setBackground_(RasterImage_gray* bkgd)
{
	const unsigned char	*const* firstBg	= bkgd->getShiftedGray2D();
	const ImageRect* rect = bkgd->getBoundRect();
			
	const int   iHigh = rect->getBottom(),
			    jHigh = rect->getRight(),
			    nbCols = rect->getWidth(),
			    nbRows = rect->getHeight();
	xUnit_	= (nbCols + 3*size_/4) / size_;		// unit of square on x
	yUnit_	= (nbRows + 3*size_/4) / size_;		// unit of square on y
	

    int i, j, k;
    goodSquare_ = new bool *[yUnit_];
    for (i=0; i<yUnit_; i++)
		{
	        goodSquare_[i] = new bool[xUnit_];
		}
    intensityBack_ = new double**[3];
    for (j=0; j<3; j++)
    {
	    intensityBack_[j] = new double*[yUnit_];
	    for (i=0; i<yUnit_; i++)
		{
	        intensityBack_[j][i] = new double[xUnit_];

	        
	        for (k=0; k<xUnit_; k++)
	        {
	        	intensityBack_[j][i][k] = 0.L;
	        }
		}
    }
    
	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's jHigh
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's iHigh
    for (i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		
		for (j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			// compute intensity
		    int p, q;	
			for (p=ySqTp; p<=ySqBt; p++)
				for (q=xSqLf; q<=xSqRt; q++)
					intensityBack_[0][i][j] += firstBg[p][q];
		}
    }    
}

void BackgroundRemover_dynamic_MW::setBackground_(RasterImage_RGBa_F *bkgd)
{
	const float* const* firstBgRed = bkgd->getShiftedRedF2D();
	const float* const* firstBgGreen = bkgd->getShiftedGreenF2D();
	const float* const* firstBgBlue = bkgd->getShiftedBlueF2D();
	const ImageRect* rect = bkgd->getBoundRect();			
	const int   iHigh = rect->getBottom(),
                jHigh = rect->getRight(),
                nbCols = rect->getWidth(),
                nbRows = rect->getHeight();

	xUnit_	= (nbCols + 3*size_/4) / size_;		// unit of square on x
	yUnit_	= (nbRows + 3*size_/4) / size_;		// unit of square on y
	
    goodSquare_ = new bool *[yUnit_];
    for (int i=0; i<yUnit_; i++)
	{
        goodSquare_[i] = new bool[xUnit_];
	}
    intensityBack_ = new double**[3];
    for (int j=0; j<3; j++)
    {
	    intensityBack_[j] = new double*[yUnit_];
	    for (int i=0; i<yUnit_; i++)
		{
	        intensityBack_[j][i] = new double[xUnit_];

	        
	        for (int k=0; k<xUnit_; k++)
	        {
	        	intensityBack_[j][i][k] = 0.L;
	        }
		}
    }
    
	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's rt
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's bt
			
    for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		
		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			// compute intensity
			for (int p=ySqTp; p<=ySqBt; p++)
			{
				for (int q=xSqLf; q<=xSqRt; q++)
				{
// 					double temp = 0.L; // unused var. -CJC
					/* AVERAGE INTENSITY
					temp = (firstBgRed[p][q] + firstBgGreen[p][q]
							 + firstBgBlue[p][q]) / 3;
					intensityBack_[i][j] += temp;
					*/
					/* MAX INTENSITY
					temp = max(max(firstBgRed[p][q], firstBgGreen[p][q])
								, firstBgBlue[p][q]);
					intensityBack_[i][j] += temp;
					*/
					//	treat RGB respectively
					intensityBack_[0][i][j] += firstBgRed[p][q];
					intensityBack_[1][i][j] += firstBgGreen[p][q];
					intensityBack_[2][i][j] += firstBgBlue[p][q];
				}
			}
		}
    }    
}

void BackgroundRemover_dynamic_MW::setBackground_(RasterImage_RGBa* bkgd)
{
	const unsigned char	*const* firstBg	= bkgd->getShiftedRaster2D();
	const ImageRect* rect = bkgd->getBoundRect();
	const int   iHigh = rect->getBottom(),
                jHigh = rect->getRight(),
                nbCols = rect->getWidth(),
                nbRows = rect->getHeight();

	int xUnit	= (nbCols + 3*size_/4) / size_;		// unit of square on x
	int yUnit	= (nbRows + 3*size_/4) / size_;		// unit of square on y
	
    if ((xUnit != xUnit) || (yUnit != yUnit_)) {
        deleteAll_();
        
        int	nbSquares = yUnit_ * xUnit_;

        goodSquare_ = new bool *[yUnit_];
        for (int i=0; i<yUnit_; i++)
        {
            goodSquare_[i] = new bool[xUnit_];
        }
        intensityBack_ = new double**[3];
        for (int j=0; j<3; j++)
        {
            intensityBack_[j] = new double*[yUnit_];
            for (int i=0; i<yUnit_; i++)
            {
                intensityBack_[j][i] = new double[xUnit_];

                
                for (int k=0; k<xUnit_; k++)
                {
                    intensityBack_[j][i][k] = 0.L;
                }
            }
        }
        
        list0_ = new double[nbSquares];
        list1_ = new double[nbSquares];
        list2_ = new double[nbSquares];
        list3_ = new double[nbSquares];
        list4_ = new double[nbSquares];
        list5_ = new double[nbSquares];
        list6_ = new double[nbSquares];
        list7_ = new double[nbSquares];
        list8_ = new double[nbSquares];
        list9_ = new double*[3];
        for (int i=0; i<3; i++)
            list9_[i] = new double[nbSquares];

    }
    
    
	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's jHigh
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's iHigh
			
    for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		
		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			// compute intensity
			for (int p=ySqTp; p<=ySqBt; p++)
			{
				for (int q=xSqLf; q<=xSqRt; q++)
				{
					//double temp = 0.L;
					/* AVERAGE INTENSITY
					int r;
					for (r=0; r<3; r++, l++)
						temp += firstBg[p][l];
                    temp /= 3;
                    l++;
											
					intensityBack_[i][j] += temp;
					*/
					/* MAX INTENSITY
					int r;
					for (r=0; r<3; r++, l++)
						temp = max(temp, (double)firstBg[p][l]);
                    l++;
											
					intensityBack_[i][j] += temp;
					*/
					//	treat RGB respectively
					int rEntry = 4*q;
					int gEntry = rEntry+1;
					int bEntry = gEntry+1;
					intensityBack_[0][i][j] += (double)firstBg[p][rEntry];
					intensityBack_[1][i][j] += (double)firstBg[p][gEntry];
					intensityBack_[2][i][j] += (double)firstBg[p][bEntry];
				}
			}
		}
    }    
}


void BackgroundRemover_dynamic_MW::updateBackground_(RasterImage_gray_F *currImg, ConstRectList rectList)
{
#if 0
	//-------------------------------------------------------
	//	find areas without objects(blobs), and process them
	//	to get matrix equation's A & B
	//-------------------------------------------------------
	const float* const* firstBg = ((RasterImage_gray_F*) firstBackground_)->getShiftedGrayF2D();
	const float* const* im = ((RasterImage_gray_F*) currImg)->getShiftedGrayF2D();
	float** currBg	= ((RasterImage_gray_F*) background_)->getShiftedGrayF2D(R_W_ACCESS);
	const ImageRect* rect = currImg->getBoundRect();
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();
			
	//  Initially, all squares are good (no blobs yet) except blank ones
	for (int i=0; i<yUnit_ ;i++)
	    for (int j=0; j<xUnit_; j++)
	        if (intensityBack_[0][i][j] != 0)
	            goodSquare_[i][j] = true;
	        else
	            goodSquare_[i][j] = false;
	        
	//-------------------------------------------------------
	//  Each blob "paints" a region of the sqaure array as false
	//  (not to be used for model update because not background).
	//-------------------------------------------------------
	ConstRectList::iterator iter = rectList.begin();
	for ( ;iter != rectList.end(); iter++)
	{
    	int	bTop = ((*iter)->getTop() - iLow) / size_,
    		bBottom	= ((*iter)->getBottom() - iLow) / size_,
    		bLeft	= ((*iter)->getLeft() - jLow) / size_,
    		bRight	= ((*iter)->getRight() - jLow) / size_;
        		
        for (int y=bTop; y<bBottom; y++)
            for (int x=bLeft; x<bRight; x++)
                goodSquare_[y][x] = false;

		
	}


	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's jHigh
		ySqTp	= 0,							// square's iLow
		ySqBt	= 0;							// square's iHigh
	double x, y;			
	int	nbGoodSquares = 0;
			
    for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		y = 0.5*(ySqTp+ySqBt);
		
		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			//	square without objects inside
			if (goodSquare_[i][j])
			{
				x = 0.5*(xSqLf+xSqRt);
						
				// compute intensity
				double	intensityImg	= 0.L,
						intensity		= 0.L;
			    int p, q;	
				for (p=ySqTp; p<=ySqBt; p++)
					for (q=xSqLf; q<=xSqRt; q++)
						intensityImg	+= im[p][q];

				intensity = intensityImg/intensityBack_[0][i][j];				
				
				// get the coefficients matrix of the linear equation
				list0_[nbGoodSquares] = y*y*y;
				list1_[nbGoodSquares] = x*x*x;
				list2_[nbGoodSquares] = y*y*x;
				list3_[nbGoodSquares] = x*x*y;
				list4_[nbGoodSquares] = y*y;
				list5_[nbGoodSquares] = x*x;
				list6_[nbGoodSquares] = y*x;
				list7_[nbGoodSquares] = y;
				list8_[nbGoodSquares] = x;
				list9_[0][nbGoodSquares++] = intensity;
			}
		}
	}
//	int	rowNum	= list0.size();
	int	rowNum	= nbGoodSquares;
/*	double	**matrixA;
			matrixA	= new double*[rowNum];		// double array of coefficients with a,b,c
	for (i=0; i<rowNum; i++)
		matrixA[i]	= new double[10];
	double	*matrixB	= new double[rowNum];	// array of coefficients with I(intensity)
*/
	Matrix			*A	= new Matrix(rowNum, 10);
	double* const *const matrixA = A->getMatrix(R_W_ACCESS);		// double array of coefficients with a,b,c
	ColumnVector	*B	= new ColumnVector(rowNum);
	double* const *const matrixB = B->getMatrix(R_W_ACCESS);		// array of coefficients with I(intensity)

	for (int i=0; i<nbGoodSquares; i++)
	{
		matrixA[i][0]	= list0_[i];
		matrixA[i][1]	= list1_[i];
		matrixA[i][2]	= list2_[i];
		matrixA[i][3]	= list3_[i];
		matrixA[i][4]	= list4_[i];
		matrixA[i][5]	= list5_[i];
		matrixA[i][6]	= list6_[i];
		matrixA[i][7]	= list7_[i];
		matrixA[i][8]	= list8_[i];
		matrixA[i][9]	= 1;
		matrixB[i][0]	= list9_[0][i];
	}

	//-------------------------------------------------------
	// get the best fitting value of a, b, c, using LinearLeastSquares
	//-------------------------------------------------------
	//cout << "rowNum : " << rowNum << endl;
/*	Matrix			*A	= new Matrix(rowNum, 10, matrixA);
	ColumnVector	*B	= new ColumnVector(rowNum, matrixB);
*/
/*	Matrix	*X	= lls_->solve(A, B);
	
	double* const *const mat	= X->getMatrix(R_W_ACCESS);
	double	a0		= mat[0][0],
			a1		= mat[1][0],
			a2		= mat[2][0],
			a3		= mat[3][0],
			a4		= mat[4][0],
			a5		= mat[5][0],
			a6		= mat[6][0],
			a7		= mat[7][0],
			a8		= mat[8][0],
			a9		= mat[9][0];
	
	for (int i=0; i<rowNum; i++)
		delete []matrixA[i];
	delete	[]matrixA;
	delete	[]matrixB;

	delete	A;
	delete	B;
	delete	X;	

//	for (i=0; i<10; i++)
//		delete []mat[i];
//	delete	[]mat;	
		
	//-------------------------------------------------------
	// historical damping
	//-------------------------------------------------------
	//	testcase21 in tester requires no smoothing
	if (!noSmoothing_)
	{
		double temp = 1-w_[0];
		a0_[0] = w_[0]*a0 + temp*a0_[0];
		a1_[0] = w_[0]*a1 + temp*a1_[0];
		a2_[0] = w_[0]*a2 + temp*a2_[0];
		a3_[0] = w_[0]*a3 + temp*a3_[0];
		a4_[0] = w_[0]*a4 + temp*a4_[0];
		a5_[0] = w_[0]*a5 + temp*a5_[0];
		a6_[0] = w_[0]*a6 + temp*a6_[0];
		a7_[0] = w_[0]*a7 + temp*a7_[0];
		a8_[0] = w_[0]*a8 + temp*a8_[0];
		a9_[0] = w_[0]*a9 + temp*a9_[0];
	}
	else
	{
		a0_[0] = a0;
		a1_[0] = a1;
		a2_[0] = a2;
		a3_[0] = a3;
		a4_[0] = a4;
		a5_[0] = a5;
		a6_[0] = a6;
		a7_[0] = a7;
		a8_[0] = a8;
		a9_[0] = a9;
	}	
	
	//-------------------------------------------------------
	//	update background_, replacing the object area
	//	using our model
	//-------------------------------------------------------
	for (i=iLow; i<=iHigh; i++)		// update every pixel of background_ using the model
		for (j=lf; j<=jHigh; j++)
		{
			double	scaleFactor = (a0_[0]*i*i*i + a1_[0]*j*j*j + a2_[0]*i*i*j
									+ a3_[0]*i*j*j + a4_[0]*i*i + a5_[0]*j*j + a6_[0]*i*j
									+ a7_[0]*i + a8_[0]*j + a9_[0]);
			currBg[i][j] = static_cast<float>(scaleFactor * firstBg[i][j]);
		}
*/
#endif
}


void BackgroundRemover_dynamic_MW::updateBackground_(RasterImage_gray* currImg, ConstRectList rectList)
{
#if 0       //  May 2010
	
	//-------------------------------------------------------
	//	find areas without objects(blobs), and process them
	//	to get matrix equation's A & B
	//-------------------------------------------------------
	const unsigned char* const* firstBg = firstBackground_->getShiftedRaster2D();
	const unsigned char* const* im = currImg->getShiftedRaster2D();
	unsigned char** currBg	= background_->getShiftedRaster2D(R_W_ACCESS);
	const ImageRect* rect = currImg->getBoundRect();
	const int   iLow = rect->getTop(),
		        iHigh = rect->getBottom(),
        		jLow = rect->getLeft(),
        		jHigh = rect->getRight();
			
	//  Initially, all squares are good (no blobs yet) except blank ones
	for (int i=0; i<yUnit_ ;i++)
	    for (int j=0; j<xUnit_; j++)
	        if (intensityBack_[0][i][j] != 0)
	            goodSquare_[i][j] = true;
	        else
	            goodSquare_[i][j] = false;
	        
	//-------------------------------------------------------
	//  Each blob "paints" a region of the sqaure array as false
	//  (not to be used for model update because not background).
	//-------------------------------------------------------
	ConstRectList::iterator iter = rectList.begin();
	for ( ;iter != rectList.end(); iter++)
	{
    	const int   bTop = ((*iter)->getTop() - iLow) / size_,
            		bBottom	= ((*iter)->getBottom() - iLow) / size_,
            		bLeft	= ((*iter)->getLeft() - jLow) / size_,
            		bRight	= ((*iter)->getRight() - jLow) / size_;
        		
        for (int i=bTop; i<bBottom; i++)
            for (int j=bLeft; j<bRight; j++)
                goodSquare_[i][j] = false;

		
	}


	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's rt
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's bt
	double	x, y;    			
	int	nbGoodSquares = 0;
	
	for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		y = 0.5*(ySqTp+ySqBt);

		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			//	square without objects inside
			if (goodSquare_[i][j])
			{
				x = 0.5*(xSqLf+xSqRt);
						
				// compute intensity
				double	intensityImg	= 0.L,
						intensity		= 0.L;
			    int p, q;	
				for (p=ySqTp; p<=ySqBt; p++)
					for (q=xSqLf; q<=xSqRt; q++)
						intensityImg	+= im[p][q];

				intensity = intensityImg/intensityBack_[0][i][j];				
				
				// get the coefficients matrix of the linear equation
				list0_[nbGoodSquares] = y*y*y;
				list1_[nbGoodSquares] = x*x*x;
				list2_[nbGoodSquares] = y*y*x;
				list3_[nbGoodSquares] = x*x*y;
				list4_[nbGoodSquares] = y*y;
				list5_[nbGoodSquares] = x*x;
				list6_[nbGoodSquares] = y*x;
				list7_[nbGoodSquares] = y;
				list8_[nbGoodSquares] = x;
				list9_[0][nbGoodSquares++] = intensity;
			}
		}
	}
//	int	rowNum	= list0.size();
	int	rowNum	= nbGoodSquares;
/*	double	**matrixA;
			matrixA	= new double*[rowNum];		// double array of coefficients with a,b,c
	for (i=0; i<rowNum; i++)
		matrixA[i]	= new double[10];
	double	*matrixB	= new double[rowNum];	// array of coefficients with I(intensity)
*/
	Matrix			*A	= new Matrix(rowNum, 10);
	double* const *const matrixA = A->getMatrix(R_W_ACCESS);		// double array of coefficients with a,b,c
	ColumnVector	*B	= new ColumnVector(rowNum);
	double* const *const matrixB = B->getMatrix(R_W_ACCESS);		// array of coefficients with I(intensity)

	for (int i=0; i<nbGoodSquares; i++){
		matrixA[i][0]	= list0_[i];
		matrixA[i][1]	= list1_[i];
		matrixA[i][2]	= list2_[i];
		matrixA[i][3]	= list3_[i];
		matrixA[i][4]	= list4_[i];
		matrixA[i][5]	= list5_[i];
		matrixA[i][6]	= list6_[i];
		matrixA[i][7]	= list7_[i];
		matrixA[i][8]	= list8_[i];
		matrixA[i][9]	= 1;
		matrixB[i][0]	= list9_[0][i];
	}

	//-------------------------------------------------------
	// get the best fitting value of a, b, c, using LinearLeastSquares
	//-------------------------------------------------------
	//cout << "rowNum : " << rowNum << endl;
/*	Matrix			*A	= new Matrix(rowNum, 10, matrixA);
	ColumnVector	*B	= new ColumnVector(rowNum, matrixB);
*/
/*
	Matrix	*X	= lls_->solve(A, B);
	
	double* const *const mat	= X->getMatrix(R_W_ACCESS);
	double	a0		= mat[0][0],
			a1		= mat[1][0],
			a2		= mat[2][0],
			a3		= mat[3][0],
			a4		= mat[4][0],
			a5		= mat[5][0],
			a6		= mat[6][0],
			a7		= mat[7][0],
			a8		= mat[8][0],
			a9		= mat[9][0];
	
	for (int i=0; i<rowNum; i++)
		delete []matrixA[i];
	delete	[]matrixA;
	delete	[]matrixB;
	
	delete	A;
	delete	B;
	delete	X;	
//	for (i=0; i<10; i++)
//		delete []mat[i];
//	delete	[]mat;	
		
	//-------------------------------------------------------
	// historical damping
	//-------------------------------------------------------
	//	testcase21 in tester requires no smoothing
	if (!noSmoothing_)
	{
		double temp = 1-w_[0];
		a0_[0] = w_[0]*a0 + temp*a0_[0];
		a1_[0] = w_[0]*a1 + temp*a1_[0];
		a2_[0] = w_[0]*a2 + temp*a2_[0];
		a3_[0] = w_[0]*a3 + temp*a3_[0];
		a4_[0] = w_[0]*a4 + temp*a4_[0];
		a5_[0] = w_[0]*a5 + temp*a5_[0];
		a6_[0] = w_[0]*a6 + temp*a6_[0];
		a7_[0] = w_[0]*a7 + temp*a7_[0];
		a8_[0] = w_[0]*a8 + temp*a8_[0];
		a9_[0] = w_[0]*a9 + temp*a9_[0];
	}
	else
	{
		a0_[0] = a0;
		a1_[0] = a1;
		a2_[0] = a2;
		a3_[0] = a3;
		a4_[0] = a4;
		a5_[0] = a5;
		a6_[0] = a6;
		a7_[0] = a7;
		a8_[0] = a8;
		a9_[0] = a9;
	}
	
	//-------------------------------------------------------
	//	update background_, replacing the object area
	//	using our model
	//-------------------------------------------------------
	for (i=iLow; i<=iHigh; i++)		// update every pixel of background_ using the model
		for (j=lf; j<=jHigh; j++)
		{
			double	scaleFactor = a0_[0]*i*i*i + a1_[0]*j*j*j + a2_[0]*i*i*j + 
                                  a3_[0]*i*j*j + a4_[0]*i*i + a5_[0]*j*j + a6_[0]*i*j +
                                  a7_[0]*i + a8_[0]*j + a9_[0];
			currBg[i][j] = static_cast<unsigned char>(scaleFactor * firstBg[i][j]);
		}
*/
#endif
}


void BackgroundRemover_dynamic_MW::updateBackground_(RasterImage_RGBa_F *currImg, ConstRectList rectList)
{
#if 0       //  May 2010

	//-------------------------------------------------------
	//	find areas without objects(blobs), and process them
	//	to get matrix equation's A & B
	//-------------------------------------------------------
	const float* const* firstBgRed = ((RasterImage_RGBa_F*) firstBackground_)->getShiftedRedF2D();
	const float* const* firstBgGreen = ((RasterImage_RGBa_F*) firstBackground_)->getShiftedGreenF2D();
	const float* const* firstBgBlue = ((RasterImage_RGBa_F*) firstBackground_)->getShiftedBlueF2D();
	const float* const* imRed = ((RasterImage_RGBa_F*) currImg)->getShiftedRedF2D();
	const float* const* imGreen = ((RasterImage_RGBa_F*) currImg)->getShiftedGreenF2D();
	const float* const* imBlue = ((RasterImage_RGBa_F*) currImg)->getShiftedBlueF2D();
	float** currBgRed = ((RasterImage_RGBa_F*) background_)->getShiftedRedF2D(R_W_ACCESS);
	float** currBgGreen = ((RasterImage_RGBa_F*) background_)->getShiftedGreenF2D(R_W_ACCESS);
	float** currBgBlue = ((RasterImage_RGBa_F*) background_)->getShiftedBlueF2D(R_W_ACCESS);


	const ImageRect* rect = currImg->getBoundRect();
	const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();

	//  Initially, all squares are good (no blobs yet) except blank ones
	for (int i=0; i<yUnit_ ;i++)
	    for (int j=0; j<xUnit_; j++)
	        if (intensityBack_[0][i][j] != 0
	        	|| intensityBack_[1][i][j] != 0
	        	|| intensityBack_[2][i][j] != 0)
	            goodSquare_[i][j] = true;
	        else
	            goodSquare_[i][j] = false;
	        
	//-------------------------------------------------------
	//  Each blob "paints" a region of the sqaure array as false
	//  (not to be used for model update because not background).
	//-------------------------------------------------------
	ConstRectList::iterator iter = rectList.begin();
	for ( ;iter != rectList.end(); iter++)
	{
    	int	bTop = ((*iter)->getTop() - iLow) / size_,
    		bBottom	= ((*iter)->getBottom() - iLow) / size_,
    		bLeft	= ((*iter)->getLeft() - jLow) / size_,
    		bRight	= ((*iter)->getRight() - jLow) / size_;
        		
        for (int i=bTop; i<bBottom; i++)
            for (int j=bLeft; j<bRight; j++)
                goodSquare_[i][j] = false;

	}

	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's jHigh
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's iHigh
		
	double	x, y;				
	int	nbGoodSquares = 0;

    for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		y = 0.5*(ySqTp+ySqBt);
		
		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			//	square without objects inside
			if (goodSquare_[i][j])
			{
				
				x = 0.5*(xSqLf+xSqRt);
						
				// compute intensity
				double	intensityImgR	= 0.L,
						intensityImgG	= 0.L,
						intensityImgB	= 0.L,
						intensityR	= 0.L,
						intensityG	= 0.L,
						intensityB	= 0.L;
			    int p, q;	
				for (p=ySqTp; p<=ySqBt; p++)
					for (q=xSqLf; q<=xSqRt; q++)
					{
// 						double temp = 0.L; // unused var. -CJC
						/* AVERAGE INTENSITY
						temp = (imRed[p][q] + imGreen[p][q] + imBlue[p][q]) / 3;
						*/
						/* MAX INTENSITY
						temp = max(max(imRed[p][q], imGreen[p][q]), imBlue[p][q]);
												
						intensityImg	+= temp;
					}
				intensity = intensityImg/intensityBack_[i][j];
						*/
						intensityImgR += (double)imRed[p][q];
						intensityImgG += (double)imGreen[p][q];
						intensityImgB += (double)imBlue[p][q];
					}
				intensityR = intensityImgR/intensityBack_[0][i][j];
				intensityG = intensityImgG/intensityBack_[1][i][j];			
				intensityB = intensityImgB/intensityBack_[2][i][j];
				
				// get the coefficients matrix of the linear equation
				list0_[nbGoodSquares] = y*y*y;
				list1_[nbGoodSquares] = x*x*x;
				list2_[nbGoodSquares] = y*y*x;
				list3_[nbGoodSquares] = x*x*y;
				list4_[nbGoodSquares] = y*y;
				list5_[nbGoodSquares] = x*x;
				list6_[nbGoodSquares] = y*x;
				list7_[nbGoodSquares] = y;
				list8_[nbGoodSquares] = x;
				list9_[0][nbGoodSquares] = intensityR;
				list9_[1][nbGoodSquares] = intensityG;
				list9_[2][nbGoodSquares++] = intensityB;
			}
		}
	}
//	int	rowNum	= list0.size();
	int	rowNum	= nbGoodSquares;
/*	double	**matrixA;
			matrixA	= new double*[rowNum];		// double array of coefficients with a,b,c
	for (i=0; i<rowNum; i++)
		matrixA[i]	= new double[10];
	double	*matrixB	= new double[rowNum];	// array of coefficients with I(intensity)
*/
	Matrix			*A	= new Matrix(rowNum, 10);
	double* const *const matrixA = A->getMatrix(R_W_ACCESS);		// double array of coefficients with a,b,c
	ColumnVector	*B	= new ColumnVector(rowNum);
	double* const *const matrixB = B->getMatrix(R_W_ACCESS);		// array of coefficients with I(intensity)

	int colBase, rowBase, rowIndex;
	for (int k=0; k<3; k++)
	{
		colBase = 10*k;
		rowBase = nbGoodSquares*k;
		for (int i=0; i<nbGoodSquares; i++)
		{
			rowIndex = rowBase + i;
			matrixA[rowIndex][colBase]		= list0_[i];
			matrixA[rowIndex][colBase+1]	= list1_[i];
			matrixA[rowIndex][colBase+2]	= list2_[i];
			matrixA[rowIndex][colBase+3]	= list3_[i];
			matrixA[rowIndex][colBase+4]	= list4_[i];
			matrixA[rowIndex][colBase+5]	= list5_[i];
			matrixA[rowIndex][colBase+6]	= list6_[i];
			matrixA[rowIndex][colBase+7]	= list7_[i];
			matrixA[rowIndex][colBase+8]	= list8_[i];
			matrixA[rowIndex][colBase+9]	= 1;
			matrixB[rowIndex][0]			= list9_[k][i];
		}
	}

	//-------------------------------------------------------
	// get the best fitting value of a, b, c, using LinearLeastSquares
	//-------------------------------------------------------
	//cout << "rowNum : " << rowNum << endl;
/*	Matrix			*A	= new Matrix(rowNum, 10, matrixA);
	ColumnVector	*B	= new ColumnVector(rowNum, matrixB);
*/
/*	Matrix	*X	= lls_->solve(A, B);
	
	double* const *const mat	= X->getMatrix(R_W_ACCESS);
	double	a0R		= mat[0][0],
			a1R		= mat[1][0],
			a2R		= mat[2][0],
			a3R		= mat[3][0],
			a4R		= mat[4][0],
			a5R		= mat[5][0],
			a6R		= mat[6][0],
			a7R		= mat[7][0],
			a8R		= mat[8][0],
			a9R		= mat[9][0],
			a0G		= mat[10][0],
			a1G		= mat[11][0],
			a2G		= mat[12][0],
			a3G		= mat[13][0],
			a4G		= mat[14][0],
			a5G		= mat[15][0],
			a6G		= mat[16][0],
			a7G		= mat[17][0],
			a8G		= mat[18][0],
			a9G		= mat[19][0],
			a0B		= mat[20][0],
			a1B		= mat[21][0],
			a2B		= mat[22][0],
			a3B		= mat[23][0],
			a4B		= mat[24][0],
			a5B		= mat[25][0],
			a6B		= mat[26][0],
			a7B		= mat[27][0],
			a8B		= mat[28][0],
			a9B		= mat[29][0];
	
	for (int i=0; i<rowNum; i++)
		delete []matrixA[i];
	delete	[]matrixA;
	delete	[]matrixB;

	delete	A;
	delete	B;
	delete	X;	
//	for (i=0; i<30; i++)
//		delete []mat[i];
//	delete	[]mat;	
		
	//-------------------------------------------------------
	// historical damping
	//-------------------------------------------------------
	//	testcase21 in tester requires no smoothing
	if (!noSmoothing_)
	{
		double temp = 1-w_[0];
		a0_[0] = w_[0]*a0R + temp*a0_[0];
		a1_[0] = w_[0]*a1R + temp*a1_[0];
		a2_[0] = w_[0]*a2R + temp*a2_[0];
		a3_[0] = w_[0]*a3R + temp*a3_[0];
		a4_[0] = w_[0]*a4R + temp*a4_[0];
		a5_[0] = w_[0]*a5R + temp*a5_[0];
		a6_[0] = w_[0]*a6R + temp*a6_[0];
		a7_[0] = w_[0]*a7R + temp*a7_[0];
		a8_[0] = w_[0]*a8R + temp*a8_[0];
		a9_[0] = w_[0]*a9R + temp*a9_[0];
		
		temp = 1-w_[1];
		a0_[1] = w_[1]*a0G + temp*a0_[1];
		a1_[1] = w_[1]*a1G + temp*a1_[1];
		a2_[1] = w_[1]*a2G + temp*a2_[1];
		a3_[1] = w_[1]*a3G + temp*a3_[1];
		a4_[1] = w_[1]*a4G + temp*a4_[1];
		a5_[1] = w_[1]*a5G + temp*a5_[1];
		a6_[1] = w_[1]*a6G + temp*a6_[1];
		a7_[1] = w_[1]*a7G + temp*a7_[1];
		a8_[1] = w_[1]*a8G + temp*a8_[1];
		a9_[1] = w_[1]*a9G + temp*a9_[1];
		
		temp = 1-w_[2];
		a0_[2] = w_[2]*a0B + temp*a0_[2];
		a1_[2] = w_[2]*a1B + temp*a1_[2];
		a2_[2] = w_[2]*a2B + temp*a2_[2];
		a3_[2] = w_[2]*a3B + temp*a3_[2];
		a4_[2] = w_[2]*a4B + temp*a4_[2];
		a5_[2] = w_[2]*a5B + temp*a5_[2];
		a6_[2] = w_[2]*a6B + temp*a6_[2];
		a7_[2] = w_[2]*a7B + temp*a7_[2];
		a8_[2] = w_[2]*a8B + temp*a8_[2];
		a9_[2] = w_[2]*a9B + temp*a9_[2];
	}
	else
	{
		a0_[0] = a0R;
		a1_[0] = a1R;
		a2_[0] = a2R;
		a3_[0] = a3R;
		a4_[0] = a4R;
		a5_[0] = a5R;
		a6_[0] = a6R;
		a7_[0] = a7R;
		a8_[0] = a8R;
		a9_[0] = a9R;
		a0_[1] = a0G;
		a1_[1] = a1G;
		a2_[1] = a2G;
		a3_[1] = a3G;
		a4_[1] = a4G;
		a5_[1] = a5G;
		a6_[1] = a6G;
		a7_[1] = a7G;
		a8_[1] = a8G;
		a9_[1] = a9G;
		a0_[2] = a0B;
		a1_[2] = a1B;
		a2_[2] = a2B;
		a3_[2] = a3B;
		a4_[2] = a4B;
		a5_[2] = a5B;
		a6_[2] = a6B;
		a7_[2] = a7B;
		a8_[2] = a8B;
		a9_[2] = a9B;
	}
	
	//-------------------------------------------------------
	//	update background_, replacing the object area
	//	using our model
	//-------------------------------------------------------
	for (i=tp; i<=iHigh; i++)		// update every pixel of background_ using the model
		for (j=lf; j<=jHigh; j++)
		{
			double	scaleFactorR = (a0_[0]*i*i*i + a1_[0]*j*j*j + a2_[0]*i*i*j
									+ a3_[0]*i*j*j + a4_[0]*i*i + a5_[0]*j*j + a6_[0]*i*j
									+ a7_[0]*i + a8_[0]*j + a9_[0]);
			double	scaleFactorG = (a0_[1]*i*i*i + a1_[1]*j*j*j + a2_[1]*i*i*j
									+ a3_[1]*i*j*j + a4_[1]*i*i + a5_[1]*j*j + a6_[1]*i*j
									+ a7_[1]*i + a8_[1]*j + a9_[1]);
			double	scaleFactorB = (a0_[2]*i*i*i + a1_[2]*j*j*j + a2_[2]*i*i*j
									+ a3_[2]*i*j*j + a4_[2]*i*i + a5_[2]*j*j + a6_[2]*i*j
									+ a7_[2]*i + a8_[2]*j + a9_[2]);
	
			double	tempRed		= (scaleFactorR * firstBgRed[i][j]),
					tempGreen 	= (scaleFactorG * firstBgGreen[i][j]),   
					tempBlue 	= (scaleFactorB * firstBgBlue[i][j]);
													  
			currBgRed[i][j] = (float)tempRed;
			currBgGreen[i][j] = (float)tempGreen;
			currBgBlue[i][j] = (float)tempBlue;

			if (tempRed<0)
				currBg[i][4*j] = 0;
			else if (tempRed>255)
				currBg[i][4*j] = (unsigned char)255;
			else
				currBg[i][4*j] = (unsigned char)tempRed;
			if (tempGreen<0)
				currBg[i][4*j+1] = 0;
			else if (tempGreen>255)
				currBg[i][4*j+1] = (unsigned char)255;
			else
				currBg[i][4*j+1] = (unsigned char)tempGreen;
			if (tempBlue<0)
				currBg[i][4*j+2] = 0;
			else if (tempBlue>255)
				currBg[i][4*j+2] = (unsigned char)255;
			else
				currBg[i][4*j+2] = (unsigned char)tempBlue;

			currBgAlpha[i][j] = (float) 255;		    
		}
*/
#endif
}

void BackgroundRemover_dynamic_MW::updateBackground_(RasterImage_RGBa* currImg, ConstRectList rectList)
{
#if 0       //  May 2010
	//-------------------------------------------------------
	//	find areas without objects(blobs), and process them
	//	to get matrix equation's A & B
	//-------------------------------------------------------
	const unsigned char* const* firstBg = firstBackground_->getShiftedRaster2D();
	const unsigned char* const* im = currImg->getShiftedRaster2D();
    unsigned char** currBg = background_->getShiftedRaster2D(R_W_ACCESS);

	const ImageRect* rect = currImg->getBoundRect();
	const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();
			
	//  Initially, all squares are good (no blobs yet) except blank ones
	//	if apply test21_, test22_ in tester,intialization controlled by tester not here
	if (!initializedByTester_)
	{
    	for (int i=0; i<yUnit_ ;i++)
    	    for (int j=0; j<xUnit_; j++)
    	        if ((intensityBack_[0][i][j] != 0) || (intensityBack_[1][i][j] != 0) ||
    	        	(intensityBack_[2][i][j] != 0))
    	            goodSquare_[i][j] = true;
    	        else
    	            goodSquare_[i][j] = false;
	
	}
	        
	//-------------------------------------------------------
	//  Each blob "paints" a region of the square array as false
	//  (not to be used for model update because not background).
	//-------------------------------------------------------
	ConstRectList::iterator iter = rectList.begin();
	if ((*iter) != NULL)
	{
		for ( ;iter != rectList.end(); iter++)
		{
	    	int	bTop = ((*iter)->getTop() - iLow) / size_,
	    		bBottom	= ((*iter)->getBottom() - iLow) / size_,
	    		bLeft	= ((*iter)->getLeft() - jLow) / size_,
	    		bRight	= ((*iter)->getRight() - jLow) / size_;
	       		
	        for (int i=bTop; i<bBottom; i++)
	            for (int j=bLeft; j<bRight; j++)
	            {
	            	//cout << "y: " << y << endl;
	            	//cout << "x: " << x << endl;
	                goodSquare_[i][j] = false;
				}

			
		}
	}


	//	process each non-object square to get matrix equation's A & B	
	int	xSqLf	= 0,							// square's lf
		xSqRt	= 0,							// square's jHigh
		ySqTp	= 0,							// square's tp
		ySqBt	= 0;							// square's iHigh
	double	x, y;			
	int	nbGoodSquares = 0;
	
    for (int i=0; i<yUnit_; i++)
	{
		ySqTp = i*size_;
		ySqBt = ySqTp + size_-1;
		if (ySqBt > iHigh)
		    ySqBt = iHigh;
		y = 0.5*(ySqTp+ySqBt);
		
		for (int j=0; j<xUnit_; j++)
		{
			xSqLf = j*size_;
			xSqRt = xSqLf + size_-1;
			if (xSqRt > jHigh)
			    xSqRt = jHigh;
			
			//	square without objects inside
			if (goodSquare_[i][j]){
				x = 0.5*(xSqLf+xSqRt);
										
				// compute intensity
				double	intensityImgR	= 0.L,
						intensityImgG	= 0.L,
						intensityImgB	= 0.L,
						intensityR	= 0.L,
						intensityG	= 0.L,
						intensityB	= 0.L;

				for (int p=ySqTp; p<=ySqBt; p++)
				{
					for (int q=xSqLf; q<=xSqRt; q++)
					{
						//double temp = 0.L;
						/* AVERAGE INTENSITY
						int r;
						for (r=0; r<3; r++, l++){
							temp += im[p][l];
						}
						temp /= 3;
						l++
												
						intensityImg	+= temp;
						*/
						/* MAX INTENSITY
						int r;
						for (r=0; r<3; r++, l++)
							temp = max(temp, (double)im[p][l]);
						l++;
												
						intensityImg	+= temp;
						*/
						int rEntry = 4*q;
						int gEntry = rEntry+1;
						int bEntry = gEntry+1;

						intensityImgR += (double)im[p][rEntry];
						intensityImgG += (double)im[p][gEntry];
						intensityImgB += (double)im[p][bEntry];
					}
				}
				intensityR = intensityImgR/intensityBack_[0][i][j];
				intensityG = intensityImgG/intensityBack_[1][i][j];			
				intensityB = intensityImgB/intensityBack_[2][i][j];
				
				// get the coefficients matrix of the linear equation
				list0_[nbGoodSquares] = y*y*y;
				list1_[nbGoodSquares] = x*x*x;
				list2_[nbGoodSquares] = y*y*x;
				list3_[nbGoodSquares] = x*x*y;
				list4_[nbGoodSquares] = y*y;
				list5_[nbGoodSquares] = x*x;
				list6_[nbGoodSquares] = y*x;
				list7_[nbGoodSquares] = y;
				list8_[nbGoodSquares] = x;
				list9_[0][nbGoodSquares] = intensityR;
				list9_[1][nbGoodSquares] = intensityG;
				list9_[2][nbGoodSquares++] = intensityB;
			}
		}
	}
//	int	rowNum	= list0.size();
	int	rowNum	= nbGoodSquares;
	cout << "nbGoodSquares: " << nbGoodSquares << endl;
/*	double	**matrixA;
			matrixA	= new double*[rowNum];		// double array of coefficients with a,b,c
	for (i=0; i<rowNum; i++)
		matrixA[i]	= new double[10];
	double	*matrixB	= new double[rowNum];	// array of coefficients with I(intensity)
*/
	rowNum *= 3;
	Matrix			*A	= new Matrix(rowNum, 30);
	double* const *const matrixA = A->getMatrix(R_W_ACCESS);		// double array of coefficients with a,b,c
	ColumnVector	*B	= new ColumnVector(rowNum);
	double* const *const matrixB = B->getMatrix(R_W_ACCESS);		// array of coefficients with I(intensity)

/*///
ofstream	mtxBRedOutput("mtxARed.txt", ios::app),
			mtxBGreenOutput("mtxBGreen.txt", ios::app),
			mtxBBlueOutput("mtxBBlue.txt", ios::app),
			mtxXRGBOutput("mtxXRGB.txt", ios::app);
*/
			

	int colBase, rowBase, rowIndex;
	for (int k=0; k<3; k++)
	{
		colBase = 10*k;
		rowBase = nbGoodSquares*k;
		for (int i=0; i<nbGoodSquares; i++)
		{
			rowIndex = rowBase + i;
			matrixA[rowIndex][colBase]		= list0_[i];
			matrixA[rowIndex][colBase+1]	= list1_[i];
			matrixA[rowIndex][colBase+2]	= list2_[i];
			matrixA[rowIndex][colBase+3]	= list3_[i];
			matrixA[rowIndex][colBase+4]	= list4_[i];
			matrixA[rowIndex][colBase+5]	= list5_[i];
			matrixA[rowIndex][colBase+6]	= list6_[i];
			matrixA[rowIndex][colBase+7]	= list7_[i];
			matrixA[rowIndex][colBase+8]	= list8_[i];
			matrixA[rowIndex][colBase+9]	= 1;
			matrixB[rowIndex][0]			= list9_[k][i];
			
/*
			///
			if(k==0)
				mtxBRedOutput << matrixB[rowIndex][0] << endl;
			if(k==1)
				mtxBGreenOutput << matrixB[rowIndex][0] << endl;
			if(k==2)
				mtxBBlueOutput << matrixB[rowIndex][0] << endl;
*/
		}
	}

	//-------------------------------------------------------
	// get the best fitting value of a, b, c, using LinearLeastSquares
	//-------------------------------------------------------
	//cout << "rowNum : " << rowNum << endl;
/*	Matrix			*A	= new Matrix(rowNum, 10, matrixA);
	ColumnVector	*B	= new ColumnVector(rowNum, matrixB);
*/
/*
	Matrix	*X	= lls_->solve(A, B);

		
	double* const *const mat	= X->getMatrix(R_W_ACCESS);
	

	///
	for (k=0; k<3; k++)
	{
		int l = 10*k;
		mtxXRGBOutput << mat[0+l][0] << endl;
		mtxXRGBOutput << mat[1+l][0] << endl;
		mtxXRGBOutput << mat[2+l][0] << endl;
		mtxXRGBOutput << mat[3+l][0] << endl;
		mtxXRGBOutput << mat[4+l][0] << endl;
		mtxXRGBOutput << mat[5+l][0] << endl;
		mtxXRGBOutput << mat[6+l][0] << endl;
		mtxXRGBOutput << mat[7+l][0] << endl;
		mtxXRGBOutput << mat[8+l][0] << endl;
		mtxXRGBOutput << mat[9+l][0] << endl << endl;
	}

	
	double	a0R		= mat[0][0],
			a1R		= mat[1][0],
			a2R		= mat[2][0],
			a3R		= mat[3][0],
			a4R		= mat[4][0],
			a5R		= mat[5][0],
			a6R		= mat[6][0],
			a7R		= mat[7][0],
			a8R		= mat[8][0],
			a9R		= mat[9][0],
			a0G		= mat[10][0],
			a1G		= mat[11][0],
			a2G		= mat[12][0],
			a3G		= mat[13][0],
			a4G		= mat[14][0],
			a5G		= mat[15][0],
			a6G		= mat[16][0],
			a7G		= mat[17][0],
			a8G		= mat[18][0],
			a9G		= mat[19][0],
			a0B		= mat[20][0],
			a1B		= mat[21][0],
			a2B		= mat[22][0],
			a3B		= mat[23][0],
			a4B		= mat[24][0],
			a5B		= mat[25][0],
			a6B		= mat[26][0],
			a7B		= mat[27][0],
			a8B		= mat[28][0],
			a9B		= mat[29][0];
	
	for (int i=0; i<rowNum; i++)
		delete []matrixA[i];
	delete	[]matrixA;
	delete	[]matrixB;

	delete	A;
	delete	B;
	delete	X;	
//	for (i=0; i<30; i++)
//		delete []mat[i];
//	delete	[]mat;	
		
	//-------------------------------------------------------
	// historical damping
	//-------------------------------------------------------
	//	testcase21 in tester requires no smoothing
	if (!noSmoothing_)
	{
		double temp = 1-w_[0];
		a0_[0] = w_[0]*a0R + temp*a0_[0];
		a1_[0] = w_[0]*a1R + temp*a1_[0];
		a2_[0] = w_[0]*a2R + temp*a2_[0];
		a3_[0] = w_[0]*a3R + temp*a3_[0];
		a4_[0] = w_[0]*a4R + temp*a4_[0];
		a5_[0] = w_[0]*a5R + temp*a5_[0];
		a6_[0] = w_[0]*a6R + temp*a6_[0];
		a7_[0] = w_[0]*a7R + temp*a7_[0];
		a8_[0] = w_[0]*a8R + temp*a8_[0];
		a9_[0] = w_[0]*a9R + temp*a9_[0];
		
		temp = 1-w_[1];
		a0_[1] = w_[1]*a0G + temp*a0_[1];
		a1_[1] = w_[1]*a1G + temp*a1_[1];
		a2_[1] = w_[1]*a2G + temp*a2_[1];
		a3_[1] = w_[1]*a3G + temp*a3_[1];
		a4_[1] = w_[1]*a4G + temp*a4_[1];
		a5_[1] = w_[1]*a5G + temp*a5_[1];
		a6_[1] = w_[1]*a6G + temp*a6_[1];
		a7_[1] = w_[1]*a7G + temp*a7_[1];
		a8_[1] = w_[1]*a8G + temp*a8_[1];
		a9_[1] = w_[1]*a9G + temp*a9_[1];
		
		temp = 1-w_[2];
		a0_[2] = w_[2]*a0B + temp*a0_[2];
		a1_[2] = w_[2]*a1B + temp*a1_[2];
		a2_[2] = w_[2]*a2B + temp*a2_[2];
		a3_[2] = w_[2]*a3B + temp*a3_[2];
		a4_[2] = w_[2]*a4B + temp*a4_[2];
		a5_[2] = w_[2]*a5B + temp*a5_[2];
		a6_[2] = w_[2]*a6B + temp*a6_[2];
		a7_[2] = w_[2]*a7B + temp*a7_[2];
		a8_[2] = w_[2]*a8B + temp*a8_[2];
		a9_[2] = w_[2]*a9B + temp*a9_[2];
	}
	else
	{
		a0_[0] = a0R;
		a1_[0] = a1R;
		a2_[0] = a2R;
		a3_[0] = a3R;
		a4_[0] = a4R;
		a5_[0] = a5R;
		a6_[0] = a6R;
		a7_[0] = a7R;
		a8_[0] = a8R;
		a9_[0] = a9R;
		a0_[1] = a0G;
		a1_[1] = a1G;
		a2_[1] = a2G;
		a3_[1] = a3G;
		a4_[1] = a4G;
		a5_[1] = a5G;
		a6_[1] = a6G;
		a7_[1] = a7G;
		a8_[1] = a8G;
		a9_[1] = a9G;
		a0_[2] = a0B;
		a1_[2] = a1B;
		a2_[2] = a2B;
		a3_[2] = a3B;
		a4_[2] = a4B;
		a5_[2] = a5B;
		a6_[2] = a6B;
		a7_[2] = a7B;
		a8_[2] = a8B;
		a9_[2] = a9B;
	}
	
	
	//-------------------------------------------------------
	//	update background_, replacing the object area
	//	using our model
	//-------------------------------------------------------
	for (i=tp; i<=iHigh; i++)		// update every pixel of background_ using the model
		for (j=lf; j<=jHigh; j++)
		{
			double	scaleFactorR = ((a0_[0]*i*i*i + a1_[0]*j*j*j + a2_[0]*i*i*j
									+ a3_[0]*i*j*j) + (a4_[0]*i*i + a5_[0]*j*j + a6_[0]*i*j)
									+ (a7_[0]*i + a8_[0]*j)) + a9_[0];
			double	scaleFactorG = ((a0_[1]*i*i*i + a1_[1]*j*j*j + a2_[1]*i*i*j
									+ a3_[1]*i*j*j) + (a4_[1]*i*i + a5_[1]*j*j + a6_[1]*i*j)
									+ (a7_[1]*i + a8_[1]*j)) + a9_[1];
			double	scaleFactorB = ((a0_[2]*i*i*i + a1_[2]*j*j*j + a2_[2]*i*i*j
									+ a3_[2]*i*j*j) + (a4_[2]*i*i + a5_[2]*j*j + a6_[2]*i*j)
									+ (a7_[2]*i + a8_[2]*j)) + a9_[2];
	
			int rEntry = 4*j;
			int gEntry = rEntry+1;
			int bEntry = gEntry+1;
			int alphaEntry = bEntry+1;
			
			double	tempRed		= (scaleFactorR * firstBg[i][rEntry]),
					tempGreen 	= (scaleFactorG * firstBg[i][gEntry]),   
					tempBlue 	= (scaleFactorB * firstBg[i][bEntry]);
													  

			currBg[i][4*j] = (unsigned char)tempRed;
			currBg[i][4*j+1] = (unsigned char)tempGreen;
			currBg[i][4*j+2] = (unsigned char)tempBlue;
			
			if (tempRed<=255)
				if (tempRed>0)
					currBg[i][rEntry] = (unsigned char)tempRed;
				else
					currBg[i][rEntry] = 0;
			else
				currBg[i][rEntry] = (unsigned char)255;

			if (tempGreen<=255)
				if (tempGreen>0)
					currBg[i][gEntry] = (unsigned char)tempGreen;
				else
					currBg[i][gEntry] = 0;
			else
				currBg[i][gEntry] = (unsigned char)255;
				
			if (tempBlue<=255)
				if (tempBlue>0)
					currBg[i][bEntry] = (unsigned char)tempBlue;
				else
					currBg[i][bEntry] = 0;
			else
				currBg[i][bEntry] = (unsigned char)255;

			currBg[i][alphaEntry] = (unsigned char) 255;		    
	
    }
*/
#endif
}


void BackgroundRemover_dynamic_MW::deleteAll_(void)
{
    if (lls_ != NULL)
        delete lls_;
        
	for (int i=0; i<10; i++)
	{
		delete []a_[i];
	}
    delete []a_;
    
    if (list0_ != NULL)
        delete []list0_;
    if (list1_ != NULL)
        delete []list1_;
    if (list2_ != NULL)
        delete []list2_;
    if (list3_ != NULL)
        delete []list3_;
    if (list4_ != NULL)
        delete []list4_;
    if (list5_ != NULL)
        delete []list5_;
    if (list6_ != NULL)
        delete []list6_;
    if (list7_ != NULL)
        delete []list7_;
    if (list8_ != NULL)
        delete []list8_;
    if (list9_ != NULL) {
        delete []list9_[0];
        delete []list9_[1];
        delete []list9_[2];
        delete []list9_;
    }
    
    if (goodSquare_ != NULL) {
        for (int k=0; k<yUnit_; k++)
            delete []goodSquare_[k];
        delete []goodSquare_;
    }
    
    if (intensityBack_ != NULL) {
        for (int k=0; k<3; k++)
            delete []intensityBack_[k];
        delete []intensityBack_;
    }

}
