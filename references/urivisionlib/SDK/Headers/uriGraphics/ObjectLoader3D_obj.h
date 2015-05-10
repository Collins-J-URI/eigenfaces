/*  NAME:
        GraphicComponent3D.h
 
    DESCRIPTION:
        GraphicComponent3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_OBJECT_LOADER_OBJ_H
#define URIVL_OBJECT_LOADER_OBJ_H

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



namespace uriVL
{
    /** Implements a virtual object loader for files in the "obj" format.<p>
     *
     *  This code is derived from something I wrote for my MS student Liqin Ouyang,
     *  when we were trying to get high-res models of bones for her cartilage deformation
     *  project.
     *
	 *	Version history
	 *		- 2005/04/27 [jyh]		adapted and brought into the uriVisionLib
	 *		- 2010/07/2x [jyh]		massive cleanup of C++ code, with 0-warning objective
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class ObjectLoader_obj : public ObjectLoader
    {
    	public:

    		ObjectLoader_obj(char* theFolderPath);

    		virtual ~ObjectLoader_obj(void);


    	private:
 
    };
}

#endif  //  OBJECT_LOADER_OBJ_H
