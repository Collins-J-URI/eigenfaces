/*  NAME:
        SegmentDetector.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib SegmentDetector class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "SegmentDetector.h"

using namespace uriVL;

#error Work in progress: do not use this class yet

SegmentDetector::SegmentDetector(SegmentDetectorType theType,
                                 int blockWidth,
                                 int blockHeight,
                                 int windowSize,
                                 int voteThresh)
{
    segType = theType;
    bWidth = blockWidth;
    bHeight = blockHeight;
    wSize = windowSize;
    threshold = voteThresh;

}

SegmentDetector::~SegmentDetector(void)
{}


void SegmentDetector::detect(RasterImage* inputImage, vector <int> *v)
{
    int numberCol,numberRow;
    numberCol = inputImage->getWidth();
    numberRow = inputImage->getHeight();
    unsigned char 	**inRaster2D;
    inRaster2D = inputImage->getRaster2D();

    int step = 2;

    switch(segType)
    {
        case kVertical:
            int accumulator = 0;
            int windowNumber = int(bWidth/step);
            int tempX;
            int tempY;
            int* pixelCount;

            pixelCount = new int[windowNumber];
            for(int j=0; j<windowNumber; j++)
            {
                pixelCount[j]=0;
            }

            for(int rowIndex = 0; rowIndex < (numberRow-bHeight); rowIndex += bHeight)
            {
                for(int colIndex = 0; colIndex < (numberCol-bWidth); colIndex +=bWidth)
                {
                    //colllect votes within the block
                    tempX = rowIndex;
                    for(int x = 0; x < bHeight; x++, tempX++)
                    {
                        tempY = colIndex;
                        for(int y = 0; y < bWidth; y++, tempY++)
                        {

                            if (inRaster2D[tempX][tempY] == 0)                                                                                                  							if(inRaster2D[tempX][tempY]==0)
                            {
                                if(y<step)
                                {
                                    pixelCount[0]++;
                                }
                                else
                                {
                                    pixelCount[int(y/step)]++;
                                    if((y%step)<(wSize-step))
                                        pixelCount[int(y/step)-1]++;
                                }
                            }
                        }
                    }

                    int maxCount = 0;
                    int maxIndex = 0;
                    for(int i = 0; i < windowNumber; i++)
                        //find the vertical window with the max pixels
                    {
                        if(pixelCount[i] > maxCount)
                        {
                            maxCount = pixelCount[i];
                            maxIndex = i;
                        }
                        if(maxCount>threshold)
                        {
                            v->push_back(colIndex+(i*step));
                            v->push_back(rowIndex);
                            v->push_back(colIndex+(i*step)+wSize);
                            v->push_back(rowIndex+bHeight);
                        }
                    }

                    for(int j=0; j<windowNumber; j++)
                    {
                        pixelCount[j]=0;
                    }
                }
            }
            delete pixelCount;

            break;
        case kHorizontal:
            break;
        default:
            break;
    }


}

