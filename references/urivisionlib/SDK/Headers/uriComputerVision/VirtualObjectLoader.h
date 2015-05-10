/*  NAME:
        VirtualObjectLoader.h
 
    DESCRIPTION:
        VirtualObjectLoader public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIRTUAL_OBJECT_LOADER_H
#define URIVL_VIRTUAL_OBJECT_LOADER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"
#include "VirtualObject3D.h"
#include "VirtualPart3D.h"



namespace uriVL
{
	/** A simple class to load 3D models from a file. So far only reads obj files.
	 * 
     *  @author Fang Han & jean-yves herve', 3D Group, 
     *                                       Department of Computer Science and Statistics,
     *                                       University of Rhode Island
	 */
	class URIVL_EXPORT VirtualObjectLoader
	{
		public:

            
            /** Default contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			VirtualObjectLoader(void);

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~VirtualObjectLoader(void);
			
            /**
             *  loads obj files
             *	@param	path the obj file path	
             */
			VirtualObject3D *loadObject(char* path);            
            
           
       
        	
        	
	};
	
}
#endif  //  URIVL_VIRTUAL_OBJECT_LOADER_H
