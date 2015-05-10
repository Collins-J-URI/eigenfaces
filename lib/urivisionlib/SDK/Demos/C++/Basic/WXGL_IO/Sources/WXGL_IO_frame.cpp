/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_frame.cpp
// Purpose:     Source file for example wxWidgets and OpenGL demo application
//              This file contains all the frame class functions
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

// base CVPD wxWidget / OpenGL includes for this application
#include "WXGL_IO_menus.h"
#include "WXGL_IO_canvas.h"
#include "WXGL_IO_frame.h"


// Application specific includes
#include "WXGL_IO_App.h"

BEGIN_EVENT_TABLE(WXGL_IO_Frame, wxFrame)

	     EVT_MENU(MENU_ID_FILE_OPEN,WXGL_IO_Frame::FileOpen)
        EVT_MENU(MENU_ID_FILE_SAVE,WXGL_IO_Frame::FileSave)
        EVT_MENU(MENU_ID_FILE_SAVEAS,WXGL_IO_Frame::FileSaveAs)
        EVT_MENU(MENU_ID_FILE_READ,WXGL_IO_Frame::FileRead)
        EVT_MENU(MENU_ID_FILE_WRITE,WXGL_IO_Frame::FileWrite)
		  EVT_MENU(MENU_ID_FILE_EXIT, WXGL_IO_Frame::FileExit)
        EVT_CLOSE(WXGL_IO_Frame::OnCloseWindow)

		  EVT_MENU(MENU_ID_PARAM_COLS,WXGL_IO_Frame::ParamSize)
		  EVT_MENU(MENU_ID_PARAM_ROWS,WXGL_IO_Frame::ParamSize)

		  EVT_MENU(MENU_ID_EXT_BMP,WXGL_IO_Frame::TypeExtension)
		  EVT_MENU(MENU_ID_EXT_JPG,WXGL_IO_Frame::TypeExtension)
		  EVT_MENU(MENU_ID_EXT_PNG,WXGL_IO_Frame::TypeExtension)
		  EVT_MENU(MENU_ID_EXT_TIF,WXGL_IO_Frame::TypeExtension)

		  EVT_MENU(MENU_ID_IMAGE_BINARY,WXGL_IO_Frame::RasterType)
		  EVT_MENU(MENU_ID_IMAGE_GRAY,  WXGL_IO_Frame::RasterType)
		  EVT_MENU(MENU_ID_IMAGE_GRAY_F,WXGL_IO_Frame::RasterType)
		  EVT_MENU(MENU_ID_IMAGE_RGBA,  WXGL_IO_Frame::RasterType)

		  EVT_MENU(MENU_ID_HELP_HELP,WXGL_IO_Frame::HelpHelp)
		  EVT_MENU(MENU_ID_HELP_ABOUT,WXGL_IO_Frame::HelpAbout)

END_EVENT_TABLE()


// My frame constructor
WXGL_IO_Frame::WXGL_IO_Frame(wxFrame* frame, const wxString& title, const wxPoint& pos, const wxSize& size):
    wxFrame(frame, wxID_ANY, title, pos, size)
{
#ifdef __WXMAC__
    wxApp::s_macAboutMenuItemId = wxID_ABOUT ;
#endif
    // set the icon
#ifdef __WXMSW__
    SetIcon(wxIcon(_T("UtilityIcon.ico")));
#else
#ifdef GTK_TBD
    SetIcon(wxIcon(Cards_bits, Cards_width, Cards_height));
#endif
#endif

	// call member function to init all the memus
	CreateMenus();

	// Create new drawing canvas
	mp_Canvas = new WXGL_IO_Canvas(this, -1 , wxDefaultPosition, size, 0, "GLKansas");
	
#if wxUSE_STATUSBAR
    CreateStatusBar();
#endif // wxUSE_STATUSBAR
}


WXGL_IO_Frame::~WXGL_IO_Frame()
{
	// do not need to delete frame or canvas here
	// they get done automatically in WX base classes somewhere
}


wxGLCanvas* WXGL_IO_Frame::GetCanvas() 
{ 
	return mp_Canvas; 
}

// function to init the canvas
// does stuff like SetCurrent() and SetUpCamera() for GL calls
void WXGL_IO_Frame::InitCanvas()
{
	// make canvas current for GL calls
	mp_Canvas->SetCurrent();
	// sey up the camera co-ords
	mp_Canvas->SetUpCamera();
}



//----------------------------------------------------------------------------
// stAboutDialog
//----------------------------------------------------------------------------

WXGL_IO_AboutDialog::WXGL_IO_AboutDialog( wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &position, const wxSize& size, long style ) :
    wxDialog( parent, id, title, position, size, style )
{
    AddControls(this);

    Centre(wxBOTH);
}

bool WXGL_IO_AboutDialog::AddControls(wxWindow* parent)
{
#if wxUSE_HTML
    wxString htmlText, htmlLine;
    wxString htmlFile = "nada"; //wxGetApp().GetHelpFile();

    {
        wxTextFile file(htmlFile);
        if (file.Exists())
        {
			  file.Open();
#if 0    
            for ( htmlText = file.GetFirstLine();
                  !file.Eof();
                  htmlText << file.GetNextLine() /*<< _T("\n")*/ ) ;
#else
				htmlText = file.GetFirstLine();
				while (!file.Eof())
				{
					htmlLine = file.GetNextLine();
					if (!htmlLine.empty())
					{
						htmlText << htmlLine;
					}
				}
			  file.Close();

#endif
        }
    }

    if (htmlText.empty())
    {
        htmlText.Printf(wxT("<html><head><title>Warning</title></head><body><P>Sorry, could not find resource for Help Contents dialog</P></body></html>"));
    }

    // Customize the HTML
    htmlText.Replace(wxT("$DATE$"), _T(__DATE__));

    wxSize htmlSize(400, 290);

    // Note: in later versions of wxWin this will be fixed so wxRAISED_BORDER
    // does the right thing. Meanwhile, this is a workaround.
#ifdef __WXMSW__
    long borderStyle = wxDOUBLE_BORDER;
#else
    long borderStyle = wxRAISED_BORDER;
#endif

    wxHtmlWindow* html = new wxHtmlWindow(this, ID_ABOUT_HTML_WINDOW, wxDefaultPosition, htmlSize, borderStyle);
    html -> SetBorders(10);
    html -> SetPage(htmlText);

    //// Start of sizer-based control creation

    wxSizer *item0 = new wxBoxSizer( wxVERTICAL );

    wxWindow *item1 = parent->FindWindow( ID_ABOUT_HTML_WINDOW );
    wxASSERT( item1 );
    item0->Add( item1, 0, wxALIGN_CENTRE|wxALL, 5 );

    wxButton *item2 = new wxButton( parent, wxID_CLOSE );
    item2->SetDefault();
    item2->SetFocus();
    SetAffirmativeId(wxID_CLOSE);

    item0->Add( item2, 0, wxALIGN_RIGHT|wxALL, 5 );

    parent->SetSizer( item0 );
    parent->Layout();
    item0->Fit( parent );
    item0->SetSizeHints( parent );
#endif

    return true;
}
