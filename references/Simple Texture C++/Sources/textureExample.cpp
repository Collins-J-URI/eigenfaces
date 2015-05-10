#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cmath>
//
#include "glPlatform.h"
#include "fileIO_TGA.h"
#include "fileIO_PPM.h"

using namespace std;

typedef	struct sTextureInfoStruct {
    short	nbCols,
    nbRows,
    bytesPerPixel;
} sTextureInfo;

#define	kNbText				4
#define	kNbTextMapModes		2
#define	kNbObjectModes		4

#define	GLUPERSPECTIVE_VERSION	1
#define GLULOOKAT_VERSION		0

// This is the only thing I changed - Chris
//#error fix the paths
#define	kNameFile1	"/Users/Christopher/Desktop/Simple Texture C++/Images/bottles.tga"
#define	kNameFile2	"/Users/Christopher/Desktop/Simple Texture C++/Images/claypotsB.ppm"

int				gCurrentText = 0,	//	index of texture used for display
gTextureMode = 1,
gObjectMode = 1,
gTextMapID = 0;
GLfloat			gTheta = 35.f;
//
unsigned char	**gMyData;			//	pointers for the textures seen as characters (3 * 1 byte)
GLuint	 		gMyTextID[kNbText];	//	identifiers for the textures created
sTextureInfo	gMyTextInfo[kNbText];
//
//	First I define the vertices for my
GLfloat			gCubeVertex[8][3] = {	{-1.f, -1.f, -1.f},
    { 1.f, -1.f, -1.f},
    { 1.f,  1.f, -1.f},
    {-1.f,  1.f, -1.f},
    {-1.f, -1.f,  1.f},
    { 1.f, -1.f,  1.f},
    { 1.f,  1.f,  1.f},
    {-1.f,  1.f,  1.f}
},
//
gPolyVertex[7][3] = {	{0.f, -1.2f, 0.0f},
    {1.1f, -.4f, 0.0f},
    {1.3f, 0.7f, 0.0f},
    {0.3f, 1.2f, 0.0f},
    {-.8f, 0.6f, 0.0f},
    {-.9f, -.8f, 0.0f},
    {-.5f, -1.2f, 0.0f}
},
//
gCubeTextMap[2][4][2] = {	{{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}},
    {{.8f, 0.f}, {1.f, 1.f}, {0.f, .5f}, {0.f, 0.f}}
},
//
gCubeTextWrap[14][2] =	{	{0.f, 1.f}, {.25f, 1.f}, {0.f, .75f},
    {.25f, .67f}, {.5f, .67f}, {.75f, .67f},
    {1.f, .67f}, {0.f, .33f}, {.25f, .33f},
    {.5f, .33f},  {.75f, .33f}, {1.f, .33f},
    {0.f, 0.f}, {.25f, 0.f}
},
//
gPolyTextMap[2][7][2] = {	{
    {.1f, .5f}, {.5f, .2f}, {.8f, .4f}, {.8f, .5f},
    {.6f, .8f}, {.4f, .9f}, {.2f, .9f}
},
    {
        {.6f, .8f}, {.1f, .8f}, {.1f, .5f}, {.4f, .2f},
        {.6f, .2f}, {.8f, .4f}, {.8f, .6f}
    }
};

GLfloat		worldX = 0.f, worldY = 0, worldZ = -5.f;
GLfloat		rotX = 0.f, rotY = 0.f, rotZ = 0.f;


int loadGLTextures(void);
void createTextures(void);
void myDisplay(void);
void myResize(int w, int h);
void myKeyboard(unsigned char c, int x, int y);
int myInit(void);
void myTimeOut(int dt);
void displayCube(void);
void displayPoly(void);


void createTextures(void)				// Loads A Bitmap Image
{
    long	idx;
    short	n;
    int		nbRows, nbCols;
    eImageFileType	imgType;
    
    gMyData = (unsigned char **) calloc(kNbText, sizeof(char *));
    
    //-------------------------------------------------------------
    //	Texture 0 is created by computation --> fill the array with RGB values
    //-------------------------------------------------------------
    n = gMyTextInfo[0].nbCols = gMyTextInfo[0].nbRows = 256;
    gMyTextInfo[0].bytesPerPixel = 3;
    //
    gMyData[0] = (unsigned char *) calloc(n * n * 3, sizeof(char));
    
    if (gMyData[0] !=NULL)
    {
        for (short i=0; i<n; i++)
        {
            idx = 3 * n * i;
            
            for (short j=0; j<n; j++)
            {
                gMyData[0][idx++] = (unsigned char) i;
                gMyData[0][idx++] = (unsigned char) 255-j;
                gMyData[0][idx++] = (unsigned char) 0;
            }
        }
    }
    else
    {
        cout << "Generation of texture 0 failed.";
        std::exit(-1);
    }
    
    //--------------------------------------------------------------------------
    //	Texture 1 is created by computation --> fill the array with RGBA values
    //--------------------------------------------------------------------------
    n = gMyTextInfo[1].nbCols = gMyTextInfo[1].nbRows = 256;
    gMyTextInfo[1].bytesPerPixel = 4;
    //
    gMyData[1] = (unsigned char *) calloc(n * n * 4, sizeof(char));
    
    if (gMyData[1] !=NULL)
    {
        for (short i=0; i<n; i++)
        {
            idx = 4 * n * i;
            
            for (short j=0; j<n; j++)
            {
                gMyData[1][idx++] = (unsigned char) floor((i-n/2.)*(i-n/2.)/120. + 20);
                gMyData[1][idx++] = (unsigned char) j/4 + 30;
                gMyData[1][idx++] = (unsigned char) floor(127*(cos(0.06*i)*sin(0.1*j)+0.5));
                gMyData[1][idx++] = (unsigned char) 255;
                
            }
        }
    }
    else
    {
        cout << "Generation of texture 0 failed.";
        std::exit(-1);
    }
    
    //-------------------------------------------------------------
    //	Texture 2 is read from a TGA file
    //-------------------------------------------------------------
    gMyData[2] = readTGA(kNameFile1, &nbRows, &nbCols, &imgType);
    
    if (gMyData[2] !=NULL)
    {
        gMyTextInfo[2].nbCols = nbCols;
        gMyTextInfo[2].nbRows = nbRows;
        if (imgType == kTGA_COLOR)
            gMyTextInfo[2].bytesPerPixel = 3;
        else
            gMyTextInfo[2].bytesPerPixel = 1;
        
    }
    else
    {
        cout << "Generation of texture 2 failed.";
        std::exit(-1);
    }
    
    //-------------------------------------------------------------
    //	Texture 3 is read from a PPM file
    //-------------------------------------------------------------
    gMyData[3] = readPPM(kNameFile2, &nbRows, &nbCols, &imgType);
    
    if (gMyData[3] !=NULL)
    {
        gMyTextInfo[3].nbCols = nbCols;
        gMyTextInfo[3].nbRows = nbRows;
        if (imgType == kPPM)
            gMyTextInfo[3].bytesPerPixel = 3;
    }
    else
    {
        cout << "Generation of texture 3 failed.";
        std::exit(-1);
    }
}



int loadGLTextures(void)				// Load Bitmaps And Convert To Textures
{
    int Status=false;
    
    createTextures();
    
    // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
    if (gMyData!=NULL)
    {
        Status=true;									// Set The Status To TRUE
        
        for (int i=0; i<kNbText; i++)
        {
            glGenTextures(1, gMyTextID + i);			// gets a free identificator for the texture
            
            // Typical Texture Generation Using Data From The Bitmap
            glBindTexture(GL_TEXTURE_2D, gMyTextID[i]);
            
            switch (gMyTextInfo[i].bytesPerPixel)
            {
                case 3:
                    glTexImage2D(GL_TEXTURE_2D, 0, 	gMyTextInfo[i].bytesPerPixel,
                                 gMyTextInfo[i].nbCols,
                                 gMyTextInfo[i].nbRows,
                                 0,
                                 GL_RGB, GL_UNSIGNED_BYTE,
                                 gMyData[i]);
                    break;
                    
                case 4:
                    glTexImage2D(GL_TEXTURE_2D, 0, 	gMyTextInfo[i].bytesPerPixel,
                                 gMyTextInfo[i].nbCols,
                                 gMyTextInfo[i].nbRows,
                                 0,
                                 GL_RGBA, GL_UNSIGNED_BYTE,
                                 gMyData[i]);
                    break;
                    
                default:
                    std::exit(-1);
                    break;
            }
            
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            free(gMyData[i]);					// Free The Texture Image Memory
            gMyData[i] = NULL;
        }
    }
    
    return Status;										// Return The Status
}

//---------------------------------------------------------------------------------
//	myDisplay()
//		See pdf file for explanations on the different drawing modes.
//---------------------------------------------------------------------------------

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glRotatef(-rotX, 1.f, 0.f, 0.f);
    glRotatef(-rotY, 0.f, 1.f, 0.f);
    glRotatef(-rotZ, 0.f, 0.f, 1.f);
    
#if GLUPERSPECTIVE_VERSION
    glTranslatef(worldX, worldY, worldZ);
#else
    gluLookAt(0, 0, 0, worldX, worldY, worldZ, 0., 1., 0.);
#endif
    
    if ((gObjectMode == 1) || (gObjectMode == 3))
        displayCube();
    
    if ((gObjectMode == 2) || (gObjectMode == 3))
        displayPoly();
    
    glutSwapBuffers();
}

void displayCube(void)
{
    glPushMatrix();
    
#if GLULOOKAT_VERSION
    glTranslatef(worldX, worldY, worldZ);
#endif
    
    glRotatef(gTheta,1.0,1.0,1.0);
    
    switch (gTextureMode)
    {
        case 1:
            glBindTexture(GL_TEXTURE_2D, gMyTextID[gCurrentText]);		// Select Texture
            glBegin(GL_QUADS);
            // Front Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[4]);
            // Back Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[7]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[6]);
            // Top Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[7]);
            // Bottom Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[1]);
            // Right face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[5]);
            // Left Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[7]);
            glEnd();
            break;
            
        case 2:
            glBindTexture(GL_TEXTURE_2D, gMyTextID[gCurrentText]);		// Select Texture
            glBegin(GL_QUADS);
            // Front Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[4]);
            // Back Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[7]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[6]);
            // Top Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[7]);
            glEnd();
            //
            glBindTexture(GL_TEXTURE_2D, gMyTextID[(gCurrentText+1)%kNbText]);	// Select Texture
            glBegin(GL_QUADS);
            // Bottom Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[1]);
            // Right face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[5]);
            // Left Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[7]);
            glEnd();
            break;
            
        case 3:
            glBindTexture(GL_TEXTURE_2D, gMyTextID[gCurrentText]);		// Select Texture
            glBegin(GL_QUADS);
            // Front Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[4]);
            // Back Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[7]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[6]);
            glEnd();
            //
            glBindTexture(GL_TEXTURE_2D, gMyTextID[(gCurrentText+1)%kNbText]);	// Select Texture
            glBegin(GL_QUADS);
            // Top Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[7]);
            // Bottom Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[1]);
            glEnd();
            //
            glBindTexture(GL_TEXTURE_2D, gMyTextID[(gCurrentText+2)%kNbText]);	// Select Texture
            glBegin(GL_QUADS);
            // Right face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[5]);
            // Left Face
            glTexCoord2fv(gCubeTextMap[gTextMapID][0]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][1]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][2]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextMap[gTextMapID][3]); glVertex3fv(gCubeVertex[7]);
            glEnd();
            break;
            
        case 4:
            glBindTexture(GL_TEXTURE_2D, gMyTextID[gCurrentText]);		// Select Texture
            glBegin(GL_QUADS);
            // Front Face
            glTexCoord2fv(gCubeTextWrap[7]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextWrap[8]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextWrap[3]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextWrap[2]); glVertex3fv(gCubeVertex[4]);
            // Back Face
            glTexCoord2fv(gCubeTextWrap[9]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextWrap[10]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextWrap[5]); glVertex3fv(gCubeVertex[7]);
            glTexCoord2fv(gCubeTextWrap[4]); glVertex3fv(gCubeVertex[6]);
            // Top Face
            glTexCoord2fv(gCubeTextWrap[2]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextWrap[3]); glVertex3fv(gCubeVertex[5]);
            glTexCoord2fv(gCubeTextWrap[1]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextWrap[0]); glVertex3fv(gCubeVertex[7]);
            // Bottom Face
            glTexCoord2fv(gCubeTextWrap[7]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextWrap[12]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextWrap[13]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextWrap[8]); glVertex3fv(gCubeVertex[1]);
            // Right face
            glTexCoord2fv(gCubeTextWrap[8]); glVertex3fv(gCubeVertex[1]);
            glTexCoord2fv(gCubeTextWrap[9]); glVertex3fv(gCubeVertex[2]);
            glTexCoord2fv(gCubeTextWrap[4]); glVertex3fv(gCubeVertex[6]);
            glTexCoord2fv(gCubeTextWrap[3]); glVertex3fv(gCubeVertex[5]);
            // Left Face
            glTexCoord2fv(gCubeTextWrap[10]); glVertex3fv(gCubeVertex[3]);
            glTexCoord2fv(gCubeTextWrap[11]); glVertex3fv(gCubeVertex[0]);
            glTexCoord2fv(gCubeTextWrap[6]); glVertex3fv(gCubeVertex[4]);
            glTexCoord2fv(gCubeTextWrap[5]); glVertex3fv(gCubeVertex[7]);
            glEnd();
            break;
            
        default:
            break;
            
    }
    
    glPopMatrix();
}


void displayPoly(void)
{
    glPushMatrix();
    
    glTranslatef(0.55, -0.25, -4.0);
    glRotatef(gTheta, 1.0, 0.0, 1.0);
    
    glBindTexture(GL_TEXTURE_2D, gMyTextID[gCurrentText]);		// Select Texture
    glBegin(GL_POLYGON);
    for (int i=0; i<7; i++)
    {
        glTexCoord2fv(gPolyTextMap[gTextMapID][i]); glVertex3fv(gPolyVertex[i]);
    }
    glEnd();
    
    glPopMatrix();
}



void myResize(int w, int h)
{
    //	update projection transformations
    glViewport(0, 0, w, h);
    
    //	When it's done, request a refresh of the display
    glutPostRedisplay();
}

void myTimeOut(int dt)
{
    //	Here I could do something to update the state of my scene (in an animation)
    gTheta -= 0.5f;
    
    //	And finally I perform the rendering
    glutPostRedisplay();
    
    //	Set up next timer event
    glutTimerFunc(20, myTimeOut, 0);
}



void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
            //		case 'q':
            //			std::exit(0);
            //			break;
            
            //	[ and ] let me cycle through the texture maps
        case '[':
            gCurrentText = (gCurrentText+kNbText-1) % kNbText;
            break;
        case ']':
            gCurrentText = (gCurrentText + 1) % kNbText;
            break;
            
            //	1, 2, 3, 4 let me choose the texture mode (1, 2, or 3 textures used for
            //	the display; 4 wraps the texture around the cube).
        case '1':
        case '2':
        case '3':
        case '4':
            gTextureMode = (int) (c - '0');
            break;
            
            //	, and . (keyboard < and >) let me cycle through the
            //	different object modes.
        case ',':
            gObjectMode = (gObjectMode - 1) % kNbObjectModes + 1;
            break;
        case '.':
            gObjectMode = (gObjectMode + 1) % kNbObjectModes + 1;
            break;
            
            //	; and ' let me cycle through the
            //	different texture mapping modes.
        case ';':
            gTextMapID = (gTextMapID + kNbTextMapModes -1) % kNbTextMapModes;
            break;
            
        case '\'':
            gTextMapID = (gTextMapID + 1) % kNbTextMapModes;
            break;
            
        case 'w':
            worldZ += 0.05f;
            break;
        case 's':
            worldZ -= 0.05f;
            break;
            
        case 'a':
            worldX += 0.05f;
            break;
        case 'd':
            worldX -= 0.05f;
            break;
            
        case '-':
            worldY += 0.05f;
            break;
        case '+':
            worldY -= 0.05f;
            break;
            
        case 'q':
            rotX += 0.5f;
            break;
        case 'e':
            rotX -= 0.5f;
            break;
            
        case 'z':
            rotY += 0.5f;
            break;
        case 'x':
            rotY -= 0.5f;
            break;
            
        case 'c':
            rotZ += 0.5f;
            break;
        case 'v':
            rotZ -= 0.5f;
            break;
            
        default:
            break;
            
    }
}


int myInit(void)
{
    //	If textures didn't load, exit.
    if (!loadGLTextures())					
        std::exit(-1);							
    
    glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping	
    
    glShadeModel(GL_SMOOTH);						// Enables Smooth Shading
    
    glClearColor(0.0,0.0,0.0,1.0);
    
    glClearDepth(1.0f);								// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Nice Perspective Calculations
    
    
    glMatrixMode(GL_PROJECTION);
    
    gluPerspective( /* field of view in degrees */ 40.0,
                   /* aspect ratio */ 1.0,
                   /* Z near */ 1.0, /* Z far */ 10.0);
    
    glMatrixMode(GL_MODELVIEW);
    
    return false;
    
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(300,300);
    glutCreateWindow("Texture Demo");
    
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myResize);
    glutTimerFunc(20, myTimeOut, 0);
    glutKeyboardFunc(myKeyboard);
    myInit();
    
    glutMainLoop();
    
    return 0;
}