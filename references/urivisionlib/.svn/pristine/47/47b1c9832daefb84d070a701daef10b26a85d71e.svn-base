/*  NAME:
        HysteresisLabeler.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonMaximumSuppressor class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>
#include <cmath>
//
#include "HysteresisLabeler.h"

#include <iostream>

using namespace std;
using namespace uriVL;

HysteresisLabeler::~HysteresisLabeler()
{

}


LabelRaster* HysteresisLabeler::applyTo(float high, float low, VectorField *vecIn)
{
	//----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
	FAIL_CONDITION( high == 0,
					kNullParameterError,
					"0 value passed for high threshold parameter");

	FAIL_CONDITION( low == 0,
					kNullParameterError,
					"0 value passed for low threshold parameter");
					
	FAIL_CONDITION(	vecIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");
	
	//----------------------------------------------------------------
	//	STEP 1:		Allocate our LabelRaster and get NormSquared 
	//				of our VectorField
	//----------------------------------------------------------------
	int	nbCols = vecIn->getWidth(),
		nbRows = vecIn->getHeight();
		
	LabelRaster *label = new LabelRaster(nbRows,nbCols);
	char** raster = label->getLabel2D(R_W_ACCESS);
	
	const RasterImage_gray_F* normSquaredImg = vecIn->getNormSquaredAsRasterImage();
	const float* const* normSquaredArray = normSquaredImg->getShiftedGrayF2D();

	list< pair<int,int> >	indices(0);

	//----------------------------------------------------------------
	//	STEP 2:		Iterate through the normSquaredArray:
	//				if > high threshold then it is an edge pixel
	//				if < low we mark as zero and ignore
	//----------------------------------------------------------------

	for (int i = 0; i < nbRows - 1; i++)
	{
		
		for (int j = 0; j < nbCols - 1; j++)
		{
			//if true then we know that this is an edge pixel
			if (normSquaredArray[i][j] >= high)
			{
				indices.push_back(pair<int,int>(i,j));
				raster[i][j] = 1;
			}
			
			//if true then we know that this is NOT an edge pixel
			if (normSquaredArray[i][j] < low)
			{
				raster[i][j] = 0;
			}
			
		}
		
	}
	
	if (indices.size() != 0)
	{							
		list< pair<int,int> >::iterator	index = indices.begin();

		//----------------------------------------------------------------
		//  STEP 3:		Iterate through our list of edge pixels
		//				and check to see if its neighboring pixels
		//				are above the low threshold
		//	NOTE:	
		//		There are nine cases in which the edge pixel may be 
		//		that are denoted by an X in the following diagram
		//
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//----------------------------------------------------------------
		for (index = indices.begin(); index != indices.end(); index++)
		{
			//first we handle the four corner cases
			int i = (*index).first,
				j = (*index).second;
			
			//edge pixel at (0,0)
			if (i == 0 && j == 0)
			{
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			} 
			
			//edge pixel at M_[0][nbCols]
			if (i == 0 && j == nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
			
			//edge pixel at M_[nbRows][0]
			if (i == nbRows-1 && j == 0)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));

					}
				}
			}
			
			//edge pixel at (nbCols,nbRows)
			if (i == nbRows-1 && j == nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
			}
			
			//edge pixel along the first row
			if (i == 0 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel along the first Column
			if (i > 0 && i < nbRows-1 && j == 0)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low) 
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel not on a border pixel
			if (i > 0 && i < nbRows-1 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbRows row
			if (i == nbRows-1 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbCols column
			if (i > 0 && i < nbRows-1 && j == nbCols-1)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}

				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
		}
	}
	
	return label;
} 

LabelRaster* HysteresisLabeler::applyTo(float high, float low, VectorField *vecIn, VectorField *vecOut)
{
	//----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
	FAIL_CONDITION( high == 0,
					kNullParameterError,
					"0 value passed for high threshold parameter");

	FAIL_CONDITION( low == 0,
					kNullParameterError,
					"0 value passed for low threshold parameter");
					
	FAIL_CONDITION(	vecIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");
					
	FAIL_CONDITION( vecOut == NULL,
					kNullVectorFieldError,
					"NULL output vector field passed as parameter");
	
	//----------------------------------------------------------------
	//	STEP 1:		Allocate our LabelRaster and get NormSquared 
	//				of our VectorField
	//----------------------------------------------------------------
	int	nbCols = vecIn->getWidth(),
		nbRows = vecIn->getHeight();
		
	LabelRaster *label = new LabelRaster(nbRows,nbCols);
	char** raster = label->getLabel2D(R_W_ACCESS);
	
	const RasterImage_gray_F* normSquaredImg = vecIn->getNormSquaredAsRasterImage();
	const float* const* normSquaredArray = normSquaredImg->getShiftedGrayF2D();
	
	//  Get a reference to the coordinate rasters of the input and output vector fields
    const float* const* vxIn = static_cast<const float* const*>(vecIn->getShiftedRasterX2D());
    const float* const* vyIn = static_cast<const float* const*>(vecIn->getShiftedRasterY2D());
	
	float* const* vxOut = static_cast<float** >(vecOut->getShiftedRasterX2D(R_W_ACCESS)),
		  *const* vyOut = static_cast<float** >(vecOut->getShiftedRasterY2D(R_W_ACCESS));

	list< pair<int,int> >	indices;

	//----------------------------------------------------------------
	//	STEP 2:		Iterate through the normSquaredArray:
	//				if > high threshold then it is an edge pixel
	//				if < low we mark as zero and ignore
	//----------------------------------------------------------------

	for (int i = 0; i < nbRows - 1; i++)
	{
		
		for (int j = 0; j < nbCols - 1; j++)
		{
			vxOut[i][j] = vxIn[i][j];
			vyOut[i][j] = vyIn[i][j];
			
			//if true then we know that this is an edge pixel
			if (normSquaredArray[i][j] >= high)
			{
				indices.push_back(pair<int,int>(i,j));
				raster[i][j] = 1;	
			}
			
			//if true then we know that this is NOT an edge pixel
			if (normSquaredArray[i][j] < low)
			{
				raster[i][j] = 0;
				vxOut[i][j] = vyOut[i][j] = 0.f;
			}
			
		}
		
	}
	
	if (indices.size() != 0)
	{							
		list< pair<int,int> >::iterator	index = indices.begin();

		//----------------------------------------------------------------
		//  STEP 3:		Iterate through our list of edge pixels
		//				and check to see if its neighboring pixels
		//				are above the low threshold
		//	NOTE:	
		//		There are nine cases in which the edge pixel may be 
		//		that are denoted by an X in the following diagram
		//
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//----------------------------------------------------------------
		for (index = indices.begin(); index != indices.end(); index++)
		{
			//first we handle the four corner cases
			int i = (*index).first,
				j = (*index).second;
			
			//edge pixel at (0,0)
			if (i == 0 && j == 0)
			{
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			} 
			
			//edge pixel at M_[0][nbCols]
			if (i == 0 && j == nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
			
			//edge pixel at M_[nbRows][0]
			if (i == nbRows-1 && j == 0)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));

					}
				}
			}
			
			//edge pixel at (nbCols,nbRows)
			if (i == nbRows-1 && j == nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
			}
			
			//edge pixel along the first row
			if (i == 0 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel along the first Column
			if (i > 0 && i < nbRows-1 && j == 0)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low) 
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel not on a border pixel
			if (i > 0 && i < nbRows-1 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbRows row
			if (i == nbRows-1 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbCols column
			if (i > 0 && i < nbRows-1 && j == nbCols-1)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}

				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
		}
	}
	
	for (int i = 0; i < nbRows - 1; i++)
	{
		for (int j = 0; j < nbCols - 1; j++)
		{
			if (raster[i][j] == 1)
			{
				vxOut[i][j] = vxIn[i][j];
				vyOut[i][j] = vyIn[i][j];
			}
		}
	}
	
	return label;
}

void HysteresisLabeler::applyInto(float high, float low, VectorField *vecIn, LabelRaster *labelOut)
{
	//----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
	FAIL_CONDITION( high == 0,
					kNullParameterError,
					"0 value passed for high threshold parameter");

	FAIL_CONDITION( low == 0,
					kNullParameterError,
					"0 value passed for low threshold parameter");
					
	FAIL_CONDITION(	vecIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");
					
	FAIL_CONDITION( labelOut == NULL,
					kNullLabelRasterError,
					"NULL LabelRaster passed as parameter");
	
	//----------------------------------------------------------------
	//	STEP 1:		Allocate our LabelRaster and get NormSquared 
	//				of our VectorField
	//----------------------------------------------------------------
//	int	nbCols = vecIn->getWidth(),
//		nbRows = vecIn->getHeight();
		
	const ImageRect* vrect = vecIn->getValidRect();
    const int   iLow = vrect->getTop(),
                iHigh = vrect->getBottom(),
                jLow = vrect->getLeft(),
                jHigh = vrect->getRight();
			
	char** raster = labelOut->getLabel2D(R_W_ACCESS);
	
	const RasterImage_gray_F* normSquaredImg = vecIn->getNormSquaredAsRasterImage();
	const float* const* normSquaredArray = normSquaredImg->getShiftedGrayF2D();

	list< pair<int,int> >	indices;

	//----------------------------------------------------------------
	//	STEP 2:		Iterate through the normSquaredArray:
	//				if > high threshold then it is an edge pixel
	//				if < low we mark as zero and ignore
	//----------------------------------------------------------------

	for (int i = iLow; i <= iHigh; i++)
	{
		for (int j = jLow; j <= jHigh; j++)
		{
			//if true then we know that this is an edge pixel
			if (normSquaredArray[i][j] >= high)
			{
				indices.push_back(pair<int,int>(i,j));
				raster[i][j] = 1;
			}
			
			//if true then we know that this is NOT an edge pixel
			if (normSquaredArray[i][j] < low)
			{
				raster[i][j] = 0;
			}
			
		}
		
	}
	
	if (indices.size() != 0)
	{							
		list< pair<int,int> >::iterator	index = indices.begin();

		//----------------------------------------------------------------
		//  STEP 3:		Iterate through our list of edge pixels
		//				and check to see if its neighboring pixels
		//				are above the low threshold
		//	NOTE:	
		//		There are nine cases in which the edge pixel may be 
		//		that are denoted by an X in the following diagram
		//
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//----------------------------------------------------------------
		for (index = indices.begin(); index != indices.end(); index++)
		{
			//first we handle the four corner cases
			int i = (*index).first,
				j = (*index).second;
			
			//edge pixel not on a border pixel
			if (i > iLow && i < iHigh && j > jLow && j < jHigh)
			{
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the first row
			else if (i == jLow && j > jLow && j < jHigh)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel along the first Column
			else if (i > iLow && i < iHigh && j == jLow)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low) 
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel along the nbRows row
			else if (i == iHigh && j > jLow && j < jHigh)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbCols column
			else if (i > iLow && i < iHigh && j == jHigh)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}

				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}

			//edge pixel at (0,0)
			else if (i == iLow && j == jLow)
			{
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			} 
			
			//edge pixel at M_[0][nbCols]
			else if (i == iLow && j == jHigh)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
			
			//edge pixel at M_[nbRows][0]
			else if (i == iHigh && j == jLow)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));

					}
				}
			}
			
			//edge pixel at (nbCols,nbRows)
			else if (i == iHigh && j == jHigh)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
			}
			else {
				FAIL_CONDITION( true,
								kNoCode,
								"This should not be happening");
			}

			
		}
	}
	
	labelOut->setValidRect(vrect);
}


void HysteresisLabeler::applyInto(float high, float low, VectorField *vecIn,
								VectorField *vecOut, LabelRaster *labelOut)
{
	//----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
	FAIL_CONDITION( high == 0,
					kNullParameterError,
					"0 value passed for high threshold parameter");

	FAIL_CONDITION( low == 0,
					kNullParameterError,
					"0 value passed for low threshold parameter");
	
	FAIL_CONDITION(	vecIn == NULL,
                    kNullVectorFieldError,
                    "NULL input VectorField passed as parameter");
	FAIL_CONDITION( vecOut == NULL,
					kNullVectorFieldError,
					"NULL output VectorField passed as parameter");
	
	FAIL_CONDITION( labelOut == NULL,
					kNullLabelRasterError,
					"NULL LabelRaster passed as parameter");
	
	//----------------------------------------------------------------
	//	STEP 1:		Allocate our LabelRaster and get NormSquared 
	//				of our VectorField
	//----------------------------------------------------------------
	const ImageRect* vrect = vecIn->getValidRect();
    const int   iLow = vrect->getTop(),
                iHigh = vrect->getBottom(),
                jLow = vrect->getLeft(),
                jHigh = vrect->getRight();


	int	nbCols = vecIn->getWidth(),
		nbRows = vecIn->getHeight();
		
	char** raster = labelOut->getLabel2D(R_W_ACCESS);
	
	const RasterImage_gray_F* normSquaredImg = vecIn->getNormSquaredAsRasterImage();
	const float* const* normSquaredArray = normSquaredImg->getShiftedGrayF2D();
	
	//  Get a reference to the coordinate rasters of the input and output vector fields
    const float* const* vxIn = static_cast<const float* const*>(vecIn->getShiftedRasterX2D());
    const float* const* vyIn = static_cast<const float* const*>(vecIn->getShiftedRasterY2D());
	
	float* const* vxOut = static_cast<float** >(vecOut->getShiftedRasterX2D(R_W_ACCESS)),
		  *const* vyOut = static_cast<float** >(vecOut->getShiftedRasterY2D(R_W_ACCESS));

	list< pair<int,int> >	indices;

	//----------------------------------------------------------------
	//	STEP 2:		Iterate through the normSquaredArray:
	//				if > high threshold then it is an edge pixel
	//				if < low we mark as zero and ignore
	//----------------------------------------------------------------

	for (int i = 0; i < nbRows - 1; i++)
	{
		
		for (int j = 0; j < nbCols - 1; j++)
		{
			vxOut[i][j] = vxIn[i][j];
			vyOut[i][j] = vyIn[i][j];
			//if true then we know that this is an edge pixel
			if (normSquaredArray[i][j] >= high)
			{
				indices.push_back(pair<int,int>(i,j));
				raster[i][j] = 1;
			}
			
			//if true then we know that this is NOT an edge pixel
			if (normSquaredArray[i][j] < low)
			{
				raster[i][j] = 0;
				vxOut[i][j] = vyOut[i][j] = 0.f;
			}
			
		}
		
	}
	
	if (indices.size() != 0)
	{							
		list< pair<int,int> >::iterator	index = indices.begin();

		//----------------------------------------------------------------
		//  STEP 3:		Iterate through our list of edge pixels
		//				and check to see if its neighboring pixels
		//				are above the low threshold
		//	NOTE:	
		//		There are nine cases in which the edge pixel may be 
		//		that are denoted by an X in the following diagram
		//
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//						|	|	|
		//						X---X---X
		//----------------------------------------------------------------
		for (index = indices.begin(); index != indices.end(); index++)
		{
			//first we handle the four corner cases
			int i = (*index).first,
				j = (*index).second;
			
			//edge pixel at (0,0)
			if (i == 0 && j == 0)
			{
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			} 
			
			//edge pixel at M_[0][nbCols]
			if (i == 0 && j == nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
			
			//edge pixel at M_[nbRows][0]
			if (i == nbRows-1 && j == 0)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));

					}
				}
			}
			
			//edge pixel at (nbCols,nbRows)
			if (i == nbRows-1 && j == nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
			}
			
			//edge pixel along the first row
			if (i == 0 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel along the first Column
			if (i > 0 && i < nbRows-1 && j == 0)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low) 
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
			
			//edge pixel not on a border pixel
			if (i > 0 && i < nbRows-1 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i-1][j+1] >= low)
				{
					if (raster[i-1][j+1] != 1)
					{
						raster[i-1][j+1] = 1;
						indices.push_back(pair<int,int>(i-1,j+1));
					}
				}
				
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbRows row
			if (i == nbRows-1 && j > 0 && j < nbCols-1)
			{
				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i][j+1] >= low)
				{
					if (raster[i][j+1] != 1)
					{
						raster[i][j+1] = 1;
						indices.push_back(pair<int,int>(i,j+1));
					}
				}
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}
				
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				if (normSquaredArray[i+1][j+1] >= low)
				{
					if (raster[i+1][j+1] != 1)
					{
						raster[i+1][j+1] = 1;
						indices.push_back(pair<int,int>(i+1,j+1));
					}
				}
			}
				
			//edge pixel along the nbCols column
			if (i > 0 && i < nbRows-1 && j == nbCols-1)
			{
				if (normSquaredArray[i-1][j] >= low)
				{
					if (raster[i-1][j] != 1)
					{
						raster[i-1][j] = 1;
						indices.push_back(pair<int,int>(i-1,j));
					}
				}
				
				
				if (normSquaredArray[i-1][j-1] >= low)
				{
					if (raster[i-1][j-1] != 1)
					{
						raster[i-1][j-1] = 1;
						indices.push_back(pair<int,int>(i-1,j-1));
					}
				}

				if (normSquaredArray[i][j-1] >= low)
				{
					if (raster[i][j-1] != 1)
					{
						raster[i][j-1] = 1;
						indices.push_back(pair<int,int>(i,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j-1] >= low)
				{
					if (raster[i+1][j-1] != 1)
					{
						raster[i+1][j-1] = 1;
						indices.push_back(pair<int,int>(i+1,j-1));
					}
				}
				
				if (normSquaredArray[i+1][j] >= low)
				{
					if (raster[i+1][j] != 1)
					{
						raster[i+1][j] = 1;
						indices.push_back(pair<int,int>(i+1,j));
					}
				}
			}
		}
	}
	
	for (int i = 0; i < nbRows - 1; i++)
	{
		for (int j = 0; j < nbCols - 1; j++)
		{
			if (raster[i][j] == 1)
			{
				vxOut[i][j] = vxIn[i][j];
				vyOut[i][j] = vyIn[i][j];
			}
		}
	}
	
	labelOut->setValidRect(vrect);
	vecOut->setValidRect(vrect);

}
