/*  NAME:
        simpleIO.cpp

    DESCRIPTION:
        demonstration application for basic image I/O operations

    COPYRIGHT:
        (c) 2003-2007, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 2.1
        For more information see http://opensource.org/licenses/afl-2.1.php
*/

/*--------------------------------------------------------------------------+
|	This small demo program is recycled from code I wrote for the computer	|
|	graphics course to show how to do texture mapping.						|
|	In the original program, the images were read from files or simply		|
|	generated to be mapped as textures on a graphic object.					|
|	In this revised version, they are now displayed in a glut window.		|
|																			|
|	The important point to keep in mind is that in C/C++ our image is		|
|	a *1D* array of unsigned char.  If you want to manipulate your image	|
|	as a 2D array, you will have to create arrays for your row pointers.	|
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
+--------------------------------------------------------------------------*/

#include <iostream>
#include <cmath>

#include "ImageReader.h"
#include "LabelRaster.h"
#include "ImageWriter.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RandomGenerator_C.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
#define	kNbImages				9
//
const string color_dir = COLOR_DIR_PATH;

const string kNameFile = color_dir + "mandrill_256x256.jpg";


//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImage(void);
void myDisplay(void);
void displayTextualInfo(const char* infoStr);
void myReshape(int w, int h);
void myMenu(int value);
void myMouse(int button, int state, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
void resetCells(void);
void myInit(void);
void updateState(int val);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
RasterImage_RGBa*	gInputImage = NULL;
RasterImage_RGBa*	gOutputImage = NULL;
LabelRaster*		gLabelOld = NULL;
LabelRaster*		gLabelNew = NULL;

bool	gInitDone = false,
		gRunningLife = false;

typedef enum GlutMenuSelect {	QUIT_MENU = 0,
								RESET_MENU,
								SEPARATOR_MENU,
								//
								LIFE_B23_S3 = 10,
								CORAL_B3_S45678,
								MAZE_B3_S12345,
								ASSIMILATION_B345_S4567,
								AMOEBA_B357_S1358,
								SEEDS_B2_
} GlutMenuSelect;

const string RULE_STR[] = {	"Life: B23/S3",				//	LIFE_B23_S3,
							"Coral: B3_S45678",			//	CORAL_B3_S45678,
							"MAZE_B3_S12345",			//	MAZE_B3_S12345,
							"Assimilation: B345/S4567",	//	ASSIMILATION_B345_S4567,
							"Amoeba: B357/S1358",		//	AMOEBA_B357_S1358,
							"Seeds: B2/"				//	SEEDS_B2_
};

//	10 hard-coded rates
const int NB_RANDOM_RATES = 10;
const float RANDOM_RATE[] = {	0.01f,		//	0
								0.02f,		//	1
								0.05f,		//	2
								0.1f,		//	3
								0.2f,		//	4
								0.3f,		//	5
								0.4f,		//	6
								0.5f,		//	7
								0.6f,		//	8
								0.7f};		//	9
int gRandomRateIndex = 6;


GlutMenuSelect gRule = LIFE_B23_S3;
float gUt = 0.f;

const unsigned int LOWER_BOUND_1_DT = 10;
const unsigned int UPPER_BOUND_1_DT = 100;
const unsigned int LOWER_BOUND_2_DT = UPPER_BOUND_1_DT;
const unsigned int UPPER_BOUND_2_DT = 1000;
const unsigned int LOWER_BOUND_3_DT = UPPER_BOUND_2_DT;
const unsigned int UPPER_BOUND_3_DT = 10000;
const unsigned int INCREMENT_1_DT = 10;
const unsigned int INCREMENT_2_DT = 100;
const unsigned int INCREMENT_3_DT = 1000;

float gDt = 1000;

#define DISPLAY_FONT    GLUT_BITMAP_HELVETICA_12
const int FONT_HEIGHT = 12;
const int TEXT_PADDING = 2;
const float kTextColor[3] = {1.f, 1.f, 1.f},
            kTextBackgroundColor[3] = {0.0f, 0.2f, 0.0f};



//=============================================================================
//	Function Implementations
//=============================================================================

void initializeQTEnvironment(void)
{
    #if URIVL_USE_QUICKTIME
        #if	URIVL_OS_WIN32
            OSErr	anErr = noErr;
            anErr = InitializeQTML(kQTMLNoIdleEvents);

            if (anErr != noErr)
            {
                cerr <<"\pQuickTime is not installed on this computer";
                ExitToShell();
            }


            anErr = EnterMovies();
            if(anErr != noErr)
            {
                cerr <<"\pProblems with EnterMovies, returning errors (exit)";
                ExitToShell();
            }
        #endif
    #endif
}

void cleanupAndQuit(int exitCode)
{
    #if URIVL_USE_QUICKTIME
        // terminate the QuickTime Media Layer
        ExitMovies();

        #if URIVL_OS_WIN32
            TerminateQTML();
        #endif

    	#if __profile__
    		ProfilerDump("\pProfilerData");
    		ProfilerTerm();
    	#endif
    #endif

    std::exit(exitCode);
}

void loadImage(void)
{
    //-------------------------------------------------------------
    //	Read image from a file
    //-------------------------------------------------------------
    //  I can create a reader by passing a file path
    ImageReader myReader(kNameFile.c_str());
    gInputImage = (RasterImage_RGBa*) myReader.getFrame(true);
	gOutputImage = new RasterImage_RGBa(gInputImage);
	
	gLabelOld = new LabelRaster(gInputImage->getHeight(), gInputImage->getWidth());
	gLabelNew = new LabelRaster(gInputImage->getHeight(), gInputImage->getWidth());

    if (gInputImage ==NULL)
    {
        cout << "Loading of image failed.";
        cleanupAndQuit(-1);
    }

	const int nbRows = gInputImage->getHeight();
	const int nbCols = gInputImage->getWidth();
	char *const* oldL = gLabelOld->getLabel2D(R_W_ACCESS);
	char *const* newL = gLabelNew->getLabel2D(R_W_ACCESS);

	for (int i=0; i<nbRows; i++)
	{
		for (int j=0; j<nbCols; j++)
		{
			oldL[i][j] = newL[i][j] = static_cast<char>(1);
		}
	}
}



void updateState(int val)
{
	if (gRunningLife)
	{
		const int nbRows = gInputImage->getHeight();
		const int nbCols = gInputImage->getWidth();
		char *const* newL = gLabelNew->getLabel2D(R_W_ACCESS);
		const char *const* oldL = gLabelOld->getLabel2D();
		const unsigned char *const* rgbaIn = gInputImage->getRaster2D();
		unsigned char *const* rgbaOut = gOutputImage->getRaster2D(R_W_ACCESS);
		
		//	apply the rule
		switch (gRule)
		{
			case LIFE_B23_S3:
				for (int i=1; i<nbRows-1; i++)
				{
					for (int j=1; j<nbCols-1; j++)
					{
						int count = oldL[i-1][j-1] + oldL[i-1][j] + oldL[i-1][j+1] +
									oldL[i][j-1] + oldL[i][j+1] +
									oldL[i+1][j-1] + oldL[i+1][j] + oldL[i+1][j+1];
						
						//	Birth rule
						if (count == 3)
						{
							newL[i][j] = static_cast<char>(1);
						}
						//	Survival rule
						else if (oldL[i][j] && 
								 ((count == 2) || (count == 3)))
						{
							newL[i][j] = static_cast<char>(1);					
						}
						else
						{
							newL[i][j] = static_cast<char>(0);					
						}
					}
				}
				break;
			

			case CORAL_B3_S45678:
				for (int i=1; i<nbRows-1; i++)
				{
					for (int j=1; j<nbCols-1; j++)
					{
						int count = oldL[i-1][j-1] + oldL[i-1][j] + oldL[i-1][j+1] +
									oldL[i][j-1] + oldL[i][j+1] +
									oldL[i+1][j-1] + oldL[i+1][j] + oldL[i+1][j+1];
						
						//	Birth rule
						if (count == 3)
						{
							newL[i][j] = static_cast<char>(1);
						}
						//	Survival rule
						else if (oldL[i][j] && 
								 ((count == 4) || (count == 5) || (count == 6) || (count == 7) || (count == 8)))
						{
							newL[i][j] = static_cast<char>(1);					
						}
						else
						{
							newL[i][j] = static_cast<char>(0);					
						}
					}
				}
				break;

			
			case MAZE_B3_S12345:
				for (int i=1; i<nbRows-1; i++)
				{
					for (int j=1; j<nbCols-1; j++)
					{
						int count = oldL[i-1][j-1] + oldL[i-1][j] + oldL[i-1][j+1] +
									oldL[i][j-1] + oldL[i][j+1] +
									oldL[i+1][j-1] + oldL[i+1][j] + oldL[i+1][j+1];
						
						//	Birth rule
						if (count == 3)
						{
							newL[i][j] = static_cast<char>(1);
						}
						//	Survival rule
						else if (oldL[i][j] && 
								 ((count == 1) || (count == 2) || (count == 3) || (count == 4) || (count == 5)))
						{
							newL[i][j] = static_cast<char>(1);					
						}
						else
						{
							newL[i][j] = static_cast<char>(0);					
						}
					}
				}
				break;
			
			case ASSIMILATION_B345_S4567:
				for (int i=1; i<nbRows-1; i++)
				{
					for (int j=1; j<nbCols-1; j++)
					{
						int count = oldL[i-1][j-1] + oldL[i-1][j] + oldL[i-1][j+1] +
									oldL[i][j-1] + oldL[i][j+1] +
									oldL[i+1][j-1] + oldL[i+1][j] + oldL[i+1][j+1];
						
						//	Birth rule
						if ((count == 3) || (count == 4) || (count == 5))
						{
							newL[i][j] = static_cast<char>(1);
						}
						//	Survival rule
						else if (oldL[i][j] && 
								 ((count == 4) || (count == 5) || (count == 6) || (count == 7)))
						{
							newL[i][j] = static_cast<char>(1);					
						}
						else
						{
							newL[i][j] = static_cast<char>(0);					
						}
					}
				}
				break;

			case AMOEBA_B357_S1358:
				for (int i=1; i<nbRows-1; i++)
				{
					for (int j=1; j<nbCols-1; j++)
					{
						int count = oldL[i-1][j-1] + oldL[i-1][j] + oldL[i-1][j+1] +
									oldL[i][j-1] + oldL[i][j+1] +
									oldL[i+1][j-1] + oldL[i+1][j] + oldL[i+1][j+1];
						
						//	Birth rule
						if ((count == 3) || (count == 5) || (count == 7))
						{
							newL[i][j] = static_cast<char>(1);
						}
						//	Survival rule
						else if (oldL[i][j] && 
								 ((count == 1) || (count == 3) || (count == 5) || (count == 8)))
						{
							newL[i][j] = static_cast<char>(1);					
						}
						else
						{
							newL[i][j] = static_cast<char>(0);					
						}
					}
				}
				break;

			case SEEDS_B2_:
				for (int i=1; i<nbRows-1; i++)
				{
					for (int j=1; j<nbCols-1; j++)
					{
						int count = oldL[i-1][j-1] + oldL[i-1][j] + oldL[i-1][j+1] +
									oldL[i][j-1] + oldL[i][j+1] +
									oldL[i+1][j-1] + oldL[i+1][j] + oldL[i+1][j+1];
						
						//	Birth rule
						if ((count == 2))
						{
							newL[i][j] = static_cast<char>(1);
						}
						//	Survival rule --> none
						//else if (oldL[i][j] && 
						//		 ((count == 1) || (count == 3) || (count == 5) || (count == 8)))
						//{
						//	newL[i][j] = static_cast<char>(1);					
						//}
						else
						{
							newL[i][j] = static_cast<char>(0);					
						}
					}
				}
				break;
				
			default:
				break;

		}

		//	Compute the new image
		for (int i=0; i<nbRows; i++)
		{
			for (int j=0; j<nbCols-1; j++)
			{
				if (newL[i][j] == 1)
				{
					rgbaOut[i][4*j] = rgbaIn[i][4*j];
					rgbaOut[i][4*j+1] = rgbaIn[i][4*j+1];
					rgbaOut[i][4*j+2] = rgbaIn[i][4*j+2];
					rgbaOut[i][4*j+3] = static_cast<unsigned char>(255);
				}
				else 
				{
					rgbaOut[i][4*j] = static_cast<unsigned char>(0);
					rgbaOut[i][4*j+1] = static_cast<unsigned char>(0);
					rgbaOut[i][4*j+2] = static_cast<unsigned char>(0);
					rgbaOut[i][4*j+3] = static_cast<unsigned char>(255);
				}

			}
		}
		
		//	swap old and new
		{
			LabelRaster* tempLabel = gLabelNew;
			gLabelNew = gLabelOld;
			gLabelOld=  tempLabel;
		}
	}
	
    glutPostRedisplay();

	glutTimerFunc(gDt, updateState, 0);
}


void resetCells(void)
{
	gRunningLife = false;
	
	const int nbRows = gInputImage->getHeight();
	const int nbCols = gInputImage->getWidth();
	char *const* oldL = gLabelOld->getLabel2D(R_W_ACCESS);
	char *const* newL = gLabelNew->getLabel2D(R_W_ACCESS);

	for (int i=0; i<nbRows; i++)
	{
		for (int j=0; j<nbCols; j++)
		{
			float val = RandomGenerator_C::nextFloat_st(0.f, 1.f);
			oldL[i][j] = newL[i][j] = static_cast<char>((val < RANDOM_RATE[gRandomRateIndex] ? 1 : 0));
		}
	}
	
		gRunningLife = true;
}

//---------------------------------------------------------------------------------
//	myDisplay()
//		Simply renders the "current" image in the window (using openGL functions)
//---------------------------------------------------------------------------------

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
        gOutputImage->render( );

    //  Build the string to display
	char infoStr[256];
	sprintf(infoStr, "Rule %s, Gen. Prob. =  %f, Anim. period = %d ms", 
							RULE_STR[gRule-10].c_str(), RANDOM_RATE[gRandomRateIndex],
							static_cast<int>(gDt));	
	
	displayTextualInfo(infoStr);

    glutSwapBuffers();
}

void displayTextualInfo(const char* infoStr)
{
    //-----------------------------------------------
    //  0.  get current material properties
    //-----------------------------------------------
    float oldAmb[4], oldDif[4], oldSpec[4], oldShiny;
    glGetMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &oldShiny);

    glPushMatrix();

    //-----------------------------------------------
    //  1.  Build the string to display <-- parameter
    //-----------------------------------------------
    int infoLn = strlen(infoStr);

                          
    //-----------------------------------------------
    //  2.  Determine the string's length (in pixels)
    //-----------------------------------------------
    int textWidth = 0;
    for (int k=0; k<infoLn; k++)
        textWidth += glutBitmapWidth(DISPLAY_FONT, infoStr[k]);
    //  add a few pixels of padding
    textWidth += 2*TEXT_PADDING;

    //-----------------------------------------------
    //  3.  Clear background rectangle
    //-----------------------------------------------
	const int imgHeight = 3*gInputImage->getHeight()/2;
	const int imgWidth = 3*gInputImage->getWidth()/2;
	glColor3f(kTextBackgroundColor[0], kTextBackgroundColor[1], kTextBackgroundColor[2]);
    glBegin(GL_POLYGON);
        glVertex2i(0, imgHeight);
        glVertex2i(imgWidth, imgHeight);
        glVertex2i(imgWidth, imgHeight - FONT_HEIGHT - 4);
        glVertex2i(0, imgHeight - FONT_HEIGHT - 4);
    glEnd();
	glTranslatef(0.f, 0.f, 1.f);
    
    //-----------------------------------------------
    //  4.  Draw the string
    //-----------------------------------------------    
    glColor3f(kTextColor[0], kTextColor[1], kTextColor[2]);
    int xPos = TEXT_PADDING,
        yPos = imgHeight - FONT_HEIGHT;
    for (int k=0; k<infoLn; k++)
    {
        glRasterPos2i(xPos, yPos);
        glutBitmapCharacter(DISPLAY_FONT, infoStr[k]);
        xPos += glutBitmapWidth(DISPLAY_FONT, infoStr[k]);
    }

    //-----------------------------------------------
    //  5.  Restore old material properties
    //-----------------------------------------------
	glMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, oldShiny);  
    
    //-----------------------------------------------
    //  6.  Restore reference frame
    //-----------------------------------------------
    glPopMatrix();
}


void myReshape(int w, int h)
{
    if (gOutputImage != NULL)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    //  and I inform drawable object of these dimensions (in case they want to
    //  get rendered "shifted")
//    DrawableObject2D::setDefaultWindowDimensions(gInputImage->getWidth(),
//                                                 gInputImage->getHeight());
    DrawableObject2D::setDefaultWindowDimensions(w, h);
}


void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

		//	run simulation faster: decrement period
        case '.':
        case '>':
			if ((gDt > LOWER_BOUND_1_DT) && (gDt <= UPPER_BOUND_1_DT))
				gDt -= INCREMENT_1_DT;
			else if ((gDt > LOWER_BOUND_2_DT) && (gDt <= UPPER_BOUND_2_DT))
				gDt -= INCREMENT_2_DT;
			else if (gDt < UPPER_BOUND_3_DT)
				gDt -= INCREMENT_3_DT;
			//
			if (gDt < LOWER_BOUND_1_DT)
				gDt = LOWER_BOUND_1_DT;
            break;

		//	run simulation slower: increment period
        case ',':
        case '<':
			if ((gDt >= LOWER_BOUND_1_DT) && (gDt < UPPER_BOUND_1_DT))
				gDt += INCREMENT_1_DT;
			else if ((gDt >= LOWER_BOUND_2_DT) && (gDt < UPPER_BOUND_2_DT))
				gDt += INCREMENT_2_DT;
			else if (gDt < UPPER_BOUND_3_DT)
				gDt += INCREMENT_3_DT;
			//
			if (gDt > UPPER_BOUND_3_DT)
				gDt = UPPER_BOUND_3_DT;
            break;

		case ' ':
			resetCells();
			break;
			
		case '[':
			if (gRandomRateIndex > 0)
				gRandomRateIndex--;
			break;
			
		case ']':
			if (gRandomRateIndex < NB_RANDOM_RATES-1)
				gRandomRateIndex++;
			break;

        default:
            break;

    }

    glutPostRedisplay();
}


void myMouse(int button, int state, int ix ,int iy)
{
    // if the left mouse button is clicked, do something
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
//        gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
//        glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth(),
//                           gMyImage[gCurrentImg]->getHeight());

    }

    //	...and then refresh the display
    glutPostRedisplay();
}



void myMenu(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

		case LIFE_B23_S3:
			gRule = LIFE_B23_S3;
			break;
			
		case CORAL_B3_S45678:
			gRule = CORAL_B3_S45678;
			break;
			
		case MAZE_B3_S12345:
			gRule = MAZE_B3_S12345;
			break;

		case ASSIMILATION_B345_S4567:
			gRule = ASSIMILATION_B345_S4567;
			break;

		case AMOEBA_B357_S1358:
			gRule = AMOEBA_B357_S1358;
			break;

		case SEEDS_B2_:
			gRule = SEEDS_B2_;
			break;

		default:
            break;

    }

    glutPostRedisplay();
}

void myInit(void)
{
    initializeQTEnvironment();

    loadImage();

    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Create Menus
    int menu, ruleSubmenu;

    //	Submenu for direct image selection
    //	This is to show that I can use the same function to handle all submenus
    //	as long as they have unique indexes.  Still, it is probably a good idea
    //	to break down menu handling into several manageable-size functions.
    ruleSubmenu = glutCreateMenu(myMenu);
    glutAddMenuEntry("Life: B3/S23", LIFE_B23_S3);
    glutAddMenuEntry("Coral: B3/S45678", CORAL_B3_S45678);
    glutAddMenuEntry("Maze: B3/S12345", MAZE_B3_S12345);
    glutAddMenuEntry("Assimilation: B345/S4567", ASSIMILATION_B345_S4567);
    glutAddMenuEntry("Amoeba: B357/S1358", AMOEBA_B357_S1358);
    glutAddMenuEntry("Seeds: B2/", SEEDS_B2_);

    // Main menu that the submenus are connected to
    menu = glutCreateMenu(myMenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Reset", RESET_MENU);
    glutAddMenuEntry("--", SEPARATOR_MENU);
    glutAddSubMenu("Rule", ruleSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    gInitDone = true;
}


int main(int argc, char** argv)
{
	try {

		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(10, 30);
		glutCreateWindow("Image IO & Display Demo");

		//  defines glut callback functions that will interrupt the glut loop
		glutDisplayFunc(myDisplay);
		glutReshapeFunc(myReshape);
		glutMouseFunc(myMouse);
		glutKeyboardFunc(myKeyboard);
		glutTimerFunc(gDt, updateState, 0);

		//  Initializes QuickTime and loads the images
		myInit();

		//  Having read the images, I can now resize my window to the appropriate dimensions
		gOutputImage->setRenderingScale(1.5f);
		glutReshapeWindow(3*gInputImage->getWidth()/2,
						  3*gInputImage->getHeight()/2);


		//  Enter infinite glut loop
		glutMainLoop();

		//  Normally this code should never be placed since the only way to leave the
		//  glut loop is to quit
		cleanupAndQuit(0);
	}
	catch (uriVL::ErrorReport & e) 
	{
		cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
		return 1;
	}

    return 0;
}
