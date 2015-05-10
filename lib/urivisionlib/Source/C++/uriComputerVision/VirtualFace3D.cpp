/*  NAME:
        VirtualFace3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib VirtualFace3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <vector>
#include <cmath>

#include "VirtualFace3D.h"

using namespace std;
using namespace uriVL;


VirtualFace3D::VirtualFace3D()
		:	Entity3D(),
			//
			vertexIndex_(vector<int>())
{    
	
	//  setup the reference frames (look at VirtualPoint3D constructor)
}

VirtualFace3D::~VirtualFace3D(void)
{
	vertexIndex_.clear();
}

void VirtualFace3D::addVertex(int vertIndex)
{
	vertexIndex_.push_back(vertIndex);
}
