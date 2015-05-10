/*  NAME:
        ObjectLoader_3jy.h

    DESCRIPTION:
        ObjectLoader_3jy public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_OBJECT_LOADER_3JY_H
#define URIVL_OBJECT_LOADER_3JY_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ObjectLoader.h"
#include "Box.h"
#include "Prism.h"
#include "PolyMesh.h"
#include "Cylinder.h"
#include "Cone.h"
#include "TubularSection.h"
#include "HalfCylinder.h"
#include "HalfCone.h"


//	Object type strings recognized by the 3D file parser
#define		kOpenBoxStr				"OPEN_BOX"
#define		kClosedBoxStr			"BOX"
#define		kOpenCylinderStr		"OPEN_CYLINDER"
#define		kClosedCylinderStr		"CYLINDER"
#define		kTubularSectionStr		"TUBULAR_SECTION"
#define		kOpenHalfCylinderStr	"OPEN_HALF_CYLINDER"
#define		kClosedHalfCylinderStr	"HALF_CYLINDER"
#define		kOpenPrismStr			"OPEN_PRISM"
#define		kClosedPrismStr			"PRISM"
#define		kOpenConeStr			"OPEN_CONE"
#define		kClosedConeStr			"CONE"
#define		kOpenHalfConeStr		"OPEN_HALF_CONE"
#define		kClosedHalfConeStr		"HALF_CONE"
#define		kPolyMeshStr			"POLYMESH"
#define		kQuadMeshStr			"QUADMESH"
#define		kTriMeshStr			    "TRIMESH"

//	Cylinder input formats recognized by the 3D file parser
#define		kCylAxesStr				"AXES"		//	origin; major, minor, direction axes
#define		kCylRadiusStr			"RADIUS"	//	origin, direction, radius
//
#define		kCylAxesFormat			0
#define		kCylRadiusFormat		1
#define		kNbCylFormats			2

//	Prism input formats recognized by the 3D file parser
#define		kPrismTopAndBotStr		"TOP+BOTTOM"	//	top and bottom faces
#define		kPrismBotAndAxisStr		"BOTTOM+AXIS"	//	 bottom face and direction axis
//
#define		kPrismTopAndBotFormat	0
#define		kPrismBotAndAxisFormat	1
#define		kNbPrismFormats			2

//	Cone input formats recognized by the 3D file parser
#define		kAxisAndRadiiStr		"AXIS+RADII"	//	origin, axis, top & bottom radii
#define		kCone2n1AxesStr			"2+1_AXES"		//
#define		kCone2n1n2AxesStr		"2+1+2_AXES"	//
//
#define		kAxisAndRadiiFormat		0
#define		kCone2n1AxesFormat		1
#define		kCone2n1n2AxesFormat	2
#define		kNbConeFormats			3

//	Half-cylinder and half-cone closure types recognized by the 3D file parser
#define		kSidesClosedStr			"SIDES"			//	Top and bottom sides closed
#define		kLengthClosedStr		"LENGTH"		//	Closed along the length
#define		kAllClosedStr			"ALL_CLOSED"	//	completely closed half shape
//
#define		kSidesClosedType		0
#define		kLengthClosedType		1
#define		kAllClosedType			2
#define		kNbClosureTypes			3



namespace uriVL
{
    /** Implements an object loader for files in the "3jy" format, a file format
     *  that I created to represent robots (manipulator, mobile, hybrid).<p>
     *
	 *  This code is derived from the one I wrote for my personal "multiple-site" version
	 *  of the SMART telerobotics project I did while at Ecole Poytechnique de Montreal.
	 *  I co-wrote (with the interns I supervised) the initial OpenGL C code, then rewrote
	 *  the whole thing as C++ classes and defined my own 3D format (mostly useful to define
	 *  models of robots) to match these classes.  There is absolutely nothing left of the
	 *  original stuff, except for the file format and object loader, which were mine to
	 *  begin with.<p>
	 *
	 *  Older versions of this code have been used (and will continued to be used) as demos
	 *  for my computer graphics courses.
	 *
	 *	Version history
	 *		- 2005/04/27 [jyh]		adapted and brought into the uriVisionLib
	 *		- 2010/07/2x [jyh]		massive cleanup of C++ code, with 0-warning objective
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class ObjectLoader_3jy : public ObjectLoader
    {
    	public:
    		//
    		//	Constructor and destructor
    		//
    		ObjectLoader_3jy(char* theFolderPath);
    		virtual ~ObjectLoader_3jy(void);
    		//
    		//	Public methods
    		//
    		GraphicComponent3D ***getModel(FILE *fp, short *nbSeg, short **nbObj, short **jRef,
    										short nbJ);
    		GraphicComponent3D **getNextSegment(FILE *fp, short *nbObjSeg, short *jRefSeg,
    										short nbJ);
    		GraphicComponent3D *getNextObject(FILE *fObj);

    	private:
    		//
    		//	private members
    		//
    		GraphicComponent3D ***segObject;
    		//
    		char			inputLine[256],
    						localFile[256],
    						myString[256],
    						folderPath[256];
    		//
    		//
    		//	private methods.
    		//
    		void readOneVect(FILE *fp, GLfloat* v);
    		GraphicComponent3D *readOneBox_(FILE *fSeg, short iBoxType);
    		GraphicComponent3D *readOnePrism_(FILE *fSeg, short iPrismType);
    		GraphicComponent3D *readOneCylinder_(FILE *fSeg, short iCylType);
    		GraphicComponent3D *readOneTubSection_(FILE *fSeg);
    		GraphicComponent3D *readOneHalfCyl_(FILE *fSeg, short iHalfCylType);
    		GraphicComponent3D *readOneCone_(FILE *fSeg, short iConeType);
    		GraphicComponent3D *readOneHalfCone_(FILE *fSeg, short iHalfConeType);
    		GraphicComponent3D *readOnePolyMesh_(FILE *fSeg);

    };
}

#endif  //  OBJECT_LOADER_3JY_H
