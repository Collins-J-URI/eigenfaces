/*  NAME:
        VirtualObjectLoader.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib VirtualObjectLoader class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
//
#include "VirtualObjectLoader.h"

using namespace std;
using namespace uriVL;


VirtualObjectLoader::VirtualObjectLoader( void)
{    
}


VirtualObjectLoader::~VirtualObjectLoader(void)
{
}

//  This is a quick hack to load a simple (single part) obj file.
VirtualObject3D *VirtualObjectLoader::loadObject(char* path)
{
	VirtualObject3D *obj = new VirtualObject3D();
	VirtualPart3D *part = new VirtualPart3D(obj);
	
    ifstream myFile;
    myFile.open(path);
    FAIL_CONDITION( !myFile.is_open(),
                    kFileOpenReadError,
                    "Could not open obj file");
            
    string dataLine;
    
    //---------------------------------------------------------------------------
    //  Part 1:  Comments, group and illumination data
    //  ignore alll stuff untill we hit the list of vertices
    //---------------------------------------------------------------------------
    bool notVertsYet = true;
    while (notVertsYet) 
    {
        getline(myFile, dataLine);

        if ((dataLine[0]=='v') && (dataLine[1]==' '))
            notVertsYet = false;
    }
    
    
    //---------------------------------------------------------------------------
    //  Part 2:  Vertex data
    //---------------------------------------------------------------------------
    bool readingVertices = true;
    double   X, Y, Z;
    while (readingVertices)
    {
        istringstream  ins;
        //  extract from the string the vertex' X, Y, Z coordinates
        dataLine.erase(0, 2);
        ins.str(dataLine);
        ins >> X >> Y >> Z;
// 		VirtualPoint3D *pt = new VirtualPoint3D(X, Y, Z, part);	// unused var. -CJC
        //  read the next line.  Check whether we are done with vertices
        getline(myFile, dataLine);
        if ((dataLine[0]!='v') || (dataLine[1]!=' '))
            readingVertices = false;

    }

	
    //---------------------------------------------------------------------------
    //  Part 3:  Texture data [possibly empty]
    //---------------------------------------------------------------------------
    if ((dataLine[0] == 'v') && (dataLine[1] == 't')) 
    {
        bool readingTexturePts = true;
        float   u, v;
        while (readingTexturePts)
        {
            istringstream  ins;
            //  extract from the string the vertex' X, Y, Z coordinates
            dataLine.erase(0, 3);
            ins.str(dataLine);
            ins >> u >> v;
            //  do something with that info
            
            //  read the next line.  Check whether we are done with vertices
            getline(myFile, dataLine);
            if ((dataLine[0]!='v') || (dataLine[1]!='t'))
                readingTexturePts = false;

        }
    }
    
    //---------------------------------------------------------------------------
    //  Part 4:  Normal data [possibly empty]
    //---------------------------------------------------------------------------
    if ((dataLine[0] == 'v') && (dataLine[1] == 'n')) 
    {
        bool readingNormals = true;
        double   nx, ny, nz;
        while (readingNormals)
        {
            istringstream  ins;
            //  extract from the string the vertex' X, Y, Z coordinates
            dataLine.erase(0, 3);
            ins.str(dataLine);
            ins >> nx >> ny >> nz;
            NormalVector *norm = new NormalVector;
            norm->nx = nx; norm->ny = ny; norm->nz = nz;
            part->addNormalVector(norm);
            
            //  read the next line.  Check whether we are done with vertices
            getline(myFile, dataLine);
            if ((dataLine[0]!='v') || (dataLine[1]!='n'))
                readingNormals = false;

        }
    }

    //---------------------------------------------------------------------------
    //  Part 5:  Group info
    //  ignore alll stuff untill we hit the list of vertices
    //---------------------------------------------------------------------------
    bool notFacesYet = true;
    while (notFacesYet) 
    {
        if ((dataLine[0]=='f') && (dataLine[1]==' '))
            notFacesYet = false;
        else
        {
            getline(myFile, dataLine);
            if ((dataLine[0]=='f') && (dataLine[1]==' '))
                notFacesYet = false;
        
        }
    }

    //---------------------------------------------------------------------------
    //  Part 6:  Read face info
    //---------------------------------------------------------------------------
    bool readingFaces = true;
    int faceIndex = 0;
//     int prevV = -1, firstV = -1;   // unused vars. -CJC
    while (readingFaces)
    {
        //  we are going to read data for a new face.  We create it first and
        //  add it to the part.
        VirtualFace3D *face = new VirtualFace3D();
        vector<VirtualPoint3D *> *vertices = part->getVertices();
        vector<VirtualPoint3D *>::iterator vertex = vertices->begin();
        part->addFace(face);

        istringstream  ins;
        dataLine.erase(0, 2);
        ins.str(dataLine);

        //  this is really a hack.  I use spaces to separate the tokens
        bool moreVertsInFace = true;
//         bool zeEnd = false; // unused var. -CJC
        int prevV = -1, firstV = -1;
        while (moreVertsInFace)
        {
            //  find index of next space
            int nextSpace = dataLine.find(" ", 0);
            
            if (nextSpace == static_cast<int>(string::npos))
            {
                moreVertsInFace = false;
                dataLine = dataLine + " ";  // ugly
                nextSpace = dataLine.size();
            }
                
            string tokenStr = dataLine.substr(0, nextSpace);
            dataLine.erase(0, nextSpace+1);
            
            //  get the vertex index;
            int slash1 = tokenStr.find("/", 0);
            string vS = tokenStr.substr(0, slash1);
            const char* vC = vS.c_str();
            int v = atoi(vC) -1;
            if (prevV == -1)
            prevV = firstV = v;
            else{
            	vertex[v]->addNeighbor(vertex[prevV]);
            	vertex[prevV]->addNeighbor(vertex[v]);
            	prevV = v;
            }
           
//                delete []vC;
            
            tokenStr.erase(0, slash1+1);

            //  get the texture point index
            int slash2 = tokenStr.find("/", 0);
            string tS = tokenStr.substr(0, slash2);
            const char* tC = tS.c_str();
            int t = atoi(tC) -1;
//                delete []tC;

            tokenStr.erase(0, slash2+1);

            //  get the texture point index
            const char* nC = tokenStr.c_str();
            int n = atoi(nC) -1;
//                delete []nC;  
            vertex[v]->addNeighbor(vertex[firstV]);
            vertex[firstV]->addNeighbor(vertex[v]);
            part->addFaceVertexInfo(faceIndex, v, t, n);      

        }
        //  read the next line.  Check whether we are done reading faces
        getline(myFile, dataLine);
        if ((dataLine[0]!='f') || (dataLine[1]!=' '))
            readingFaces = false;

        faceIndex++;
    }

    obj->addPart(part);
	return obj;
}
