/*  NAME:
        Calibration3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Calibration3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Calibration3D.h"

using namespace std;
using namespace uriVL;

Calibration3D::Calibration3D(void)
{
}

Calibration3D::~Calibration3D(void)
{
}


void Calibration3D::calibrate(int nbRows, int nbCols, const Point3D *const*const*const refGrid,
                              const ImagePoint_F *const*const* imgGrid, Camera *cam)
{
    //--------------------------------------------------------------------
    //  Step 1:  Create a vector of pointers to CalibrationPair structs
    //--------------------------------------------------------------------
    vector<CalibrationPair*> calibData;
    for (int i=0; i<nbRows; i++)
        for (int j=0; j<nbCols; j++)
            //  If an image point was found add the pair to the vector
            if (imgGrid[i][j] != NULL)
            {
                CalibrationPair *calibPair = new CalibrationPair;
                calibPair->pt3D = refGrid[i][j];
                calibPair->imgPt = imgGrid[i][j];
                calibData.push_back(calibPair);
            }

    //--------------------------------------------------------------------
    //  Step 2:  Perform calibration, using the subclass's implementation
    //--------------------------------------------------------------------
    calibrate(&calibData, cam);
    
    //--------------------------------------------------------------------
    //  Step 3:  Cleanup
    //--------------------------------------------------------------------
	int dataSize = static_cast<int>(calibData.size());
    for (int k=0; k<dataSize; k++)
        delete calibData[k];
    calibData.clear();
}            

