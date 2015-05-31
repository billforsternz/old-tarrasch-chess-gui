/******************************************************************************
 * Top level application definitions
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ******************************************************************************/
#ifndef APPDEFS_H
#define APPDEFS_H

#include "wx/wx.h"

//#define MATE_IN_ZERO_ENGINE_BUG_WORKAROUND
#ifdef  MATE_IN_ZERO_ENGINE_BUG_WORKAROUND
#define MASTER_VERSION "V2.03a + Mate in zero engine bug workaround"
#else
#define MASTER_VERSION "V2.03a"
#endif
#define DEFAULT_ENGINE "stockfish-6-32.exe"
#define DEFAULT_ENGINE_64 "stockfish-6-64.exe"  // new with V2.03a
// #define MASTER_VERSION "V2.01ar"
// #define DEFAULT_ENGINE "Rybka v2.3.2a.mp.w32.exe"
// #define MASTER_VERSION "V2.01ak"         // for Komodo version
// #define DEFAULT_ENGINE "komodo3-32.exe"  // for Komodo version

// Misc
#define NBR_KIBITZ_LINES 4
#define BORDER_COMMON wxBORDER_SUNKEN   //wxBORDER_SIMPLE

//-----------------------------------------------------------------------------
// Menu item identifiers
//-----------------------------------------------------------------------------
enum
{
    ID_Quit = 1,
    ID_Clear,
    ID_FILE_OPEN_LOG,
    ID_CMD_FLIP,
    ID_CMD_SET_POSITION, 
    ID_CMD_NEW_GAME,        
    ID_CMD_TAKEBACK,     
    ID_CMD_MOVENOW,     
    ID_CMD_DRAW,
    ID_CMD_WHITE_RESIGNS,
    ID_CMD_BLACK_RESIGNS,
    ID_CMD_PLAY_WHITE,   
    ID_CMD_PLAY_BLACK,   
    ID_CMD_SWAP_SIDES,         
    ID_CMD_KIBITZ,         
    ID_CMD_CLEAR_KIBITZ,
    ID_OPTIONS_ENGINE,      
    ID_OPTIONS_PLAYERS, 
    ID_OPTIONS_CLOCKS,
    ID_OPTIONS_BOOK,
    ID_OPTIONS_LOG,
    ID_OPTIONS_TRAINING,
    ID_OPTIONS_GENERAL,
    ID_OPTIONS_RESET,       
    ID_FILE_CURRENT,                 
    ID_FILE_SESSION,                 
    ID_FILE_CLIPBOARD,           
    ID_EDIT_GAME_DETAILS,
    ID_EDIT_PROMOTE,             
    ID_EDIT_DEMOTE,              
    ID_EDIT_DEMOTE_TO_COMMENT,   
    ID_EDIT_PROMOTE_TO_VARIATION,
    ID_EDIT_PROMOTE_REST_TO_VARIATION,
    ID_HELP_HELP,
    ID_HELP_CREDITS,
    ID_BUTTON_UP,   
    ID_BUTTON_DOWN, 
    ID_BUTTON_LEFT, 
    ID_BUTTON_RIGHT,
    ID_CMD_NEXT_GAME, 
    ID_CMD_PREVIOUS_GAME,
    ID_ANNOT1,
    ID_ANNOT2,
    ID_ANNOT3,
    ID_ANNOT4,
    ID_ANNOT5,
    ID_ANNOT6,
    ID_ANNOT7,
    ID_ANNOT10,
    ID_ANNOT14,
    ID_ANNOT15,
    ID_ANNOT16,
    ID_ANNOT17,
    ID_ANNOT18,
    ID_ANNOT19,
    ID_ANNOT20,
    ID_CMD_ABOUT = wxID_ABOUT,   // important for possible Mac port that we use this apparently
    ID_COPY_GAME_PGN_TO_CLIPBOARD
};

//-----------------------------------------------------------------------------
// Window identifiers
//-----------------------------------------------------------------------------
enum
{
    ID_POPUP=100,
    ID_BUTTON1,
    ID_BUTTON2,
    ID_BUTTON3,
    ID_BUTTON4,
    ID_WHO_TOP,
    ID_WHO_BOTTOM,
    ID_WHO_STATUS,
    ID_STATUS,
    ID_WHITE_PLAYER,
    ID_DASH_PLAYER,
    ID_BLACK_PLAYER,
    ID_BORDER,
    ID_BOX,
    ID_KIBITZ,
    ID_KIBITZ_BUTTON1,
    ID_KIBITZ_BUTTON2,
    ID_BOOK_MOVES,
/*  ID_WHITE_CLOCK,
    ID_WHITE_CLBOX,
    ID_BLACK_CLOCK,
    ID_BLACK_CLBOX, */
    ID_LIST_CTRL,
    ID_KIBITZ0,
    ID_KIBITZ1,
    ID_KIBITZ2,
    ID_KIBITZ3,
    ID_KIBITZ4,
    ID_COMBOBOX,
    ID_PGN_LISTBOX
};

#endif // APPDEFS_H
