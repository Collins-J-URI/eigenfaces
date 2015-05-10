/*  NAME:
        Calibration3D_Kim.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Calibration3D_Kim class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>
#include <iostream>
#include <cmath>
//
#include "ColumnVector.h"
#include "Calibration3D_Kim.h"
#include "PerspectiveCamera.h"
#include "IntrinsicParameters_TsaiW.h"
#include "LinearSolver_QR.h"

using namespace std;
using namespace uriVL;

//-------------------------------------------------------------------------------
//  Constants and class variables
//-------------------------------------------------------------------------------
const int DIM_LIN_SOL = 11;
const int DIM_LIN_STATE = 12;
const int DIM_NONLIN_STATE = 8;


//-------------------------------------------------------------------------------
//  Constructors and destructor
//-------------------------------------------------------------------------------

Calibration3D_Kim::Calibration3D_Kim(void)
{
}

Calibration3D_Kim::~Calibration3D_Kim(void)
{
}


#if 0
#pragma mark -
#endif

//-------------------------------------------------------------------------------
//  Calibration Calculations
//-------------------------------------------------------------------------------

void Calibration3D_Kim::calibrate(const vector<CalibrationPair*> *calibData, Camera *cam)
{
    //  We only accept perspective cameras with TsaiW intrinsic parameters
    FAIL_CONDITION( typeid(*cam) != typeid(uriVL::PerspectiveCamera),
                    kInvalidCameraType,
                    "Calibration3D_Kim::calibrate only accepts perspective camers (so far)");

    IntrinsicParameters *p = cam->getIntrinsicParameters(R_W_ACCESS);
    FAIL_CONDITION( typeid(*p) != typeid(uriVL::IntrinsicParameters_TsaiW),
                    kInvalidIntrisicParameterType,
                    "Calibration3D_Kim::calibrate only accepts camers with TsaiW parameters");
    IntrinsicParameters_TsaiW *params = dynamic_cast<IntrinsicParameters_TsaiW*>(p);

    cout << endl << "Intrinsic parameters:" << endl << "---------------------" << endl;
    cout << "Nfx = " << params->getWidth() << ", Nfy = " << params->getHeight() << endl;
    cout << "Ox = " << params->getOriginX() << ", Oy = " << params->getOriginY() << endl;
    cout << "f = " << params->getFocalLength() << endl;
    cout << "sx = " << params->getPixelShapeScale() << endl;
    cout << "kappa1 = " << params->getKappa1() << endl;

    double stateVect[DIM_NONLIN_STATE];
    calibrate_linear_(calibData, stateVect);
    
    calibrate_nonlinear_(calibData, stateVect);
}


//-------------------------------------------------------------------------------
//  Linear Calibration Calculations
//-------------------------------------------------------------------------------

void Calibration3D_Kim::calibrate_linear_(const vector<CalibrationPair*> *calibData, 
                                          double stateVect[])
{
    const int NB_PTS = calibData->size();
//	const int m = 2*NB_PTS;
//	const int n = DIM_LIN_SOL;
	
    //----------------------------------------------------------------
    //  Prepare the linear system
    //----------------------------------------------------------------
	Matrix B(2*NB_PTS, DIM_LIN_SOL);
    ColumnVector c(2*NB_PTS);
    double* const*const bMat = B.getMatrix(R_W_ACCESS);
    double* const*const cVect = c.getMatrix(R_W_ACCESS);

	for (int i=0, l=0; i<NB_PTS; i++)
	{
	    const ImagePoint_F *imgPt = (*calibData)[i]->imgPt;
	    const Point3D *pt3D = (*calibData)[i]->pt3D;
	    const float x = static_cast<float>(imgPt->getXF()), y = static_cast<float>(imgPt->getYF()),
	                XW = static_cast<float>(pt3D->getX()), YW = static_cast<float>(pt3D->getY()), 
                    ZW = static_cast<float>(pt3D->getZ());
	    
		bMat[l][0] = XW; 
		bMat[l][1] = YW;
		bMat[l][2] = ZW;
		bMat[l][3] = 1.;
		bMat[l][4] = 0.;
		bMat[l][5] = 0.;
		bMat[l][6] = 0.;
		bMat[l][7] = 0.;
		bMat[l][8] = XW * x;
		bMat[l][9] = YW * x;
		bMat[l][10] = ZW * x;
        //
		cVect[l++][0] = x;

		bMat[l][0] = 0.;
		bMat[l][1] = 0.;
		bMat[l][2] = 0.;
		bMat[l][3] = 0.;
		bMat[l][4] = XW; 
		bMat[l][5] = YW;
		bMat[l][6] = ZW;
		bMat[l][7] = 1.;
		bMat[l][8] = XW * y; 
		bMat[l][9] = YW * y;
		bMat[l][10] = ZW * y;
        //
		cVect[l++][0] = y;
	}
cout << "nb pts = " << NB_PTS << endl;
cout << "Matrix B:" << endl << B.toString("{", "}", ",", false) << endl << endl;
cout << "Vector C:" << endl << c.toString("{", "}", ",", false) << endl << endl;
    
    //----------------------------------------------------------------
    //  Solve the LLS
    //----------------------------------------------------------------
    LinearSolver_QR solverQR(&B);
    LinearSolverRecord  xSol;
    solverQR.solve(&c, &xSol); 
    Matrix *x = xSol.sol;
    const double* const*const xVect = x->getMatrix();
cout << "Solution:" << endl << x->toString("{", "}", ",", false) << endl << endl;

    //----------------------------------------------------------------
    //  Scale the solution and compute the initial state vector
    //----------------------------------------------------------------
    //  The first two rows of the matrix should be unit vectors
    const double alpha = sqrt( (xVect[0][0]*xVect[0][0] + xVect[1][0]*xVect[1][0]
                                                        + xVect[2][0]*xVect[2][0] +
                                xVect[4][0]*xVect[4][0] + xVect[5][0]*xVect[5][0]
                                                        + xVect[6][0]*xVect[6][0])
                                * 0.5L );
    const double alpha1 = sqrt( xVect[0][0]*xVect[0][0] + xVect[1][0]*xVect[1][0]
                                                        + xVect[2][0]*xVect[2][0] );
                                
    const double alpha2 = sqrt( xVect[4][0]*xVect[4][0] + xVect[5][0]*xVect[5][0]
                                                        + xVect[6][0]*xVect[6][0] );
    const double scale = 1.L / alpha;
    const double foc = alpha / sqrt(xVect[8][0]*xVect[8][0] + xVect[9][0]*xVect[9][0] +
                                    xVect[10][0]*xVect[10][0]);
    const double focScale = foc * scale;
    
    const double r11 = xVect[0][0]*scale;                   
    const double r12 = xVect[1][0]*scale;                   
    const double r13 = xVect[2][0]*scale;                   
    const double r21 = xVect[4][0]*scale;                   
    const double r22 = xVect[5][0]*scale;                   
    const double r23 = xVect[6][0]*scale;                   
    const double r31 = xVect[8][0]*focScale;                   
    const double r32 = xVect[9][0]*focScale;                   
    const double r33 = xVect[10][0]*focScale;                   
    const double Tx = scale * xVect[3][0];
    const double Ty = scale * xVect[7][0];
    const double Tz = -focScale;
        
    cout << "alpha = " << alpha << endl;
    cout << "alpha1 = " << alpha1 << endl;
    cout << "alpha2 = " << alpha2 << endl;
    cout << "foc = " << foc << endl;
    cout << "focScale = " << focScale << endl;
    cout << r11 << "\t  \t" << r12 << "\t  \t" << r13 << endl;
    cout << r21 << "\t  \t" << r22 << "\t  \t" << r23 << endl;
    cout << r31 << "\t  \t" << r32 << "\t  \t" << r33 << endl;
    cout << "Translation: " << Tx << ", " << Ty << ", " << Tz << endl << endl;
    cout << "dot prod 1.1 = " << r11*r11 + r21*r21 + r31*r31 << endl;
    cout << "dot prod 2.2 = " << r12*r12 + r22*r22 + r23*r23 << endl;
    cout << "dot prod 3.3 = " << r13*r13 + r23*r23 + r33*r33 << endl;
    cout << "dot prod 1.2 = " << r11*r12 + r21*r22 + r31*r32 << endl;
    cout << "dot prod 1.3 = " << r11*r13 + r21*r23 + r31*r33 << endl;
    cout << "dot prod 2.3 = " << r12*r13 + r22*r23 + r32*r33 << endl;
}

//-------------------------------------------------------------------------------
//  Nonlinear Calibration Calculations
//-------------------------------------------------------------------------------

void Calibration3D_Kim::calibrate_nonlinear_(const vector<CalibrationPair*> *calibData, 
                                             double stateVect[])
{
}
