/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_Frame.cpp
// Purpose:     Source file for example wxWidgets and OpenGL demo application
//              This file contains all the frame class menu functions
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

// application object includes
#include "WXGL_IO_canvas.h"
#include "WXGL_IO_frame.h"

// Application specific includes
#include "WXGL_IO_App.h"
#include "SVINFO.h" 

// flag to show callback message boxes
#define DISPLAY_CALLBACK_MSGBOX 1

/////////////////////////////////////////
//  stock and standard OnCloseWindow callback
/////////////////////////////////////////
void WXGL_IO_Frame::OnCloseWindow(wxCloseEvent& myEvent)
{
    if (mp_Canvas->OnCloseCanvas() )
    {
        this->Destroy();
    }
    else
        myEvent.Veto();
}

/////////////////////////////////////////
//         CREATE MENUS
/////////////////////////////////////////

void WXGL_IO_Frame::CreateMenus()
{    
	 // Make the FILE MENU 
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(MENU_ID_FILE_OPEN, 
		 _T("&Open"), 
		 _T("Open a configuration file for this utility")
		 );
    fileMenu->Append(MENU_ID_FILE_SAVE, 
		 _T("&Save"), 
		 _T("Save current configuration for this utility")
		 );
    fileMenu->Append(MENU_ID_FILE_SAVEAS, 
		 _T("Save &As"), 
		 _T("Save current configuration for this utility")
		 );

	 fileMenu->AppendSeparator();

	 fileMenu->Append(MENU_ID_FILE_READ, 
		 _T("&Read"), 
		 _T("Read current values from the controller")
		 );
    fileMenu->Append(MENU_ID_FILE_WRITE, 
		 _T("&Write"), 
		 _T("Write current values to the controller")
		 );
 
	 fileMenu->AppendSeparator();

    fileMenu->Append(MENU_ID_FILE_EXIT, 
		 MENU_TXT_FILE_EXIT, 
		 _T("Exits the utility")
		 );

	 // make the PARAMETER MENU
    wxMenu* paramMenu = new wxMenu;
    paramMenu->Append(MENU_ID_PARAM_ROWS, 
		_T("&Rows"), 
		_T("Sets the number of rows the test image")
		);
    paramMenu->Append(MENU_ID_PARAM_COLS, 
		_T("&Cols"), 
		_T("Sets the number of columns the test image")
		);

	 // make the TYPE MENU
    wxMenu* typeMenu = new wxMenu;
    typeMenu->Append(MENU_ID_EXT_BMP, 
		_T("&BMP"), 
		_T("Sets the test image type to .BMP")
		);
    typeMenu->Append(MENU_ID_EXT_JPG, 
		_T("&JPG"), 
		_T("Sets the test image type to .JPG")
		);
    typeMenu->Append(MENU_ID_EXT_PNG, 
		_T("&PNG"), 
		_T("Sets the test image type to .PNG")
		);
    typeMenu->Append(MENU_ID_EXT_TIF, 
		_T("&TIF"), 
		_T("Sets the test image type to .TIF")
		);


	 // make the IMAGE MENU
    wxMenu* imageMenu = new wxMenu;
    imageMenu->Append(MENU_ID_IMAGE_BINARY, 
		_T("&Binary"), 
		_T("Sets the test image raster to Binary")
		);
    imageMenu->Append(MENU_ID_IMAGE_GRAY_F, 
		_T("&FloatGray"), 
		_T("Sets the test image raster to Floating Grayscale")
		);
    imageMenu->Append(MENU_ID_IMAGE_GRAY, 
		_T("&Gray"), 
		_T("Sets the test image raster to Grayscale")
		);
    imageMenu->Append(MENU_ID_IMAGE_RGBA, 
		_T("&RGBa"), 
		_T("Sets the test image raster to RGBa")
		);


	 // make the HELP MENU
    wxMenu* helpMenu = new wxMenu;
	     helpMenu->Append(MENU_ID_HELP_HELP, 
		_T("&Help Contents"), 
		_T("Displays information about running this utility")
		);

    helpMenu->Append(MENU_ID_HELP_ABOUT, 
		_T("&About..."), 
		_T("Displays information about the Alignment and Linearization utility")
		);

	 // make the menu bar
    mp_menuBar = new wxMenuBar;
    mp_menuBar->Append(fileMenu,   _T("&File"));
    mp_menuBar->Append(paramMenu,  _T("&Params"));
    mp_menuBar->Append(typeMenu,   _T("&Type"));
    mp_menuBar->Append(imageMenu,  _T("&Image"));
    mp_menuBar->Append(helpMenu,   _T("&Help"));

    SetMenuBar(mp_menuBar);
}

/////////////////////////////////////////
//         FILE MENUS
/////////////////////////////////////////

void WXGL_IO_Frame::FileOpen(wxCommandEvent& WXUNUSED(myEvent))
{
#if DISPLAY_CALLBACK_MSGBOX 
	wxMessageBox(
		_T("FileOpen command reached\n"),_T("DEBUG"),
      wxOK|wxICON_INFORMATION, 
		this
   );
#endif // DISPLAY_CALLBACK_MSGBOX

}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::FileSave(wxCommandEvent& WXUNUSED(myEvent))
{
#if DISPLAY_CALLBACK_MSGBOX 
	wxMessageBox(
		_T("FileSave command reached\n"),_T("DEBUG"),
      wxOK|wxICON_INFORMATION, 
		this
   );
#endif // DISPLAY_CALLBACK_MSGBOX

}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::FileSaveAs(wxCommandEvent& WXUNUSED(myEvent))
{
#if DISPLAY_CALLBACK_MSGBOX 
	wxMessageBox(
		_T("FileSaveAs command reached\n"),_T("DEBUG"),
      wxOK|wxICON_INFORMATION, 
		this
   );
#endif // DISPLAY_CALLBACK_MSGBOX

}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::FileRead(wxCommandEvent& WXUNUSED(myEvent))
{
#if DISPLAY_CALLBACK_MSGBOX 
	wxMessageBox(
		_T("FileRead command reached\n"),_T("DEBUG"),
      wxOK|wxICON_INFORMATION, 
		this
   );
#endif // DISPLAY_CALLBACK_MSGBOX

}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::FileWrite(wxCommandEvent& WXUNUSED(myEvent))
{
 #if DISPLAY_CALLBACK_MSGBOX 
	wxMessageBox(
		_T("FileWrite command reached\n"),_T("DEBUG"),
      wxOK|wxICON_INFORMATION, 
		this
   );
#endif // DISPLAY_CALLBACK_MSGBOX

}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::FileExit(wxCommandEvent& WXUNUSED(myEvent))
{
    Close(true);
}



/////////////////////////////////////////
//      PARAM MENUS
/////////////////////////////////////////
void WXGL_IO_Frame::ParamSize(wxCommandEvent& myEvent)
{
	// set up default items for the dialog box
	wxString strMessage("defMessage");
	wxString strDefaultValue;
	wxPoint  pointLoc(242,242);
	char abValue[32];

	// check event to see if this is looking for rows or columns
	// set caption string accordingly
	char *pszMessage = 0;
	char *pszCaption = "Enter Test Image Size Parameter";
	if (myEvent.GetId() == MENU_ID_PARAM_COLS)
	{
		pszMessage = "Enter # of COLUMNS in test image";
		sprintf(abValue,"%d",mp_Canvas->GetTestImageCols());
	}
	else
	{
		pszMessage = "Enter # of ROWS in test image";
		sprintf(abValue,"%d",mp_Canvas->GetTestImageRows());
	}
	strDefaultValue = abValue;
	wxTextEntryDialog *ptDialog = new wxTextEntryDialog(this,wxString(pszMessage),wxString(pszCaption),strDefaultValue,wxOK|wxCANCEL|wxCENTRE, pointLoc);

	if (ptDialog != 0)
	{
		int nStatus = ptDialog->ShowModal();

		if (nStatus==wxID_OK)
		{
			wxString strAnswer = ptDialog->GetValue();
			if(strAnswer.IsNumber())
			{
				unsigned long ulAnswer;
				strAnswer.ToULong(&ulAnswer);
				if (myEvent.GetId() == MENU_ID_PARAM_COLS)
				{		
					mp_Canvas->SetTestImageCols(ulAnswer);
				}
				else if (myEvent.GetId() == MENU_ID_PARAM_ROWS)
				{
					mp_Canvas->SetTestImageRows(ulAnswer);				
				}
				mp_Canvas->CreateTestImage();
				mp_Canvas->UpdateCanvas();
			}
			else
			{
				wxMessageBox(
									_T("Invalid Data Entry\n")+strAnswer,
									_T("ERROR"),
									wxOK|wxICON_INFORMATION, 
									this
								 );
			}
		}
	}
	else
	{
			wxMessageBox(
								_T("Error creating text dialog box\n"),
								_T("ERROR"),
								wxOK|wxICON_INFORMATION, 
								this
							 );
	}

}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::TypeExtension(wxCommandEvent& myEvent)
{
	mp_Canvas->SetFileType(myEvent.GetId());
	mp_Canvas->CreateTestImage();
	mp_Canvas->UpdateCanvas();
}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::RasterType(wxCommandEvent& myEvent)
{
	mp_Canvas->SetRasterType(myEvent.GetId());
	mp_Canvas->CreateTestImage();
	mp_Canvas->UpdateCanvas();
}
/////////////////////////////////////////
//      HELP MENUS
/////////////////////////////////////////
void WXGL_IO_Frame::HelpHelp(wxCommandEvent& myEvent)
{
#if 0 & wxUSE_HTML
    if (wxFileExists(wxGetApp().GetHelpFile()))
    {
        Linear25AboutDialog dialog(this, wxID_ANY, wxT("Demonstration wxWidget and OpenGL Code"));
        if (dialog.ShowModal() == wxID_OK)
        {
        }
    }
    else
#endif
    {
        wxMessageBox(
            _T("URI Vision Library\n\n")
            _T("Demonstration wxWidget and OpenGL Code\n\n")
            _T("Author: Lewis Collier\n")
				_T("email:  wlcollier@cs.uri.edu\n\n")
				_T("(c) 2012 URI 3D Group for Interactive Visualization\n\n")
				_T("HELP COMING IN FUTURE VERSION\n"),
				_T("HELP"),
            wxOK|wxICON_INFORMATION, 
				this
            );
    }
}

///////////////////////////////////////////////////////////////////////////////
void WXGL_IO_Frame::HelpAbout(wxCommandEvent& WXUNUSED(myEvent))
{
#if USE_SVNINFO
	char full_rev[128];
	memset(full_rev,NULL,sizeof(full_rev));
	int result=GetSvinfo_FullRev(full_rev,sizeof(full_rev));
#else
	char *full_rev = "1.0.0"; // manually set the version number if SVN info is not used
#endif


	wxString contents=
				_T("URI Vision Library\n\n")
				_T("Demonstration wxWidget and OpenGL Code\n\n Version: ");
	contents+=	full_rev;
	contents+=	_T("\n\n")
				_T("Author: Lewis Collier\n") 
				_T("email:  wlcollier@cs.uri.edu\n")
				_T("(c) 2012  URI 3D Group for Interactive Visualization");
       
	wxMessageBox(	contents,
					_T("ABOUT"),
					wxOK|wxICON_INFORMATION, 
					this
				);
}