/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_canvas.h
// Purpose:     Header file for example wxWidgets and OpenGL demo application
//              drawing CANVAS class
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
#ifndef _WXGL_IO_CANVAS_H_
#define _WXGL_IO_CANVAS_H_

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "RasterImage.h"

using namespace std;
using namespace uriVL;

class WXGL_IO_Canvas: public wxGLCanvas
	//, public wxScrolledWindow
	// may need to add this back in for scrolling canvases, iff this ever works under windows:(
{
public:
    WXGL_IO_Canvas(wxWindow* parent,
						 const wxWindowID id = -1,
						 const wxPoint& pos = wxDefaultPosition, 
						 const wxSize& size = wxDefaultSize,
						 long style = 0,
						 const wxString &name = "GLCanvas");

    virtual ~WXGL_IO_Canvas();

	 void MakeFake_ImageRGBa(RasterImage *ptImage);
	 void MakeFake_ImageGS(RasterImage *ptImage);
	 void MakeFake_ImageGSF(RasterImage *ptImage);
	 void MakeFake_ImageBinary(RasterImage *ptImage);

	 void DrawSquare(void); // demo function
	 void DrawSquare(int nX, int xY); // demo function

    //virtual void OnDraw(wxDC& dc);
    bool OnCloseCanvas();
    void OnMouseEvent(wxMouseEvent& event);
    void OnCharEvent(wxKeyEvent& event);
	 void OnFocusEvent(wxFocusEvent& event);
	 void OnKillFocusEvent(wxFocusEvent& event);
	 void OnActivateEvent(wxActivateEvent& event);
	 void OnShowEvent(wxShowEvent& event);
	 void OnPaintEvent(wxPaintEvent& event);

	 // functions to do real stuff
	 void SetUpCamera(void);
	 void UpdateCanvas(void);
	 void CreateTestImage(void);

	 // frame accessor functions
	 void SetTestImageRows(unsigned long ulR){mulRows=ulR;}
	 unsigned long GetTestImageRows(){return mulRows;}
	 void SetTestImageCols(unsigned long ulC){mulCols=ulC;}
	 unsigned long GetTestImageCols(){return mulCols;}
	 void SetFileType(unsigned long ulFT){mulFileType=ulFT;}
	 unsigned long GetFileType(){return mulFileType;}
	 void SetRasterType(unsigned long ulRT){mulRasterType=ulRT;}
	 unsigned long GetRasterType(){return mulRasterType;}

protected:
    DECLARE_EVENT_TABLE() // put it in protected since this macro does a protected: implicitly

private:
	RasterImage *pmtOriginalImage;
	RasterImage *pmtReadInImage;
	RasterImage *pmtDifferenceImage;

	unsigned long mulRows, mulCols;
	unsigned long mulFileType, mulRasterType;

	enum MenuCommands {
			 // file extension types
			MENU_ID_EXT_BMP = 100, // copies are made in frame so these have to match
			MENU_ID_EXT_JPG,
			MENU_ID_EXT_PNG,
			MENU_ID_EXT_TIF,

			 // raster types
			MENU_ID_IMAGE_BINARY,
			MENU_ID_IMAGE_GRAY,
			MENU_ID_IMAGE_GRAY_F,
			MENU_ID_IMAGE_RGBA,
    };
};

#endif // _WXGL_IO_CANVAS_H_
