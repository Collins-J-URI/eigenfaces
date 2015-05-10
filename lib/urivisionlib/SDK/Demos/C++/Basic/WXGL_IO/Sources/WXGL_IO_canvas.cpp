/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_canvas.cpp
// Purpose:     Source file for example wxWidgets and OpenGL demo application
//              This file contains all the canvas class functions
// Author:      Lewis Collier
// Created:     16 April 2012
// Copyright:   (c) 2012-2014, 3D Group for Interactive Visualization
//                        University of Rhode Island.
//
//        Licensed under the Academic Free License - v. 3.0
//        For more information see http://opensource.org/licenses/academic.php
//---------------------------------------------------------------------------
// Modification History
//---------------------------------------------------------------------------
// Modification on: 
// Modified by: 
// Modification:
//---------------------------------------------------------------------------
// Modification on: 
// Modified by: 
// Modification: 
/////////////////////////////////////////////////////////////////////////////

// START STANDARD wxWIDGETS STUFF
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#if wxUSE_HTML
#include "wx/textfile.h"
#include "wx/html/htmlwin.h"
#endif
#include "wx/stockitem.h"
// END STANDARD wxWIDGETS STUFF

// include URIVL headers as needed
#include "ImagePoint.h"
#include "ImageReader.h"
#include "ImageWriter.h"
#include "RasterImage.h"
#include "RasterImage_binary.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"

// base CVPD wxWidget / OpenGL includes for this application
#include "WXGL_IO_canvas.h"
#include "WXGL_IO_frame.h"

// Application specific includes
#include "WXGL_IO_App.h"

using namespace std;
using namespace uriVL;


BEGIN_EVENT_TABLE(WXGL_IO_Canvas, wxGLCanvas)
   EVT_MOUSE_EVENTS(WXGL_IO_Canvas::OnMouseEvent)
	EVT_CHAR(WXGL_IO_Canvas::OnCharEvent)
	EVT_SET_FOCUS(WXGL_IO_Canvas::OnFocusEvent)
	EVT_KILL_FOCUS(WXGL_IO_Canvas::OnKillFocusEvent)
	//EVT_ACTIVATE(WXGL_IO_Canvas::OnActivateEvent)
	//EVT_SHOW(WXGL_IO_Canvas::OnShowEvent)
	EVT_PAINT(WXGL_IO_Canvas::OnPaintEvent)
END_EVENT_TABLE()

WXGL_IO_Canvas::WXGL_IO_Canvas(wxWindow* parent,
										 const wxWindowID id,
										 const wxPoint& pos, 
										 const wxSize& size,
										 long style,
										 const wxString& name)
										 : wxGLCanvas(parent, id, pos, size, style, name)

// : wxScrolledWindow(parent, wxID_ANY, pos, size)
{
	pmtOriginalImage = pmtReadInImage = pmtDifferenceImage = 0;
	mulRows = 480;
	mulCols = 644;
	mulFileType = MENU_ID_EXT_BMP;
	mulRasterType = MENU_ID_IMAGE_RGBA;

	CreateTestImage();
}
void WXGL_IO_Canvas::CreateTestImage(void)
{
	wxString strExtension, strFileName;

// set up file stuff

	switch (mulFileType)
	{
		case MENU_ID_EXT_BMP:
			strExtension = ".BMP";
			break;

		case MENU_ID_EXT_JPG:
			strExtension = ".JPG";
			break;

		case MENU_ID_EXT_PNG:
			strExtension = ".PNG";
			break;

		case MENU_ID_EXT_TIF:
		default:
			mulFileType = MENU_ID_EXT_TIF; // just in case we got here by default :)
			strExtension = ".TIF";
			break;
	} // end switch over file type
	strFileName = "TestFile"+strExtension;

// set up image stuff

	if (pmtOriginalImage!=0)
	{
		delete pmtOriginalImage;
	}
	switch (mulRasterType)
	{
		case MENU_ID_IMAGE_BINARY:
			pmtOriginalImage = new RasterImage_binary(mulRows, mulCols);
			MakeFake_ImageBinary(pmtOriginalImage);
			break;

		case MENU_ID_IMAGE_GRAY:
			pmtOriginalImage   = new RasterImage_gray(mulRows, mulCols);
			MakeFake_ImageGS(pmtOriginalImage);
			break;

		case MENU_ID_IMAGE_GRAY_F:
			pmtOriginalImage   = new RasterImage_gray_F(mulRows, mulCols);
			MakeFake_ImageGSF(pmtOriginalImage);
			break;

		case MENU_ID_IMAGE_RGBA:
		default:
			mulRasterType = MENU_ID_IMAGE_RGBA; // just in case we got here by default :)
			pmtOriginalImage   = new RasterImage_RGB(mulRows, mulCols);
			MakeFake_ImageRGBa(pmtOriginalImage);
			break;	
	} // end switch over image type

	// write the image then read it back in
	ImageWriter *pmyWriter = new ImageWriter(strFileName.c_str());
	ImageReader *pmyReader = new ImageReader();

	pmyWriter->outputFrame(pmtOriginalImage,false);

	if (pmtReadInImage!=0)
	{
		delete pmtReadInImage;
	}
	pmtReadInImage = pmyReader->readImage(strFileName.c_str(),true);

	// FIXME :: YES, this is a memory leak since readImage keeps allocating new images
}

WXGL_IO_Canvas::~WXGL_IO_Canvas()
{

}

/*
Called when the main frame is closed
*/
bool WXGL_IO_Canvas::OnCloseCanvas()
{
    return true;
}

////// Redraw events 
void WXGL_IO_Canvas::OnFocusEvent(wxFocusEvent& event)
{
	UpdateCanvas();
}

void WXGL_IO_Canvas::OnKillFocusEvent(wxFocusEvent& event)
{
	UpdateCanvas();
}

void WXGL_IO_Canvas::OnActivateEvent(wxActivateEvent& event)
{
	UpdateCanvas();
	event.Skip();
}


void WXGL_IO_Canvas::OnShowEvent(wxShowEvent& event)
{
	UpdateCanvas();
}


void WXGL_IO_Canvas::OnPaintEvent(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	SetFocus();
	UpdateCanvas();
}

////// Mouse event handler

void WXGL_IO_Canvas::OnMouseEvent(wxMouseEvent& event)
{
    int mouseX = (int)event.GetX();
    int mouseY = (int)event.GetY();

    if (event.LeftDClick())
    {
    }
    else if (event.LeftDown())
    {
    }
    else if (event.LeftUp())
    {
 		DrawSquare(mouseX, mouseY);
    }
    else if (event.RightDown() && !event.LeftIsDown())
    {
    }
    else if (event.Dragging())
    {
    }

    if (!event.LeftIsDown())
    {
    }

	 // not doing UpdateCanvas so you can see how the mouse event handler works
	 // remove the example code and do what you want here
	 // then uncomment the UpdateCanvas() call below
	 // UpdateCanvas();
}

////// Keyboard event handler

void WXGL_IO_Canvas::OnCharEvent(wxKeyEvent& event)
{
	wxInt32 nCanvasWidth, nCanvasHeight;
	this->GetClientSize(&nCanvasWidth, &nCanvasHeight);
	wxFloat32 fAmt = 1.0f;
	wxInt32 nChar = event.GetKeyCode();

	switch (nChar)
	{
		case WXK_PAGEDOWN:
			// do nothing, just showing how to handle a special key
			break;
		case 'A':
		case 'a':
			// do nothing, just showing how to handle a letter key
			break;
		default:
			// ignore character
			break;
	} // End switch over input key char

	UpdateCanvas();
}


////// The actual workhorse function WXGL_IO_Canvas::UpdateCanvas()
//
// redraws all items as they exist based on user requested changes
void WXGL_IO_Canvas::UpdateCanvas(void)
{
	int nCanvasWidth, nCanvasHeight;
	this->GetClientSize(&nCanvasWidth, &nCanvasHeight);
	DrawableObject2D::setDefaultWindowDimensions(nCanvasWidth, nCanvasHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	//glViewport(nX, nCanvasHeight-nY-gMyImage->getHeight()/2, gMyImage->getWidth(), gMyImage->getHeight());
	
	ImagePoint *ptPoint1 = new ImagePoint(50,50);
	pmtOriginalImage->render(ptPoint1);

	ImagePoint *ptPoint2 = new ImagePoint(50+10+pmtOriginalImage->getWidth(),50);
	pmtReadInImage->render(ptPoint2);

	SwapBuffers();
	SetCurrent();
}

/////////////////////////////////////////////////////////////////////////////
// SAMPLE GL CODE
//
// DrawSquare toggles square color/location and background color
// SetUpCamera sets up for drawing on the canvas
/////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Canvas::DrawSquare(int nX, int nY)
{
	static int nClear = 0;

	int nCanvasWidth, nCanvasHeight;
	this->GetClientSize(&nCanvasWidth, &nCanvasHeight);

	if (nClear==0)
	{
	   glClearColor(0.0, 0.0, 0.0, 1.0);
		nClear = 1;
	}
	else
	{
	   glClearColor(192.0, 192.0, 192.0, 1.0);
		nClear = 0;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2i(nX, nCanvasHeight-(nY));
		glVertex2i(nX, nCanvasHeight-(nY+100));
		glVertex2i(nX+100, nCanvasHeight-(nY+100));
		glVertex2i(nX+100, nCanvasHeight-(nY));
	glEnd();

	SwapBuffers();
   SetCurrent();
}

void WXGL_IO_Canvas::DrawSquare(void)
{
	static int nClear = 0, nPos = 0;

	   SetCurrent();

		SetUpCamera();

		if (nClear==0)
		{
		   glClearColor(0.0, 0.0, 0.0, 1.0);
			nClear = 1;
		}
		else
		{
		   glClearColor(192.0, 192.0, 192.0, 1.0);
			nClear = 0;
			nPos = (nPos==0?1:0);
		}

		if (nPos==0)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glClear(GL_COLOR_BUFFER_BIT);

			glBegin(GL_LINE_LOOP);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2i(500, 500);
				glVertex2i(500, 600);
				glVertex2i(600, 600);
				glVertex2i(600, 500);
			glEnd();
		}
		else
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glClear(GL_COLOR_BUFFER_BIT);

			glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 1.0, 0.0);
				glVertex2i(100, 100);
				glVertex2i(100, 200);
				glVertex2i(200, 200);
				glVertex2i(200, 100);
			glEnd();
		}

	//glFlush();
	SwapBuffers();
   SetCurrent();
}
void WXGL_IO_Canvas::SetUpCamera(void)
{
	// {fixme} this really needs to be size of canvas
	// but all I know is wxDisplaySize:)
	int nCanvasWidth, nCanvasHeight;
	wxDisplaySize(&nCanvasWidth, &nCanvasHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(	0.0f, (GLfloat) nCanvasWidth, 0.0f, (GLfloat) nCanvasHeight);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, nCanvasWidth, nCanvasHeight);
}
void WXGL_IO_Canvas::MakeFake_ImageRGBa(RasterImage *ptImage)
{
	bool boFillAlpha;
	int  nBytesPerPixel;

    //-------------------------------------------------------------
    //	Image 1 is created by computation --> fill the array with RGBA values
    //   This code fragment demonstrate how to get direct access to the image's
    //   raster for initialization and other IP calculations
	 //
	 //   This code was lifted from SimpleIO demo program
    //-------------------------------------------------------------

    //
    //  the raster image is created at the desired dimensions
    if (ptImage != NULL)
    {
		 // get base raster type so we know if to fill in alpha values
		 int nRasterType = ptImage->getBaseRasterType();
		 if (nRasterType==kRGB24Raster)
		 {
			 boFillAlpha=false;
			 nBytesPerPixel=3;
		 }
		 else if (nRasterType==kRGBa32Raster)
		 {
			 boFillAlpha=true;
			 nBytesPerPixel=4;
		 }
		 else if (nRasterType==kaRGB32Raster)
		 {
			 //boFillAlpha=true;
			 //nBytesPerPixel=4;
			 return; // since this class does not exist yet
		 }
		 else
		 {
			 return; // error
		 }

        //  we get a reference to the image's base raster
        //  Here, we use the version of the getRaster family of functions that return a
        //  read-write reference
        unsigned char *const* rgba = ptImage->getRaster2D(R_W_ACCESS);

        int   nbRows = ptImage->getHeight(),
              nbCols = ptImage->getWidth();
        //  from this point on
        for (int i=0; i<nbRows; i++)
		  {
            for (int j=0, l=0; j<nbCols; j++, l+=nBytesPerPixel)
            {
                rgba[i][l] = static_cast<unsigned char>(i/4);
                rgba[i][l+1] = static_cast<unsigned char>(j/4);
                rgba[i][l+2] = static_cast<unsigned char>(floor(127*(cos(0.06*i)*sin(0.1*j)+0.5)));
					 if (boFillAlpha)
					 {
						rgba[i][l+3] = static_cast<unsigned char>(255);
					 }
            }
		  }

        //	By default, the RasterImage class adopts the "safe" mode for the definition
        //	of the valid data rectangle.  Since image1 was just allocated not initialized,
        //	its valid data rectangle was located at the upper left corner of the bounding
        //	rectangle, with width and height 0.  We must now set the correct values.
        ptImage->setValidRect(0, 0, nbCols, nbRows);
    }
    else
    {
        printf("Generation of image 1 failed.");
    }

} // end of MakeFake_ImageRGBa

void WXGL_IO_Canvas::MakeFake_ImageGS(RasterImage *ptImage)
{
	int  nBytesPerPixel;

    //-------------------------------------------------------------
    //	Image 1 is created by computation --> fill the array with GS values
    //   This code fragment demonstrate how to get direct access to the image's
    //   raster for initialization and other IP calculations
	 //
	 //   This code was lifted from SimpleIO demo program
    //-------------------------------------------------------------

    //
    //  the raster image is created at the desired dimensions
    if (ptImage != NULL)
    {
		 // get base raster type so we know if to fill in alpha values
		 int nRasterType = ptImage->getBaseRasterType();
		 if (nRasterType==kBinaryRaster)
		 {
			 nBytesPerPixel=1;
		 }
		 else if (nRasterType==kGrayRaster)
		 {
			 nBytesPerPixel=1;
		 }
		 else if (nRasterType==kLongGrayRaster)
		 {
			 nBytesPerPixel=2;
			 return; // since this class does not exist yet
		 }
		 else
		 {
			 return; // error
		 }

        //  we get a reference to the image's base raster
        //  Here, we use the version of the getRaster family of functions that return a
        //  read-write reference
        unsigned char *const* rgba = ptImage->getRaster2D(R_W_ACCESS);

        int   nbRows = ptImage->getHeight(),
              nbCols = ptImage->getWidth();
        //  from this point on
        for (int i=0; i<nbRows; i++)
		  {
            for (int j=0, l=0; j<nbCols; j++, l+=nBytesPerPixel)
            {
					 unsigned char ubTemp[3];
                ubTemp[0] = static_cast<unsigned char>(i/4);
                ubTemp[1] = static_cast<unsigned char>(j/4);
                ubTemp[2] = static_cast<unsigned char>(floor(127*(cos(0.06*i)*sin(0.1*j)+0.5)));
					 unsigned short uhTemp = ubTemp[0]/4 + ubTemp[1]/2 + ubTemp[2]/4;
					 if (nBytesPerPixel==1)
					 {
						rgba[i][l] = uhTemp;
					 }
					 else
					 {
						rgba[i][l] = uhTemp/256;
						rgba[i][l+1] = uhTemp&0xff;
					 }
            }
		  }

        //	By default, the RasterImage class adopts the "safe" mode for the definition
        //	of the valid data rectangle.  Since image1 was just allocated not initialized,
        //	its valid data rectangle was located at the upper left corner of the bounding
        //	rectangle, with width and height 0.  We must now set the correct values.
        ptImage->setValidRect(0, 0, nbCols, nbRows);
    }
    else
    {
        printf("Generation of image 1 failed.");
    }

} // end of MakeFake_ImageGS

void WXGL_IO_Canvas::MakeFake_ImageGSF(RasterImage *ptImage)
{
	int  nBytesPerPixel;

    //-------------------------------------------------------------
    //	Image 1 is created by computation --> fill the array with GS values
    //   This code fragment demonstrate how to get direct access to the image's
    //   raster for initialization and other IP calculations
	 //
	 //   This code was lifted from SimpleIO demo program
    //-------------------------------------------------------------

    //
    //  the raster image is created at the desired dimensions
    if (ptImage != NULL)
    {
		 // get base raster type so we know if to fill in alpha values
		 int nRasterType = ptImage->getBaseRasterType();
		 if ((nRasterType==kGrayRaster) && (ptImage->hasFloatRaster()))
		 {
			 nBytesPerPixel=4;
		 }
		 else
		 {
			 return; // error if not a gray float raster
		 }

        //  we get a reference to the image's base raster
        //  Here, we use the version of the getRaster family of functions that return a
        //  read-write reference
        float *pixels = ((RasterImage_gray_F*)ptImage)->getGrayF(R_W_ACCESS);
		  int nThisPixel = 0;

		  int   nbRows = ptImage->getHeight(),
              nbCols = ptImage->getWidth();
        //  from this point on
        for (int i=0; i<nbRows; i++)
		  {
            for (int j=0, l=0; j<nbCols; j++, l+=nBytesPerPixel)
            {
					 unsigned char ubTemp[3];
                ubTemp[0] = static_cast<unsigned char>(i/4);
                ubTemp[1] = static_cast<unsigned char>(j/4);
                ubTemp[2] = static_cast<unsigned char>(floor(127*(cos(0.06*i)*sin(0.1*j)+0.5)));
					 unsigned short uhTemp = ubTemp[0]/4 + ubTemp[1]/2 + ubTemp[2]/4;
					 pixels[nThisPixel] = float(uhTemp/65536.0f);
					 nThisPixel++;
            }
		  }

        //	By default, the RasterImage class adopts the "safe" mode for the definition
        //	of the valid data rectangle.  Since image1 was just allocated not initialized,
        //	its valid data rectangle was located at the upper left corner of the bounding
        //	rectangle, with width and height 0.  We must now set the correct values.
        ptImage->setValidRect(0, 0, nbCols, nbRows);
    }
    else
    {
        printf("Generation of image 1 failed.");
    }

} // end of MakeFake_ImageGSF

void WXGL_IO_Canvas::MakeFake_ImageBinary(RasterImage *ptImage)
{
    //-------------------------------------------------------------
    //	Image 1 is created by computation --> fill the array with GS values
    //   This code fragment demonstrate how to get direct access to the image's
    //   raster for initialization and other IP calculations
	 //
	 //   This code was lifted from SimpleIO demo program
    //-------------------------------------------------------------

    //
    //  the raster image is created at the desired dimensions
    if (ptImage != NULL)
    {
        //  we get a reference to the image's base raster
        //  Here, we use the version of the getRaster family of functions that return a
        //  read-write reference
        unsigned char *pubRaster = ((RasterImage_binary*)ptImage)->getRaster(R_W_ACCESS);

        int   nbRows = ptImage->getHeight(),
              nbCols = ptImage->getWidth();
        //  from this point on
        for (int i=0; i<nbRows; i++)
		  {
            for (int j=0; j<nbCols; j++)
            {
					if (( (j%3)==0 )&&( (i%8)==0)) // set every 3rd column in every 8th row
					{
						*pubRaster++ = 255;
					}
					else
					{
						*pubRaster++ = 0;
					}
            }
		  }

        //	By default, the RasterImage class adopts the "safe" mode for the definition
        //	of the valid data rectangle.  Since image1 was just allocated not initialized,
        //	its valid data rectangle was located at the upper left corner of the bounding
        //	rectangle, with width and height 0.  We must now set the correct values.
        ptImage->setValidRect(0, 0, nbCols, nbRows);
    }
    else
    {
        printf("Generation of image 1 failed.");
    }

} // end of MakeFake_ImageBinary