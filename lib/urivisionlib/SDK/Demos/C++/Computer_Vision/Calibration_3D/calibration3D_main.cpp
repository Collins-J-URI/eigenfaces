/*------------------------------------------------------------------+
|	calibration3D_main.cpp										    |
|																	|
|	Just a little test program for the calibration classes.	        |
+------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
//
#include "IntrinsicParameters_TsaiW.h"
#include "PerspectiveCamera.h"
#include "Calibration3D_Kim.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Function prototypes
//----------------------------------------------------------
void readData(const string &filePath);
void deleteData(void);

//----------------------------------------------------------
//  Constants
//----------------------------------------------------------
#if	URIVL_OS_MACOS
	const string DATA_DIR = "../../../../Data/";
#elif URIVL_OS_WIN32 || URIVL_OS_UNIX
	const string DATA_DIR = "../../../../Data/";
#else
    #error "OS not supported"
#endif

const int NB_DATA_SETS = 3;
const string FILE_NAME[NB_DATA_SETS] = {"data01.txt", "data02.txt", "data03.txt"};


//----------------------------------------------------------
//  Global variables
//----------------------------------------------------------

IntrinsicParameters_TsaiW   *gTrueParams = NULL,
                            *gCalibParams = NULL;
RigidTransformation3D       *gTrueTransf = NULL,
                            *gCalibTransf = NULL;
PerspectiveCamera           *gCalibCam;
vector<CalibrationPair*>     gCalibData;

          							
int main(void)
{
ofstream outFile("debugOut.txt");
outFile << "AOK" << endl;

    Calibration3D_Kim *calib = new Calibration3D_Kim();
        
    for (int i=0; i<NB_DATA_SETS; i++) 
    {
        string filePath = DATA_DIR + FILE_NAME[i];
        readData(filePath);

        calib->calibrate(&gCalibData, gCalibCam);
        
        deleteData();
    }
    
    return 0;
}


void readData(const string &filePath)
{
    int     Nfx, Nfy, Ncx, Ncy;
    double  dx, dy, dpx, dpy, originX, originY;
    double  f, sx, kappa1;
    double  psi, theta, phi, Tx, Ty, Tz;
    int     nbPts;
        
    try 
    {
        //-----------------------------------------------------
        //  Open data file
        //-----------------------------------------------------
        const char *filePathStr = filePath.c_str();
        ifstream inFile(filePathStr);
        if (!inFile)
        {
            cout << "could not open file " << filePathStr << endl;
            std::exit(-1);
        }
    
        //-----------------------------------------------------
        //  Read "true" parameters
        //-----------------------------------------------------
        inFile >>   psi >> theta >> phi >> Tx >> Ty >> Tz;
        inFile >>   Ncx >> Ncy >> dx >> dy >> Nfx >> Nfy >> dpx >> dpy >> originX >> originY >> 
                    sx >> f >> kappa1;

        cout << "psi=" << psi << ", theta=" << theta << ", phi=" << phi << endl;
        cout << "Tx=" << Tx << ", Ty=" << Ty << ", Tz=" << Tz << endl;
        cout << "Ncx=" << Ncx << ", Ncy=" << Ncy << ", dx=" << dx << ", dy=" << dy << endl;
        cout << "Nfx=" << Nfx << ", Nfy=" << Nfy << ", dpx=" << dpx << ", dpy=" << dpy  << endl; 
        cout << "originX=" << originX << ", originY=" << originY << ", sx=" << sx << endl;
        cout << "f=" << f << ", kappa1=" << kappa1 << endl;
        
        //-----------------------------------------------------
        //  Calibration data proper
        //-----------------------------------------------------
        inFile >> nbPts;
        for (int i=0; i<nbPts; i++)
        {
            //  read all data for one point
            double X, Y, Z, x, y;
            inFile >> X >> Y >> Z >> x >> y;
            Point3D *pt3D = new Point3D(X, Y, Z);
            ImagePoint_F *imgPt = new ImagePoint_F(x, y);
            CalibrationPair *cPair = new CalibrationPair;
            cPair->pt3D = pt3D;
            cPair->imgPt = imgPt;
            gCalibData.push_back(cPair);
        }

        inFile.close();
    }
    catch (...)
    {
        cout << "I/O error while dealing with file " << filePath << endl;
        std::exit(-1);
    }
    
    gTrueParams = new IntrinsicParameters_TsaiW(Nfx, Nfy, Ncx, Ncy, dx, dy, originX, originY, f, sx, kappa1);
    gCalibParams = new IntrinsicParameters_TsaiW(Nfx, Nfy, Ncx, Ncy, dx, dy, Nfx/2, Nfy/2, 1.1*f, 1.L, 0.L);

    double  trueTrans[3] = {Tx, Ty, Tz},
            trueRPY[3] = {psi, theta, phi};
    gTrueTransf = new RigidTransformation3D(ROLL_PITCH_YAW_REF, trueRPY, trueTrans);
    
    double calibTrans[3] = {0, 0, 0},
            calibRPY[3] = {0, 0, 0};
    gCalibTransf = new RigidTransformation3D(ROLL_PITCH_YAW_REF, calibRPY, calibTrans);
    gCalibCam = new PerspectiveCamera(gCalibParams, gCalibTransf);         
}

void deleteData(void)
{
cout << "delete calib data" << endl;
    if (gCalibData.size()>0)
    {
        for (int i=0; i<gCalibData.size(); i++)
        {
cout << "\tcalib pair " << i << ": ";
            delete gCalibData[i]->imgPt;
cout << "image pt";
            delete gCalibData[i]->pt3D;
cout << ", pt 3d";
            delete gCalibData[i];
cout << ", pair" << endl;            
        }
        gCalibData.clear();
cout << "list" << endl;
    }
    
    delete gTrueTransf;
cout << "gTrueTransf" << endl;
    delete gTrueParams;
cout << "gTrueParams" << endl;
    delete gCalibTransf;
cout << "gCalibTransf" << endl;
    delete gCalibParams;
cout << "gCalibParams" << endl;
    gTrueParams = gCalibParams = NULL;
    gTrueTransf = gCalibTransf = NULL;
    delete gCalibCam;
cout << "gCalibCam" << endl;
    gCalibCam = NULL;    
}
