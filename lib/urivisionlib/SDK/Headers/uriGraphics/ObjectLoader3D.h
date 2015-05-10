/*  NAME:
        ObjectLoader3D.h
 
    DESCRIPTION:
        ObjectLoader3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_OBJECT_LOADER3D_H
#define URIVL_OBJECT_LOADER3D_H

#include <cstdio>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif


namespace uriVL
{
    /** Virtual parent class for all object loaders.<p>
     *
	 *	Version history
	 *		- 2005/04/27 [jyh]		adapted and brought into the uriVisionLib
	 *		- 2010/07/2x [jyh]		massive cleanup of C++ code, with 0-warning objective
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class ObjectLoader3D
    {
    	public:
    		//
    		//	Constructor and destructor	
    		//
    		ObjectLoader3D(char* theFolderPath);

    		virtual ~ObjectLoader3D(void);


    	protected:


    };
}

#endif  //  OBJECT_LOADER3D_H
