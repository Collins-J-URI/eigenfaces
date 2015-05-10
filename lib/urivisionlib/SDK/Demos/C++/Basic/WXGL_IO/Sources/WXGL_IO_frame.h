/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_frame.h
// Purpose:     Header file for example wxWidgets and OpenGL demo application
//              structure FRAME class
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
#ifndef _WXGL_IO_FRAME_H_
#define _WXGL_IO_FRAME_H_

class WXGL_IO_Frame: public wxFrame
{
public:
/////////////////////////////////////////////////////////////////////////////
// constructor/destructor functions
/////////////////////////////////////////////////////////////////////////////
    WXGL_IO_Frame(wxFrame* frame, const wxString& title, const wxPoint& pos, const wxSize& size);
	 virtual ~WXGL_IO_Frame();

	 class wxGLCanvas* GetCanvas();

	 void InitCanvas();

/////////////////////////////////////////////////////////////////////////////
// menu and control functions
/////////////////////////////////////////////////////////////////////////////    
	 
	 void OnCloseWindow(wxCloseEvent& event);

    //
	 // Menu creation and callback functions
	 //
	 void CreateMenus();

	 // File
    void FileOpen(wxCommandEvent& event);
	 void FileSave(wxCommandEvent& event);
	 void FileSaveAs(wxCommandEvent& event);
	 //------------------
	 void FileRead(wxCommandEvent& event);
	 void FileWrite(wxCommandEvent& event);
	 //------------------
    void FileExit(wxCommandEvent& event);

	 // Parameters
    void ParamSize(wxCommandEvent& event);

	 // Type of File
    void TypeExtension(wxCommandEvent& event);

	 // Type of Raster
    void RasterType(wxCommandEvent& event);

	 // Help
    void HelpAbout(wxCommandEvent& event);
    void HelpHelp(wxCommandEvent& event);

    //WXGL_IO_Canvas* GetCanvas() { return m_Canvas; }

    DECLARE_EVENT_TABLE()

private:

	 // wxString m_helpFile;

    WXGL_IO_Canvas *mp_Canvas;
	 wxMenuBar        *mp_menuBar;


///////////////////////////////////////////////////////////////////////////////
// menu option flags
///////////////////////////////////////////////////////////////////////////////

#define MENU_ID_FILE_EXIT  wxID_EXIT                  // MUST USE wxID_EXIT so it is #defined here
#define MENU_TXT_FILE_EXIT wxGetStockLabel(wxID_EXIT) // use standard/stock exit text
#define MENU_ID_HELP_ABOUT wxID_ABOUT                 // MUST USE wxID_ABOUT so it is #defined here

	enum MenuCommands {
		  // menu file params
        MENU_ID_FILE_OPEN = 10,
        MENU_ID_FILE_SAVE,
        MENU_ID_FILE_SAVEAS,
        MENU_ID_FILE_READ,
        MENU_ID_FILE_WRITE,
		  // MENU_ID_FILE_EXIT MUST USE wxID_EXIT so it is #defined above

		  // parameter setting IDs
		  MENU_ID_PARAM_ROWS,
		  MENU_ID_PARAM_COLS,

			 // file extension types
			MENU_ID_EXT_BMP = 100, // copies are made in canvas so these have to match
			MENU_ID_EXT_JPG,
			MENU_ID_EXT_PNG,
			MENU_ID_EXT_TIF,

			 // raster types
			MENU_ID_IMAGE_BINARY,
			MENU_ID_IMAGE_GRAY,
			MENU_ID_IMAGE_GRAY_F,
			MENU_ID_IMAGE_RGBA,

			MENU_ID_HELP_HELP
			// MENU_ID_HELP_ABOUT MUST USE wxID_ABOUT so it is #defined above
    };

};

//----------------------------------------------------------------------------
// stAboutDialog
//
//----------------------------------------------------------------------------

class WXGL_IO_AboutDialog: public wxDialog
{
public:
    // constructors and destructors
    WXGL_IO_AboutDialog( wxWindow *parent, wxWindowID id, const wxString &title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE );

    bool AddControls(wxWindow* parent);
};

#define ID_ABOUT_HTML_WINDOW    1000

#endif // _WXGL_IO_FRAME_H_
