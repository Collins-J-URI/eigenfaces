/////////////////////////////////////////////////////////////////////////////
// Name:        WXGL_IO_App.h
// Purpose:     Header file for example wxWidgets and OpenGL demo application
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

#ifndef _WXGL_IO_APP_H_
#define _WXGL_IO_APP_H_

class WXGL_IO_App: public wxApp
{
public:
    WXGL_IO_App(){};
    virtual ~WXGL_IO_App();
    virtual bool OnInit();

private:
	int    m_nDisplayWidth, m_nDisplayHeight; // needed here to create frame
	
	WXGL_IO_Frame* mp_Frame;
};

DECLARE_APP(WXGL_IO_App)

#endif // _WXGL_IO_APP_H_
