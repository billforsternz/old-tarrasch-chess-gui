/****************************************************************************
 * Control - A coloured rectangular box, subclass for user interaction
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef CTRL_BOX_H
#define CTRL_BOX_H
#include "wx/wx.h"

class CtrlBox : public wxControl
{
    DECLARE_CLASS( CtrlBox )
    DECLARE_EVENT_TABLE()

public:
	// Constructor
	CtrlBox( wxWindow *parent,
             wxWindowID id,
             const wxPoint& point,
             const wxSize& size,
             long  style=BORDER_COMMON );

    // Write txt
    void SetTxt( const wxString &txt, bool redraw=true );

private:
    wxString txt;
    void OnMouseLeftDown (wxMouseEvent& WXUNUSED(event));
    void OnEraseBackGround(wxEraseEvent& WXUNUSED(event)) {};

public:
    bool     hack_its_a_clock;
    wxRect   r;
    void OnPaint(wxPaintEvent& WXUNUSED(evt));
};

#endif  // CTRL_BOX
