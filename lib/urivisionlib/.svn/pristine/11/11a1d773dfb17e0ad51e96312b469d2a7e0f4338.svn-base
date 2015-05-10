/*==================================================================================+
#																					#
#	File:		ObjectParser_GL.cpp													#
#																					#
#	This file implements the object parser (OpenGL) class.  An ObjectParser object	#
#	is created by each ObjectParser_GL and VirtualTool objects of a telerobotics	#
#	session.																		#
#																					#
#	Comments:	Right now I can't afford to put the time required to master the		#
#				VRML97Lib API.  Perversely enough, it is faster for me at this		#
#				point to make up my own Mickey Mouse file format and work it from	#
#				there.																#
#																					#
#-----------------------------------------------------------------------------------#
#	Author:			jean-yves herve', Dept of CS, U. of Rhode Island				#
#																					#
#	Date Creation:	June 2001														#
#																					#
#	Modifications:																	#
#																					#
+==================================================================================*/

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
//
//#include "ObjectParser_GL.h"
//#include "readSpecFile.h"
#include "GlUtils.h"
//#include "errorHandling.h"
//#include "graphicsDef.h"

#error Work in progress: do not use this class yet

static char	*gGraphicObjectTypeChoice[] =
				{
					kOpenBoxStr,
					kClosedBoxStr,
					kOpenCylinderStr,
					kClosedCylinderStr,
					kTubularSectionStr,
					kOpenHalfCylinderStr,
					kClosedHalfCylinderStr,
					kOpenPrismStr,
					kClosedPrismStr,
					kOpenConeStr,
					kClosedConeStr,
					kOpenHalfConeStr,
					kClosedHalfConeStr,
					kPolyhedronStr,
				},
			//
			*gCylinderInputChoice[] = 
				{
					kCylAxesStr,
					kCylRadiusStr
				},
			//
			*gPrismInputChoice[] = 
				{	kPrismTopAndBotStr,
					kPrismBotAndAxisStr
				},
			//
			*gConeInputChoice[] = 
				{
					kAxisAndRadiiStr,
					kCone2n1AxesStr,
					kCone2n1n2AxesStr
				},
			//
			*gHalfShapeInputChoice[] =
				{
					kSidesClosedStr,
					kLengthClosedStr,
					kAllClosedStr
				};


/*----------------------------------------------------------------------------------+
|	Constructor:	ObjectParser_GL(folderPath)										|
|																					|
|	Parameters:		folderPath		path to the robot's folder						|
|																					|
|	Description:	Can't think of much that this constructor should do				|
+----------------------------------------------------------------------------------*/
ObjectParser_GL::ObjectParser_GL(char theFolderPath[])
{
	strcpy(folderPath, theFolderPath);
}


/*----------------------------------------------------------------------------------+
|	Destructor:		~ObjectParser_GL(void)											|
|																					|
|	Parameters:		None															|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Can't think of much that this destructor should do				|
+----------------------------------------------------------------------------------*/

ObjectParser_GL::~ObjectParser_GL(void)
{
}



//==================================================================================
//	ObjectParser_GL public methods
//==================================================================================
#pragma mark -



/*----------------------------------------------------------------------------------+
|	Constructor:	getModel(FILE *fp, short *nbSeg, short **nbObj, short **jRef,	|
|								short nbJ)											|
|																					|
|	Parameters:		fp		(open) model spec file									|
|					nbSeg	number of segments in the model that was read			|
|					nbObj	number of object in each segment of the model			|
|					jRef	reference frame fo each of the segments read			|
|					nbJ		number of joints of the manipulator (read in file)		|
|																					|
|	Description:	Reads a complete model composed of multiple segment, themselves	|
|					composed of objects, from a set of spec files.					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL ***ObjectParser_GL::getModel(FILE *fp, short *nbSeg, short **nbObj,
											short **jRef, short nbJ)
{
	GraphicObject_GL	***segObject;
	short	j,
			nbSegments;

	//	Read the number of segments in the model
	readSpecLine(fp, inputLine);
	if ((sscanf(inputLine, "%hd", &nbSegments) != 1) || (nbSegments <= 0))
		handleError(kArmParameters, "Incorrect graphical model (nb of objects)");
	*nbSeg = nbSegments;
	
	//	We can allocate the 2D array of GraphicObject_GL references that will store the
	//	display lists for the rendering
	segObject = new GraphicObject_GL **[nbSegments];
	if (segObject == NULL)
		handleError(kArmParameters, "could not allocate 2D array of GL objects");
	segObject--;
	
	//	nbObj stores the number of objects for each segment
	*nbObj = new short[nbSegments];
	(*nbObj)--;
	
	//	jRef stores each segment's reference frame index
	*jRef = new short[nbSegments];
	(*jRef)--;
	
	//	Now that the arrays have been alocated, we can read the robot's segments
	//	one by one from their respective file.
	for (j=1; j<=nbSegments; j++)
		segObject[j] = getNextSegment(fp, *nbObj+j, *jRef+j, nbJ);
	
	//	Having read everything, we return the array that we have built
	return segObject;
}



/*----------------------------------------------------------------------------------+
|	Function:		void getNextSegment(FILE *fp, short *nbObjSeg, short *jRefSeg,	|
										short nbJ)									|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		fp			(open) model spec file								|
|					nObjSeg		number of elementary objects in this segment		|
|					jRef		reference frame fo each of this segment				|
|					nbJ			number of joints of the manipulator (read in file)	|
|																					|
|	Returns:		Quits in case of error.											|
|																					|
|	Description:	Read one segment from its own file								|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL **ObjectParser_GL::getNextSegment(FILE *fp, short *nbObjSeg,
											short *jRefSeg, short nbJ)
{
	GraphicObject_GL	**myObjects;
	char		localFile[256],
				filePath[256];
	FILE		*fSeg;
	short		k,
				nRef, nbObjects;
	

	//	For each segment, we read the reference frame relative to which it is drawn,
	//	and then the file that contains its model.
	readSpecLine(fp, inputLine);
	if ((sscanf(inputLine, "%hd", &nRef) != 1) || (nRef < 0) || (nRef > nbJ))
		handleError(kArmParameters, "Incorrect graphical model (ref frame number)");
	*jRefSeg = nRef;
	//
	readSpecLine(fp, inputLine);
	if (sscanf(inputLine, "%s", localFile) != 1)
		handleError(kArmParameters, "model file missing");
		
	//	At this point, I only handle my own Mickey Mouse 3D file format (".3jy").
	strcpy(filePath, folderPath);
	strcat(filePath, localFile);
	if ((fSeg = fopen(filePath, "r")) == NULL)
		handleError(kArmParameters, "missing 3D file model");
	readSpecLine(fSeg, inputLine);
	if ((sscanf(inputLine, "%hd", &nbObjects) != 1))
		handleError(kArmParameters, "Incorrect graphical model");
	*nbObjSeg = nbObjects;

	myObjects = (GraphicObject_GL **) calloc(nbObjects, sizeof(GraphicObject_GL *));
	if (myObjects == NULL)
		handleError(kArmParameters, "could not allocate 1D array of GL objects");
	myObjects--;
	//
	//	For each object of the segment, read its parameters
	for (k=1; k<=nbObjects; k++)
		myObjects[k] = getNextObject(fSeg);
	
	fclose(fSeg);
	
	//	Return this segment
	return	myObjects;
}

/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *getNextObject(FILE *fSeg)						|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		fSeg	Pointer to the file where to read the object			|
|																					|
|	Returns:		Quits in case of error.											|
|																					|
|	Description:	Read one object from a segment's spec file						|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::getNextObject(FILE *fSeg)
{
	GraphicObject_GL	*myObj;
	float				x[10];
	short				iObjectType;


	//	first read the reflectance properties: ambient, diffuse, specular, shiny.
	//
	readOneVect(fSeg, x);						//	ambient
	readOneVect(fSeg, x+3);						//	diffuse
	readOneVect(fSeg, x+6);						//	specular
	readSpecLine(fSeg, inputLine);
	if ((sscanf(inputLine, "%f", x+9) != 1))	//	"shiny"
		handleError(kArmParameters, "Incorrect graphical model (shiny refl.)");

	//	Next, we read the type of the 3D primitive
	if ((iObjectType = readChoicePref(fSeg, inputLine, gGraphicObjectTypeChoice,
									(short) kNbObjectTypes)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (unknown obj type)");
	//
	switch (iObjectType)
	{
		case kOpenBoxObject:
		case kClosedBoxObject:
			myObj = readOneBox(fSeg, iObjectType);
			break;

		case kOpenPrismObject:
		case kClosedPrismObject:
			myObj = readOnePrism(fSeg, iObjectType);
			break;

		case kOpenCylinderObject:
		case kClosedCylinderObject:
			myObj = readOneCylinder(fSeg, iObjectType);
			break;

		case kTubularSectionObject:
			myObj = readOneTubSection(fSeg);
			break;

		case kOpenHalfCylinderObject:
		case kClosedHalfCylinderObject:
			myObj = readOneHalfCyl(fSeg, iObjectType);
			break;

		case kOpenConeObject:
		case kClosedConeObject:
			myObj = readOneCone(fSeg, iObjectType);
			break;

		case kOpenHalfConeObject:
		case kClosedHalfConeObject:
			myObj = readOneHalfCone(fSeg, iObjectType);
			break;

		case kPolyhedronObject:
			myObj = readOnePolyhedron(fSeg);
			break;
	
		default:
			handleError(kArmParameters, "Incorrect graphical model (unknown object type)");
			break;
	}

	//	Regardless of the type of object that has just been created, we initialize its
	//	material properties.
	myObj->setMaterial(x);

	return myObj;
}


//==================================================================================
//	ObjectParser_GL public methods
//==================================================================================
#pragma mark -

/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOneBox(FILE *fp, short iBoxType)			|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fp			The file to read from								|
|					iBoxType	indicates whether the box is open or closed			|																					|
|																					|
|	Returns:		A box object.													|
|																					|
|	Description:	Reads the geometric parameters of a Box object					|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOneBox(FILE *fp, short iBoxType)
{
	Box_GL		*myBox;
	GLfloat		**axes, origin[3];
	
	axes = glfMatrix(0, 2, 0, 2);

	//	Read the coordinates of the origin and axes
	readOneVect(fp, origin);
	readOneVect(fp, axes[0]);
	readOneVect(fp, axes[1]);
	readOneVect(fp, axes[2]);
	
	if (iBoxType == kOpenBoxObject)
		myBox = new Box_GL(origin, axes, false);
	else
		myBox = new Box_GL(origin, axes, true);

	free_glfMatrix(axes, 0, 2, 0, 2);

	return myBox;
}


/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOnePrism(FILE *fp, short iPrismType)		|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fp			The file to read from								|
|					iPrismType	indicates whether the prism is open or closed		|																					|
|																					|
|	Returns:		A box object.													|
|																					|
|	Description:	Reads the geometric parameters of a Prism object				|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOnePrism(FILE *fp, short iPrismType)
{
	Prism_GL	*myPrism;
	GLfloat		**topVertex, **bottomVertex,
				oneAxis[3];
	short		iInputFormat, i, nV;
	bool		isSmooth, isClosed;
	

	if (iPrismType == kOpenPrismObject)
		isClosed = false;
	else
		isClosed = true;

	//	Check if the prism should be smooth
	isSmooth = ((sscanf(inputLine, "%s", myString)==1) && (strcmp(myString, "SMOOTH")==0));

	if ((iInputFormat = readChoicePref(fp, inputLine, gPrismInputChoice,
							(short) kNbPrismFormats)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (prism format)");
	switch (iInputFormat)
	{
		//	If the prism is defined as a list of bottom vertices and an axis...
		case kPrismBotAndAxisFormat:
			//	 read the number of vertices in the bottom (and top) face
			readSpecLine(fp, inputLine);
			if ((sscanf(inputLine, "%hd", &nV) != 1))
				handleError(kArmParameters, "Incorrect graphical model (nb pts prism)");

			//	allocate and read the bottom array
			bottomVertex = glfMatrix(0, nV-1, 0, 3);
			for (i=0; i<nV; i++)
				readOneVect(fp, bottomVertex[i]);
			//	read the axis vector
			readOneVect(fp, oneAxis);
			//	create the prism
			myPrism = new Prism_GL(nV, bottomVertex, oneAxis, isClosed, isSmooth);
			//
			//	free the vertex list
			free_glfMatrix(bottomVertex, 0, nV-1, 0, 3);
			break;

		//	If the prism is defined as a top and bottom list of vertices...
		case kPrismTopAndBotFormat:
			//	 read the number of vertices in the bottom (and top) face
			readSpecLine(fp, inputLine);
			if ((sscanf(inputLine, "%hd", &nV) != 1))
				handleError(kArmParameters, "Incorrect graphical model (nb pts prism)");

			//	allocate and read the top array
			topVertex = glfMatrix(0, nV-1, 0, 3);
			for (i=0; i<nV; i++)
				readOneVect(fp, topVertex[i]);
			//	allocate and read the bottom array
			bottomVertex = glfMatrix(0, nV-1, 0, 3);
			for (i=0; i<nV; i++)
				readOneVect(fp, bottomVertex[i]);
			//	create the prism
			myPrism = new Prism_GL(nV, topVertex, bottomVertex, isClosed, isSmooth);
			//
			//	free the vertex lists
			free_glfMatrix(bottomVertex, 0, nV-1, 0, 3);
			free_glfMatrix(topVertex, 0, nV-1, 0, 3);
			break;
			
		default:
			handleError(kArmParameters, "Incorrect graf. model (prism format)");
			break;
	}


	return myPrism;
}



/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOneCylinder(FILE *fSeg, short iCylType)	|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fp			The file to read from								|
|					iCylType	indicates whether the cylinder is open or closed	|																					|
|																					|
|	Returns:		A box object.													|
|																					|
|	Description:	Reads the geometric parameters of a Box object					|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOneCylinder(FILE *fSeg, short iCylType)
{
	Cylinder_GL	*myCyl;
	GLfloat		**axes,
				origin[3], dirAxis[3],
				radius;
	short		iInputFormat;
	
	axes = glfMatrix(0, 2, 0, 2);

	//	Check how the cylinder's dimensions wil be specified
	if ((iInputFormat = readChoicePref(fSeg, inputLine, gCylinderInputChoice,
							(short) kNbCylFormats)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (cylinder format)");
	switch (iInputFormat)
	{
		case kCylAxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, axes[2]);
			//
			if (iCylType == kOpenCylinderObject)
				myCyl = new Cylinder_GL(origin, axes, false);
			else
				myCyl = new Cylinder_GL(origin, axes, true);
			//
			break;
		
		case kCylRadiusFormat:
			//	Read the coordinates of the origin and the direction axis
			readOneVect(fSeg, origin);
			readOneVect(fSeg, dirAxis);
			//	Read the radius
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f", &radius) != 1))
				handleError(kArmParameters, "Incorrect graf. model (cyl rad1)");
			//
			if (iCylType == kOpenCylinderObject)
				myCyl = new Cylinder_GL(origin, dirAxis, radius, false);
			else
				myCyl = new Cylinder_GL(origin, dirAxis, radius, true);
			//
			break;
			
		default:
			handleError(kArmParameters, "Incorrect graf. model (cyl format)");
			break;
	}
	//

	free_glfMatrix(axes, 0, 2, 0, 2);

	return myCyl;
}


/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOneTubSection(FILE *fSeg)					|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fSeg			The file to read from							|
|																					|
|	Returns:		A box object.													|
|																					|
|	Description:	Reads the geometric parameters of a Tubular Section object		|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOneTubSection(FILE *fSeg)
{
	TubularSection_GL	*myTube;
	GLfloat		**axes,
				origin[3], dirAxis[3],
				radius, theta0, thetaf;
	short		iInputFormat;
	

	axes = glfMatrix(0, 2, 0, 2);

	//	Check how the tubular section's dimensions wil be specified
	if ((iInputFormat = readChoicePref(fSeg, inputLine, gCylinderInputChoice,
							(short) kNbCylFormats)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (tub. section format)");
	switch (iInputFormat)
	{
		case kCylAxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, axes[2]);
			//
			//	Now read the min and max angles
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f%f", &theta0, &thetaf) != 2))
				handleError(kArmParameters, "Incorrect graph. model (tubular section)");
			
			myTube = new TubularSection_GL(origin, axes, theta0, thetaf);
			//
			break;
		
		case kCylRadiusFormat:
			//	Read the coordinates of the origin and the direction axis
			readOneVect(fSeg, origin);
			readOneVect(fSeg, dirAxis);
			//	Read the radius
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f", &radius) != 1))
				handleError(kArmParameters, "Incorrect graf. model (tube rad)");
			//
			//	Now read the min and max angles
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f%f", &theta0, &thetaf) != 2))
				handleError(kArmParameters, "Incorrect graph. model (tubular section)");
			
			myTube = new TubularSection_GL(origin, dirAxis, radius, theta0, thetaf);
			//
			break;
			
		default:
			handleError(kArmParameters, "Incorrect graf. model (tube format)");
			break;
	}

	free_glfMatrix(axes, 0, 2, 0, 2);

	return myTube;
}


/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOneHalfCyl(FILE *fp, short iHalfCylType)	|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fp				The file to read from							|
|					iHalfCylType	indicates whether the halfCyl is open or closed	|																					|
|																					|
|	Returns:		A box object.													|
|																					|
|	Description:	Reads the geometric parameters of a Box object					|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOneHalfCyl(FILE *fSeg, short iHalfCylType)
{
	HalfCylinder_GL	*myHalfCyl;
	GLfloat			**axes,
					origin[3], dirAxis[3],
					radius;
	short			iInputFormat;
	bool			lengthClosed, sidesClosed;
	
	
	axes = glfMatrix(0, 2, 0, 2);

	//	First, if it is a closed half cylinder, find out where it is closed.
	if (sscanf(inputLine+strlen(kClosedHalfCylinderStr), "%s", myString) == 1)
	{
		if (strcmp(myString, kAllClosedStr) == 0)
		{
			lengthClosed = sidesClosed = true;
		}
		else if (strcmp(myString, kSidesClosedStr) == 0)
		{
			lengthClosed = false;
			sidesClosed = true;
		}
		else if (strcmp(myString, kLengthClosedStr) == 0)
		{
			lengthClosed = true;
			sidesClosed = false;
		}
	}

	//	Check how the half cylinder's dimensions wil be specified
	if ((iInputFormat = readChoicePref(fSeg, inputLine, gCylinderInputChoice,
							(short) kNbCylFormats)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (cylinder format)");
	switch (iInputFormat)
	{
		case kCylAxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, axes[2]);
			//
			if (iHalfCylType == kOpenHalfCylinderObject)
				myHalfCyl = new HalfCylinder_GL(origin, axes, false, false);
			else
				myHalfCyl = new HalfCylinder_GL(origin, axes, lengthClosed, sidesClosed);
			//
			break;
		
		case kCylRadiusFormat:
			//	Read the coordinates of the origin and the direction axis
			readOneVect(fSeg, origin);
			readOneVect(fSeg, dirAxis);
			//	Read the radius
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f", &radius) != 1))
				handleError(kArmParameters, "Incorrect graf. model (half cyl rad)");
			//
			if (iHalfCylType == kOpenCylinderObject)
				myHalfCyl = new HalfCylinder_GL(origin, dirAxis, radius, lengthClosed,
											sidesClosed);
			else
				myHalfCyl = new HalfCylinder_GL(origin, dirAxis, radius, lengthClosed,
											sidesClosed);
			//
			break;
			
		default:
			handleError(kArmParameters, "Incorrect graf. model (half cyl format)");
			break;
	}
	//

	free_glfMatrix(axes, 0, 2, 0, 2);

	return myHalfCyl;
}


/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOneCone(FILE *fSeg, short iConeType)		|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fSeg			The file to read from							|
|					iConeType		indicates whether the cone is open or closed	|																					|
|																					|
|	Returns:		A Cone object.													|
|																					|
|	Description:	Reads the geometric parameters of a Cone object					|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOneCone(FILE *fSeg, short iConeType)
{
	Cone_GL		*myCone;
	GLfloat		**axes, **altAxes,
				origin[3], dirAxis[3],
				rad1, rad2;
	short		iInputFormat;
	
	axes = glfMatrix(0, 2, 0, 2);
	altAxes = glfMatrix(0, 2, 0, 2);

	//	Check how the cone's dimensions wil be specified
	if ((iInputFormat = readChoicePref(fSeg, inputLine, gConeInputChoice,
							(short) kNbConeFormats)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (cone format)");
	switch (iInputFormat)
	{
		//	The cone is specified by its origin, a major and a minor axes, and
		//	a direction axis (this is a pointed cone).
		case kCone2n1AxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, axes[2]);
			//
			if (iConeType == kOpenConeObject)
				myCone = new Cone_GL(origin, axes, false);
			else
				myCone = new Cone_GL(origin, axes, true);
			//
			break;
		
		//	The cone is specified by its origin, a bottom major and minor axes,
		//	a direction axis, and a top major and minor axes.
		case kCone2n1n2AxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, dirAxis);
			readOneVect(fSeg, altAxes[0]);
			readOneVect(fSeg, altAxes[1]);
			//
			if (iConeType == kOpenConeObject)
				myCone = new Cone_GL(origin, axes, dirAxis, altAxes, false);
			else
				myCone = new Cone_GL(origin, axes, dirAxis, altAxes, true);
			//
			break;
		
		//	The cone is specified by its origin, a direction axis, and a bottom
		//	and top radius (top and bottom faces are orthogonal to the axis
		case kAxisAndRadiiFormat:
			//	Read the coordinates of the origin and the direction axis
			readOneVect(fSeg, origin);
			readOneVect(fSeg, dirAxis);
			//	Read the top and bottom radii
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f", &rad1) != 1))
				handleError(kArmParameters, "Incorrect graf. model (cone rad1)");
			readSpecLine(fSeg, inputLine);
			//
			if ((sscanf(inputLine, "%f", &rad2) != 1))
				handleError(kArmParameters, "Incorrect graf. model (cone rad2)");
			if (iConeType == kOpenConeObject)
				myCone = new Cone_GL(origin, dirAxis, rad1, rad2, false);
			else
				myCone = new Cone_GL(origin, dirAxis, rad1, rad2, true);
			//
			break;
			
		default:
			handleError(kArmParameters, "Incorrect graf. model (cone format)");
			break;
	}
	//

	free_glfMatrix(axes, 0, 2, 0, 2);
	free_glfMatrix(altAxes, 0, 2, 0, 2);

	return myCone;
}


/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOneHalfCone(FILE *fp, short iHalfConeType)|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fp				The file to read from							|
|					iHalfConeType	indicates whether the 1/2cone is open or closed	|																					|
|																					|
|	Returns:		A Half Cone object.												|
|																					|
|	Description:	Reads the geometric parameters of a Half Cone object			|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOneHalfCone(FILE *fSeg, short iHalfConeType)
{
	HalfCone_GL	*myHalfCone;
	GLfloat		**axes, **altAxes,
				origin[3], dirAxis[3],
				rad1, rad2;
	short		iInputFormat;
	bool		lengthClosed, sidesClosed;

	axes = glfMatrix(0, 2, 0, 2);
	altAxes = glfMatrix(0, 2, 0, 2);

	//	First, if it is a closed half cone, find out where it is closed.
	if (sscanf(inputLine+strlen(kClosedHalfConeStr), "%s", myString) == 1)
		if (strcmp(myString, kAllClosedStr) == 0)
		{
			lengthClosed = sidesClosed = true;
		}
		else if (strcmp(myString, kSidesClosedStr) == 0)
		{
			lengthClosed = false;
			sidesClosed = true;
		}
		else if (strcmp(myString, kLengthClosedStr) == 0)
		{
			lengthClosed = true;
			sidesClosed = false;
		}

	//	Check how the half cone's dimensions wil be specified
	if ((iInputFormat = readChoicePref(fSeg, inputLine, gConeInputChoice,
							(short) kNbConeFormats)) < 0)
		handleError(kArmParameters, "Incorrect graphical model (cone format)");
	switch (iInputFormat)
	{
		//	The half cone is specified by its origin, a major and a minor axes, and
		//	a direction axis (this is a pointed cone).
		case kCone2n1AxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, axes[2]);
			//
			if (iHalfConeType == kOpenConeObject)
				myHalfCone = new HalfCone_GL(origin, axes, false, false);
			else
				myHalfCone = new HalfCone_GL(origin, axes, lengthClosed, sidesClosed);
			//
			break;
		
		//	The half cone is specified by its origin, a bottom major and minor axes,
		//	a direction axis, and a top major and minor axes.
		case kCone2n1n2AxesFormat:
			//	Read the coordinates of the origin and axes
			readOneVect(fSeg, origin);
			readOneVect(fSeg, axes[0]);
			readOneVect(fSeg, axes[1]);
			readOneVect(fSeg, dirAxis);
			readOneVect(fSeg, altAxes[0]);
			readOneVect(fSeg, altAxes[1]);
			//
			if (iHalfConeType == kOpenConeObject)
				myHalfCone = new HalfCone_GL(origin, axes, dirAxis, altAxes, false, false);
			else
				myHalfCone = new HalfCone_GL(origin, axes, dirAxis, altAxes, lengthClosed,
											sidesClosed);
			//
			break;
		
		//	The half cone is specified by its origin, a direction axis, and a bottom
		//	and top radius (top and bottom faces are orthogonal to the axis
		case kAxisAndRadiiFormat:
			//	Read the coordinates of the origin and the direction axis
			readOneVect(fSeg, origin);
			readOneVect(fSeg, dirAxis);
			//	Read the top and bottom radii
			readSpecLine(fSeg, inputLine);
			if ((sscanf(inputLine, "%f", &rad1) != 1))
				handleError(kArmParameters, "Incorrect graf. model (cone rad1)");
			readSpecLine(fSeg, inputLine);
			//
			if ((sscanf(inputLine, "%f", &rad2) != 1))
				handleError(kArmParameters, "Incorrect graf. model (half cone rad2)");
			if (iHalfConeType == kOpenConeObject)
				myHalfCone = new HalfCone_GL(origin, dirAxis, rad1, rad2, false, false);
			else
				myHalfCone = new HalfCone_GL(origin, dirAxis, rad1, rad2, lengthClosed,
											sidesClosed);
			//
			break;
			
		default:
			handleError(kArmParameters, "Incorrect graf. model (half cone format)");
			break;
	}

	free_glfMatrix(axes, 0, 2, 0, 2);
	free_glfMatrix(altAxes, 0, 2, 0, 2);

	return myHalfCone;
}


/*----------------------------------------------------------------------------------+
|	Function:		GraphicObject_GL *readOnePolyhedron(FILE *fSeg)					|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		fSeg		The file to read from								|
|																					|
|	Returns:		A Polyhedron_GL object.											|
|																					|
|	Description:	Reads the geometric parameters of a Polyhedron object			|
|																					|
+----------------------------------------------------------------------------------*/
GraphicObject_GL *ObjectParser_GL::readOnePolyhedron(FILE *fSeg)
{
	Polyhedron_GL	*myPolyhedron;
	GLfloat			**vertex;
	GLuint			**face;
	short			i, k, nV, nF, nVertInF;
	bool			isSmooth;
	
	//	Check if the polyhedron should be smooth
	if ((sscanf(inputLine, "%s", myString) == 1) && (strcmp(myString, "SMOOTH")==0))
			isSmooth = true;
	//
	//	Read the number of vertices
	readSpecLine(fSeg, inputLine);
	if ((sscanf(inputLine, "%hd", &nV) != 1))
		handleError(kArmParameters, "Incorrect model (nb pts polyhedron)");
	//	Read the number of faces
	readSpecLine(fSeg, inputLine);
	if ((sscanf(inputLine, "%hd", &nF) != 1))
		handleError(kArmParameters, "Incorrect model (nb faces polyhedron)");
	//	Allocate and read array of vertices
	vertex = glfMatrix(0, nV-1, 0, 2);
	for (i=0; i<nV; i++)
		readOneVect(fSeg, vertex[i]);

	//	allocate array of faces (lines)
	face = new GLuint*[nF];
	if (face == NULL)
		handleError(kArmParameters, "polyhedron faces allocation");
	//	for each face of the polyhedron
	for (i=0; i<nF; i++)
	{
		readSpecLine(fSeg, inputLine);
		if ((sscanf(inputLine, "%hd", &nVertInF) != 1) ||
											(nVertInF<=0) || (nVertInF>nV))
			handleError(kArmParameters, "Incorrect model (nb pts polyhedron)");
		//
		face[i] = new GLuint[nVertInF];
		face[i][0] = nVertInF;
		readSpecLine(fSeg, inputLine);
		if (readNItemsFromString((char* )face[i]+1, nVertInF, inputLine, "%hd",
			sizeof(GLuint)) != nVertInF)
			handleError(kArmParameters, "Incorrect model (polyhedron's face)");
		for (k=1; k<=nVertInF; k++)
			if ((face[i][k] <0) || (face[i][k] > nVertInF))
				handleError(kArmParameters, "Incorrect model (polyhedron's face)");

	}

	//	at long last we can create the polyhedron
	myPolyhedron = new Polyhedron_GL(nV, nF, vertex, face, isSmooth);

	//	and now we can do  the cleanup job.
	free_glfMatrix(vertex, 0, nV-1, 0, 2);
	for (i=0; i<nF; i++)
		free_gluiVector(face[i], 0, face[i][0]);
	delete [] face;

	return myPolyhedron;
}


/*----------------------------------------------------------------------------------+
|	Function:		void readOneVect(FILE *fp, GLfloat* v)							|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		v:		the 3D coordinate vector that must be read				|
|																					|
|	Returns:		Quits in case of error.											|
|																					|
|	Description:	Reads 3 float numbers into a 3D coordinate vector				|
|																					|
+----------------------------------------------------------------------------------*/
void ObjectParser_GL::readOneVect(FILE *fp, GLfloat* v)
{
	float	x[3];
	
	readSpecLine(fp, inputLine);
	if ((sscanf(inputLine, "%f%f%f", x, x+1, x+2) != 3))
		handleError(kArmParameters, "Incorrect graphical model (3D coord. vector)");
	
	v[0] = x[0]; v[1] = x[1]; v[2] = x[2];
}