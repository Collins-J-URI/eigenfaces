/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_App.cpp
// Purpose:     Source file for example wxWidgets and OpenGL demo application
//              This file contains all the main application class functions
// Author:      Lewis Collier
// Created:     16 April 2012
// Copyright:   (c) 2012-2014, 3D Group for Interactive Visualization
//                        University of Rhode Island.
//
//        Licensed under the Academic Free License - v. 3.0
//        For more information see http://opensource.org/licenses/academic.php
//---------------------------------------------------------------------------
//
// Program structure is
//
// App --> Frame --> Menus (for GUI, these are Frame methods, separated for clarity only)
//               \-> Canvas (for drawing), can have multiple canvases
//
//              Make a new VisualStudio "empty project".
//              Set up include and lib dirs as per .\Docs\SettingUp_VCXpress_WXGL_Project.txt
//              Copy these sources and headers into new directory.
//              Rename this APP to new application name.
//              Build new frame, canvas, and menu classes from these classes.
//              Add new .cpp/.h file pairs as needed for classes for new functionality.
//
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
#include "WXGL_IO_canvas.h"
#include "WXGL_IO_frame.h"
#include "WXGL_IO_SVINFO.h"

// Application specific includes
#include "WXGL_IO_App.h"

// Create a new application object
IMPLEMENT_APP (WXGL_IO_App)


// Application destructor
WXGL_IO_App::~WXGL_IO_App()
{
}

// Application OnInit function
// this function creates the frame
bool WXGL_IO_App::OnInit()
{
	/*
#ifndef __WXWINCE__
    m_helpFile = wxGetCwd() + wxFILE_SEP_PATH +  wxT("..") + wxFILE_SEP_PATH + wxT("TI_TALU_HELP.htm");
    if (!wxFileExists(m_helpFile))
#endif
    {
        m_helpFile = wxPathOnly(argv[0]) + wxFILE_SEP_PATH + wxT("..") + wxFILE_SEP_PATH + wxT("TI_TALU_HELP.htm");
    }
*/

	// Set to full screen
	wxDisplaySize(&m_nDisplayWidth, &m_nDisplayHeight);
    wxSize size(m_nDisplayWidth,m_nDisplayHeight);

    mp_Frame = new WXGL_IO_Frame(
            0,
				_T("URI Vision Library :: wxWidget and OpenGL Example Application"),
            wxDefaultPosition,
            size
    );


	// Show the frame
   mp_Frame->Show(true);

	// initialize the canvas now that it is shown
	mp_Frame->InitCanvas();


//////////////////////////////////////////////////////////////////
   //		Version init code

#if USE_SVNINFO
   CreateSvinfo(1); //we only have one project, we only need one structure.
 
   //put the parts into the SVNINFO object
   SetSvnInfo(	&WXGL_IO_RevInfo,
				WXGL_IO_SVINFO_VERSION_MAJOR,
				WXGL_IO_SVINFO_VERSION_MINOR,
				WXGL_IO_SVINFO_VERSION_PATCH,
				_T("CVPD_WXGL")
				);
#endif
															//
//////////////////////////////////////////////////////////////////






	// return true since we were lazy:)
	return true;
}

