/****************************************************************************
 * Control - A CtrlBox, subclassed for book moves user interaction
 *  (i.e. the green book moves box under the move list)
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef CTRL_BOX_BOOK_MOVES_H
#define CTRL_BOX_BOOK_MOVES_H
#include "wx/wx.h"
#include "CtrlBox2.h"

class CtrlBoxBookMoves : public CtrlBox2
{
    DECLARE_CLASS( CtrlBoxBookMoves )
    DECLARE_EVENT_TABLE()

public:

	// Constructor
	CtrlBoxBookMoves( wxWindow *parent,
                  wxWindowID id,
                  const wxPoint& point,
                  const wxSize& size );
    void OnMouseMove (wxMouseEvent& event );
};

#endif  // CTRL_BOX_BOOK_MOVES
