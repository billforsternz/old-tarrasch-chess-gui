/****************************************************************************
 * Top level app file for Tarrasch chess GUI
 *  Similar to boilerplate app code for wxWidgets example apps
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include "wx/wx.h"
#include "wx/file.h"
#include "wx/listctrl.h"
#include "wx/clipbrd.h"
#include "time.h"
#include "Appdefs.h"
#include "Canvas.h"
#include "GraphicBoard.h"
#include "GameLogic.h"
#include "Lang.h"
#include "CentralWorkSaver.h"
#include "Session.h"
#include "Log.h"
#include "ChessRules.h"
#include "Rybka.h"
#include "DebugPrintf.h"
#include "Book.h"
#include "Objects.h"
#include "BookDialog.h"
#include "LogDialog.h"
#include "EngineDialog.h"
#include "TrainingDialog.h"
#include "GeneralDialog.h"
#include "Repository.h"
#include "CtrlBox.h"
#include "CtrlBoxBookMoves.h"
    #include "bitmaps/myicons.xpm"
    #include "bitmaps/new.xpm"
    #include "bitmaps/open.xpm"
    #include "bitmaps/save.xpm"
    #include "bitmaps/copy.xpm"
    #include "bitmaps/cut.xpm"
    #include "bitmaps/paste.xpm"
    #include "bitmaps/undo.xpm"
    #include "bitmaps/redo.xpm"
using namespace std;
using namespace thc;

Objects objs;

// Should really be a little more sophisticated about this
#define TIMER_ID 2001

// ----------------------------------------------------------------------------
// application class
// ----------------------------------------------------------------------------

class ChessApp: public wxApp
{
public:
    virtual bool OnInit();
    virtual int  OnExit();
};

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

class ChessFrame: public wxFrame
{
public:
    ChessFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnIdle(wxIdleEvent& event);
    void OnMove       (wxMoveEvent &event);
    void OnTimeout    (wxTimerEvent& event);
    void OnQuit       (wxCommandEvent &);
    void OnClose      (wxCloseEvent &);
    void OnAbout      (wxCommandEvent &);
//  void OnUnimplemented    (wxCommandEvent &);
    void OnHelp             (wxCommandEvent &);
    void OnCredits          (wxCommandEvent &);
    void OnFlip             (wxCommandEvent &);
    void OnButtonUp         (wxCommandEvent &);
    void OnButtonDown       (wxCommandEvent &);
    void OnButtonLeft       (wxCommandEvent &);
    void OnButtonRight      (wxCommandEvent &);

    void OnSetPosition(wxCommandEvent &);
    void OnNewGame    (wxCommandEvent &);
    void OnTakeback   (wxCommandEvent &);
        void OnUpdateTakeback(wxUpdateUIEvent &);
    void OnMoveNow    (wxCommandEvent &);
        void OnUpdateMoveNow(wxUpdateUIEvent &);
    void OnDraw       (wxCommandEvent &);
        void OnUpdateDraw(wxUpdateUIEvent &);
    void OnWhiteResigns(wxCommandEvent &);
        void OnUpdateWhiteResigns(wxUpdateUIEvent &);
    void OnBlackResigns(wxCommandEvent &);
        void OnUpdateBlackResigns(wxUpdateUIEvent &);
    void OnPlayWhite  (wxCommandEvent &);
        void OnUpdatePlayWhite(wxUpdateUIEvent &);
    void OnPlayBlack  (wxCommandEvent &);
        void OnUpdatePlayBlack(wxUpdateUIEvent &);
    void OnSwapSides  (wxCommandEvent &);
        void OnUpdateSwapSides(wxUpdateUIEvent &);
    void OnKibitz    (wxCommandEvent &);
        void OnUpdateKibitz(wxUpdateUIEvent &);
    void OnClearKibitz    (wxCommandEvent &);
        void OnUpdateClearKibitz(wxUpdateUIEvent &);
    void OnPlayers    (wxCommandEvent &);
        void OnUpdatePlayers(wxUpdateUIEvent &);
    void OnClocks     (wxCommandEvent &);
        void OnUpdateClocks(wxUpdateUIEvent &);
    void OnOptionsReset(wxCommandEvent &);
        void OnUpdateOptionsReset(wxUpdateUIEvent &);
    void OnBook       (wxCommandEvent &);
        void OnUpdateBook(wxUpdateUIEvent &);
    void OnLog        (wxCommandEvent &);
        void OnUpdateLog(wxUpdateUIEvent &);
    void OnEngine     (wxCommandEvent &);
        void OnUpdateEngine(wxUpdateUIEvent &);
    void OnTraining   (wxCommandEvent &);
        void OnUpdateTraining(wxUpdateUIEvent &);
    void OnGeneral    (wxCommandEvent &);
        void OnUpdateGeneral (wxUpdateUIEvent &);
    void RefreshLanguageFont( const char *from, bool before_large_font, bool before_no_italics,
                              const char *to,   bool after_large_font,  bool after_no_italics );

    void OnFileNew (wxCommandEvent &);
        void OnUpdateFileNew( wxUpdateUIEvent &);
    void OnFileOpen (wxCommandEvent &);
        void OnUpdateFileOpen(wxUpdateUIEvent &);
    void OnFileOpenLog (wxCommandEvent &);
        void OnUpdateFileOpenLog(wxUpdateUIEvent &);
    void OnFileSave (wxCommandEvent &);
        void OnUpdateFileSave( wxUpdateUIEvent &);
    void OnFileSaveAs (wxCommandEvent &);
        void OnUpdateFileSaveAs( wxUpdateUIEvent &);
    void OnFileCurrent (wxCommandEvent &);
        void OnUpdateFileCurrent( wxUpdateUIEvent &);
    void OnFileSession (wxCommandEvent &);
        void OnUpdateFileSession( wxUpdateUIEvent &);
    void OnFileClipboard (wxCommandEvent &);
        void OnUpdateFileClipboard( wxUpdateUIEvent &);
    void OnNextGame (wxCommandEvent &);
        void OnUpdateNextGame( wxUpdateUIEvent &);
    void OnPreviousGame (wxCommandEvent &);
        void OnUpdatePreviousGame( wxUpdateUIEvent &);

    void OnEditCopy (wxCommandEvent &);
        void OnUpdateEditCopy( wxUpdateUIEvent &);
    void OnEditCut (wxCommandEvent &);
        void OnUpdateEditCut( wxUpdateUIEvent &);
    void OnEditPaste (wxCommandEvent &);
        void OnUpdateEditPaste( wxUpdateUIEvent &);
    void OnEditUndo (wxCommandEvent &);
        void OnUpdateEditUndo( wxUpdateUIEvent &);
    void OnEditRedo (wxCommandEvent &);
        void OnUpdateEditRedo( wxUpdateUIEvent &);
    void OnEditDelete (wxCommandEvent &);
        void OnUpdateEditDelete( wxUpdateUIEvent &);
    void OnEditGameDetails (wxCommandEvent &);
        void OnUpdateEditGameDetails( wxUpdateUIEvent &);
    void OnEditCopyGamePGNToClipboard (wxCommandEvent &);
        void OnUpdateEditCopyGamePGNToClipboard(wxUpdateUIEvent &);
    void OnEditPromote (wxCommandEvent &);
        void OnUpdateEditPromote( wxUpdateUIEvent &);
    void OnEditDemote (wxCommandEvent &);
        void OnUpdateEditDemote( wxUpdateUIEvent &);
    void OnEditDemoteToComment (wxCommandEvent &);
        void OnUpdateEditDemoteToComment( wxUpdateUIEvent &);
    void OnEditPromoteToVariation (wxCommandEvent &);
        void OnUpdateEditPromoteToVariation( wxUpdateUIEvent &);
    void OnEditPromoteRestToVariation (wxCommandEvent &);
        void OnUpdateEditPromoteRestToVariation( wxUpdateUIEvent &);


    DECLARE_EVENT_TABLE()
private:
    Canvas *canvas;
    wxTimer m_timer;
    void SetFocusOnList() { if(canvas) canvas->SetFocusOnList(); }
};

//-----------------------------------------------------------------------------
// Application class
//-----------------------------------------------------------------------------

IMPLEMENT_APP(ChessApp)

extern void JobBegin();
extern void JobEnd();

wxString argv1;
bool gbl_small_screen_detected;  // nasty global variable hack, sorry

bool ChessApp::OnInit()
{
    srand(time(NULL));
    //_CrtSetBreakAlloc( 195273 );
    //_CrtSetBreakAlloc( 195274 );
    //_CrtSetBreakAlloc( 272212 );
    JobBegin();
    if( argc == 2 )
    {
        argv1 = argv[1];
    }
    wxString error_msg;
    int disp_width, disp_height;
    wxDisplaySize(&disp_width, &disp_height);
    DebugPrintf(( "Display size = %d x %d\n", disp_width, disp_height ));
    objs.repository = new Repository;
    #if 0 // small screen testing
    wxSize  win_size  = wxSize(708, 596);
    wxPoint win_point = wxPoint(20, 0);
    #else
    wxSize  win_size  = wxSize(708, 738); //710); //680);
    wxPoint win_point = wxPoint(20, 20);
    gbl_small_screen_detected = (disp_height<768);
    if( gbl_small_screen_detected || objs.repository->general.m_small_board )
    {
        win_size  = wxSize(708, 596);
        win_point = wxPoint(20, 0);
    }
    else if( disp_height < 800 )
        win_point = wxPoint(20, 0);
    #endif
    ChessFrame *frame = new ChessFrame (_T("Tarrasch Chess GUI"),
                                  win_point, win_size );
    objs.frame = frame;
    frame->Show (true);
    SetTopWindow (frame);
    if( objs.gl )
        objs.gl->StatusUpdate();
    return true;
}

int ChessApp::OnExit()
{
    DebugPrintf(( "ChessApp::OnExit()\n" ));
    if( objs.rybka )
    {
        delete objs.rybka;
        objs.rybka = NULL;
    }
    JobEnd();
    if( objs.gl )
    {
        delete objs.gl;
        objs.gl = NULL;
    }
    if( objs.cws )
    {
        delete objs.cws;
        objs.cws = NULL;
    }
    if( objs.repository )
    {
        delete objs.repository;
        objs.repository = NULL;
    }
    if( objs.book )
    {
        delete objs.book;
        objs.book = NULL;
    }
    if( objs.log )
    {
        delete objs.log;
        objs.log = NULL;
    }
    if( objs.session )
    {
        delete objs.session;
        objs.session = NULL;
    }
    return wxApp::OnExit();
}

//-----------------------------------------------------------------------------
// Main frame
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(ChessFrame, wxFrame)
    EVT_CLOSE(                     ChessFrame::OnClose )
    EVT_MENU (wxID_EXIT,           ChessFrame::OnQuit)
    EVT_MENU (ID_CMD_ABOUT,        ChessFrame::OnAbout)
    EVT_MENU (ID_HELP_HELP,        ChessFrame::OnHelp)
    EVT_MENU (ID_HELP_CREDITS,     ChessFrame::OnCredits)
    EVT_MENU (ID_CMD_FLIP,         ChessFrame::OnFlip)
    EVT_MENU (ID_CMD_KIBITZ,       ChessFrame::OnKibitz)
        EVT_UPDATE_UI (ID_CMD_KIBITZ,      ChessFrame::OnUpdateKibitz)  
    EVT_MENU (ID_CMD_CLEAR_KIBITZ,      ChessFrame::OnClearKibitz)
        EVT_UPDATE_UI (ID_CMD_CLEAR_KIBITZ,      ChessFrame::OnUpdateClearKibitz)  
    EVT_MENU (ID_CMD_SET_POSITION, ChessFrame::OnSetPosition)
    EVT_MENU (ID_CMD_NEW_GAME,     ChessFrame::OnNewGame)
    EVT_MENU (wxID_UNDO,           ChessFrame::OnEditUndo)
        EVT_UPDATE_UI (wxID_UNDO,           ChessFrame::OnUpdateEditUndo)
    EVT_MENU (wxID_REDO,           ChessFrame::OnEditRedo)
        EVT_UPDATE_UI (wxID_REDO,           ChessFrame::OnUpdateEditRedo)
    EVT_MENU (ID_CMD_TAKEBACK,     ChessFrame::OnTakeback)
        EVT_UPDATE_UI (ID_CMD_TAKEBACK,     ChessFrame::OnUpdateTakeback)
    EVT_MENU (ID_CMD_MOVENOW,      ChessFrame::OnMoveNow)
        EVT_UPDATE_UI (ID_CMD_MOVENOW,      ChessFrame::OnUpdateMoveNow)
    EVT_MENU (ID_CMD_DRAW,         ChessFrame::OnDraw)
        EVT_UPDATE_UI (ID_CMD_DRAW,         ChessFrame::OnUpdateDraw)
    EVT_MENU (ID_CMD_PLAY_WHITE,   ChessFrame::OnPlayWhite)
        EVT_UPDATE_UI (ID_CMD_PLAY_WHITE,   ChessFrame::OnUpdatePlayWhite)
    EVT_MENU (ID_CMD_PLAY_BLACK,   ChessFrame::OnPlayBlack)
        EVT_UPDATE_UI (ID_CMD_PLAY_BLACK,   ChessFrame::OnUpdatePlayBlack)
    EVT_MENU (ID_CMD_WHITE_RESIGNS,ChessFrame::OnWhiteResigns)
        EVT_UPDATE_UI (ID_CMD_WHITE_RESIGNS,ChessFrame::OnUpdateWhiteResigns)
    EVT_MENU (ID_CMD_BLACK_RESIGNS,ChessFrame::OnBlackResigns)
        EVT_UPDATE_UI (ID_CMD_BLACK_RESIGNS,ChessFrame::OnUpdateBlackResigns)
    EVT_MENU (ID_CMD_SWAP_SIDES,   ChessFrame::OnSwapSides)
        EVT_UPDATE_UI (ID_CMD_SWAP_SIDES,   ChessFrame::OnUpdateSwapSides)

    EVT_MENU (wxID_NEW,         ChessFrame::OnFileNew)
        EVT_UPDATE_UI (wxID_NEW,    ChessFrame::OnUpdateFileNew)
    EVT_MENU (wxID_OPEN,        ChessFrame::OnFileOpen)
        EVT_UPDATE_UI (wxID_OPEN,   ChessFrame::OnUpdateFileOpen)
    EVT_MENU (ID_FILE_OPEN_LOG, ChessFrame::OnFileOpenLog)
        EVT_UPDATE_UI (ID_FILE_OPEN_LOG,   ChessFrame::OnUpdateFileOpenLog)
    EVT_MENU (wxID_SAVE,        ChessFrame::OnFileSave)
        EVT_UPDATE_UI (wxID_SAVE,   ChessFrame::OnUpdateFileSave)
    EVT_MENU (wxID_SAVEAS,      ChessFrame::OnFileSaveAs)
        EVT_UPDATE_UI (wxID_SAVEAS, ChessFrame::OnUpdateFileSaveAs)
    EVT_MENU (ID_FILE_CURRENT,      ChessFrame::OnFileCurrent)
        EVT_UPDATE_UI (ID_FILE_CURRENT, ChessFrame::OnUpdateFileCurrent)
    EVT_MENU (ID_FILE_SESSION,      ChessFrame::OnFileSession)
        EVT_UPDATE_UI (ID_FILE_SESSION, ChessFrame::OnUpdateFileSession)
    EVT_MENU (ID_FILE_CLIPBOARD,ChessFrame::OnFileClipboard)
        EVT_UPDATE_UI (ID_FILE_CLIPBOARD,    ChessFrame::OnUpdateFileClipboard)
    EVT_MENU (ID_CMD_NEXT_GAME,     ChessFrame::OnNextGame)
        EVT_UPDATE_UI (ID_CMD_NEXT_GAME,     ChessFrame::OnUpdateNextGame)
    EVT_MENU (ID_CMD_PREVIOUS_GAME,     ChessFrame::OnPreviousGame)
        EVT_UPDATE_UI (ID_CMD_PREVIOUS_GAME,  ChessFrame::OnUpdatePreviousGame)

    EVT_MENU (wxID_COPY,                    ChessFrame::OnEditCopy)    
        EVT_UPDATE_UI (wxID_COPY,                       ChessFrame::OnUpdateEditCopy)
    EVT_MENU (wxID_CUT,                     ChessFrame::OnEditCut)        
        EVT_UPDATE_UI (wxID_CUT,                        ChessFrame::OnUpdateEditCut)
    EVT_MENU (wxID_PASTE,                   ChessFrame::OnEditPaste)        
        EVT_UPDATE_UI (wxID_PASTE,                      ChessFrame::OnUpdateEditPaste)
    EVT_MENU (wxID_DELETE,                  ChessFrame::OnEditDelete)        
        EVT_UPDATE_UI (wxID_DELETE,                     ChessFrame::OnUpdateEditDelete)
    EVT_MENU (ID_EDIT_GAME_DETAILS,         ChessFrame::OnEditGameDetails)        
        EVT_UPDATE_UI (ID_EDIT_GAME_DETAILS,            ChessFrame::OnUpdateEditGameDetails)
    EVT_MENU(ID_COPY_GAME_PGN_TO_CLIPBOARD,              ChessFrame::OnEditCopyGamePGNToClipboard)
        EVT_UPDATE_UI(ID_COPY_GAME_PGN_TO_CLIPBOARD,                 ChessFrame::OnUpdateEditCopyGamePGNToClipboard)
    EVT_MENU (ID_EDIT_PROMOTE,              ChessFrame::OnEditPromote)        
        EVT_UPDATE_UI (ID_EDIT_PROMOTE,                 ChessFrame::OnUpdateEditPromote)
    EVT_MENU (ID_EDIT_DEMOTE,               ChessFrame::OnEditDemote)    
        EVT_UPDATE_UI (ID_EDIT_DEMOTE,                  ChessFrame::OnUpdateEditDemote)
    EVT_MENU (ID_EDIT_DEMOTE_TO_COMMENT,    ChessFrame::OnEditDemoteToComment)    
        EVT_UPDATE_UI (ID_EDIT_DEMOTE_TO_COMMENT,       ChessFrame::OnUpdateEditDemoteToComment)
    EVT_MENU (ID_EDIT_PROMOTE_TO_VARIATION, ChessFrame::OnEditPromoteToVariation)    
        EVT_UPDATE_UI (ID_EDIT_PROMOTE_TO_VARIATION,    ChessFrame::OnUpdateEditPromoteToVariation)
    EVT_MENU (ID_EDIT_PROMOTE_REST_TO_VARIATION, ChessFrame::OnEditPromoteRestToVariation)    
        EVT_UPDATE_UI (ID_EDIT_PROMOTE_REST_TO_VARIATION,    ChessFrame::OnUpdateEditPromoteRestToVariation)

    EVT_MENU (ID_OPTIONS_PLAYERS,  ChessFrame::OnPlayers)
        EVT_UPDATE_UI (ID_OPTIONS_PLAYERS,   ChessFrame::OnUpdatePlayers)
    EVT_MENU (ID_OPTIONS_CLOCKS,   ChessFrame::OnClocks)
        EVT_UPDATE_UI (ID_OPTIONS_CLOCKS,   ChessFrame::OnUpdateClocks)
    EVT_MENU (ID_OPTIONS_BOOK,     ChessFrame::OnBook)
        EVT_UPDATE_UI (ID_OPTIONS_BOOK,     ChessFrame::OnUpdateBook)
    EVT_MENU (ID_OPTIONS_LOG,      ChessFrame::OnLog)
        EVT_UPDATE_UI (ID_OPTIONS_LOG,      ChessFrame::OnUpdateLog)
    EVT_MENU (ID_OPTIONS_ENGINE,   ChessFrame::OnEngine)
        EVT_UPDATE_UI (ID_OPTIONS_ENGINE,   ChessFrame::OnUpdateEngine)
    EVT_MENU (ID_OPTIONS_TRAINING, ChessFrame::OnTraining)
        EVT_UPDATE_UI (ID_OPTIONS_TRAINING, ChessFrame::OnUpdateTraining)
    EVT_MENU (ID_OPTIONS_GENERAL,  ChessFrame::OnGeneral) 
        EVT_UPDATE_UI (ID_OPTIONS_GENERAL,  ChessFrame::OnUpdateGeneral)
    EVT_MENU (ID_OPTIONS_RESET,    ChessFrame::OnOptionsReset)
        EVT_UPDATE_UI (ID_OPTIONS_RESET,    ChessFrame::OnUpdateOptionsReset)
    EVT_TOOL (ID_CMD_FLIP,         ChessFrame::OnFlip)
    EVT_TOOL (ID_CMD_NEXT_GAME,    ChessFrame::OnNextGame)
    EVT_TOOL (ID_CMD_PREVIOUS_GAME,ChessFrame::OnPreviousGame)
    EVT_TOOL (ID_HELP_HELP,        ChessFrame::OnHelp)
    EVT_TOOL (ID_BUTTON_UP,        ChessFrame::OnButtonUp)
    EVT_TOOL (ID_BUTTON_DOWN,      ChessFrame::OnButtonDown)
    EVT_TOOL (ID_BUTTON_LEFT,      ChessFrame::OnButtonLeft)
    EVT_TOOL (ID_BUTTON_RIGHT,     ChessFrame::OnButtonRight)
    EVT_IDLE (ChessFrame::OnIdle)
    EVT_TIMER( TIMER_ID, ChessFrame::OnTimeout)
    EVT_MOVE (ChessFrame::OnMove) 
END_EVENT_TABLE()
CtrlBoxBookMoves *gbl_book_moves;

ChessFrame::ChessFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, pos, size)
{
    // Timer
    m_timer.SetOwner(this,TIMER_ID);

    // Menu - File
    wxMenu *menu_file     = new wxMenu;
    menu_file->Append (wxID_NEW,                    _T("New\tCtrl+N"));
    menu_file->Append (wxID_OPEN,                   _T("Open\tCtrl+O"));
    menu_file->Append (ID_FILE_OPEN_LOG,            _T("Open log file"));
    menu_file->Append (wxID_SAVE,                   _T("Save\tCtrl+S"));
    menu_file->Append (wxID_SAVEAS,                 _T("Save as"));
    menu_file->AppendSeparator();
    menu_file->Append (wxID_EXIT,                   _T("E&xit\tAlt-X"));

    // Menu - Edit
    wxMenu *menu_edit     = new wxMenu;
    menu_edit->Append (wxID_COPY,                         _T("Copy\tCtrl+C"));
    menu_edit->Append (wxID_CUT,                          _T("Cut\tCtrl+X"));
    menu_edit->Append (wxID_PASTE,                        _T("Paste\tCtrl+V"));
    menu_edit->Append (wxID_UNDO,                         _T("Undo\tCtrl+Z"));
    menu_edit->Append (wxID_REDO,                         _T("Redo\tCtrl+Y"));
    menu_edit->Append (wxID_DELETE,                       _T("Delete comment text or remainder of variation\tDel"));
    menu_edit->Append (ID_EDIT_GAME_DETAILS,              _T("Edit game details"));
    menu_edit->Append (ID_COPY_GAME_PGN_TO_CLIPBOARD,     _T("Copy game to clipboard (PGN)"));
    menu_edit->Append (ID_EDIT_PROMOTE,                   _T("Promote variation"));
    menu_edit->Append (ID_EDIT_DEMOTE,                    _T("Demote variation"));
    menu_edit->Append (ID_EDIT_DEMOTE_TO_COMMENT,         _T("Demote rest of variation to comment"));
    menu_edit->Append (ID_EDIT_PROMOTE_TO_VARIATION,      _T("Promote comment to variation"));
    menu_edit->Append (ID_EDIT_PROMOTE_REST_TO_VARIATION, _T("Promote rest of comment to variation"));

    // Menu - Games
    wxMenu *menu_games   = new wxMenu;
    menu_games->Append (ID_FILE_CURRENT,        _T("Current file"));
    menu_games->Append (ID_FILE_SESSION,        _T("Session"));
    menu_games->Append (ID_FILE_CLIPBOARD,      _T("Clipboard"));
    menu_games->AppendSeparator();
    menu_games->Append (ID_CMD_NEXT_GAME,       _T("Next game from file"));
    menu_games->Append (ID_CMD_PREVIOUS_GAME,   _T("Previous game from file"));

    // Menu - Commands
    wxMenu *menu_commands = new wxMenu;
    menu_commands->Append (ID_CMD_NEW_GAME,     _T("New Game"));
    menu_commands->Append (ID_CMD_FLIP,         _T("Flip board\tCtrl+F"));
    menu_commands->Append (ID_CMD_TAKEBACK,     _T("Take back\tCtrl+T"));
    menu_commands->Append (ID_CMD_SET_POSITION, _T("Set position\tCtrl+P"));
    menu_commands->Append (ID_CMD_KIBITZ,       _T("Start kibitzer\tCtrl+K"));
    #if 1 // auto_kibitz_clear (if auto kibitz clear, comment out manual clear)
//  menu_commands->Append (ID_CMD_CLEAR_KIBITZ, _T("Clear kibitz text"));
    #else // manual_kibitz_clear
    menu_commands->Append (ID_CMD_CLEAR_KIBITZ, _T("Clear kibitz text"));
    #endif

    menu_commands->Append (ID_CMD_DRAW,         _T("Draw"));
    menu_commands->Append (ID_CMD_WHITE_RESIGNS,_T("White resigns"));
    menu_commands->Append (ID_CMD_BLACK_RESIGNS,_T("Black resigns"));
    menu_commands->Append (ID_CMD_PLAY_WHITE,   _T("Play white"));
    menu_commands->Append (ID_CMD_PLAY_BLACK,   _T("Play black"));
    menu_commands->Append (ID_CMD_SWAP_SIDES,   _T("Swap sides"));
    menu_commands->Append (ID_CMD_MOVENOW,      _T("Move now"));

    // Options
    wxMenu *menu_options = new wxMenu;
    menu_options->Append (ID_OPTIONS_ENGINE,       _T("Engine"));
    menu_options->Append (ID_OPTIONS_GENERAL,      _T("General"));
    menu_options->Append (ID_OPTIONS_PLAYERS,      _T("Player names"));
    menu_options->Append (ID_OPTIONS_CLOCKS,       _T("Clocks"));
    menu_options->Append (ID_OPTIONS_LOG,          _T("Log file"));
    menu_options->Append (ID_OPTIONS_BOOK,         _T("Opening book"));
    menu_options->Append (ID_OPTIONS_TRAINING,     _T("Training"));
    menu_options->Append (ID_OPTIONS_RESET,        _T("Reset to factory defaults"));

    // Help
    wxMenu *menu_help     = new wxMenu;
    menu_help->Append (ID_CMD_ABOUT,                _T("About"));
    menu_help->Append (ID_HELP_HELP,                _T("Help"));
    menu_help->Append (ID_HELP_CREDITS,             _T("Credits"));

    // Menu bar
    wxMenuBar *menu = new wxMenuBar;
    menu->Append (menu_file,     _T("&File"));    
    menu->Append (menu_edit,     _T("&Edit"));    
    menu->Append (menu_games,    _T("&Games"));    
    menu->Append (menu_commands, _T("&Commands"));
    menu->Append (menu_options,  _T("&Options"));
    menu->Append (menu_help,     _T("&Help"));    
    SetMenuBar( menu );

    // Create a status bar
    CreateStatusBar(3);
    int widths[3] = {-1,-200,-100};
    SetStatusWidths(3,widths);


    wxToolBar *toolbar = new wxToolBar(this, wxID_ANY,
                               wxDefaultPosition, wxDefaultSize,
                               wxTB_HORIZONTAL|wxNO_BORDER);
//  wxBitmap bmpNew    (new_xpm);
    wxBitmap bmpOpen   (open_xpm);
    wxBitmap bmpSave   (save_xpm);
    wxBitmap bmpCopy   (copy_xpm);
    wxBitmap bmpCut    (cut_xpm);
    wxBitmap bmpPaste  (paste_xpm);
    wxBitmap bmp_undo  (undo_xpm); //(bill_undo_xpm);
    wxBitmap bmp_redo  (redo_xpm); //(bill_redo_xpm);
    wxBitmap bmp_left  (left_xpm);
    wxBitmap bmp_right (right_xpm);
    wxBitmap bmp_up    (up_xpm);
    wxBitmap bmp_down  (down_xpm);
    wxBitmap bmp_flip  (flip_xpm);
    wxBitmap bmp_help  (bill_help_xpm);
//  wxBitmap bmpNew    (bill_file_new_xpm);
//  wxBitmap bmp_game_new   (bill_game_new_xpm);
    wxBitmap bmpNew         (bill_doc_single_xpm);
    wxBitmap bmp_game_new   (bill_doc_stack_xpm);
    wxBitmap bmp_kibitzer   (bill_kibitzer_xpm);
    wxBitmap bmp_next       (bill_next_game_xpm);
    wxBitmap bmp_previous   (bill_previous_game_xpm);

    // I have modified all the icons to be 16 high (not 15) since a couple
    //  of the Reptor icons I brought across really needed 16.
    wxSize our_size(16,16);
    toolbar->SetToolBitmapSize(our_size);
    
    toolbar->AddTool( wxID_NEW,  bmpNew,  wxT("New file") );
    toolbar->AddTool( ID_CMD_NEW_GAME,  bmp_game_new,  wxT("New game") );
    toolbar->AddTool( wxID_OPEN, bmpOpen, wxT("Open") );
    toolbar->AddTool( wxID_SAVE, bmpSave, wxT("Save") );
    toolbar->AddSeparator();
    toolbar->AddSeparator();
    int nbr_separators = 0;
    nbr_separators++;
    nbr_separators++;
    toolbar->AddTool( wxID_COPY, bmpCopy, _T("Copy") ); //, toolBarBitmaps[Tool_copy], _T("Toggle button 2"), wxITEM_CHECK);
    toolbar->AddTool( wxID_CUT,  bmpCut,  _T("Cut")  ); //, toolBarBitmaps[Tool_cut], _T("Toggle/Untoggle help button"));
    toolbar->AddTool( wxID_PASTE,bmpPaste,_T("Paste")); // , toolBarBitmaps[Tool_paste], _T("Paste"));
    toolbar->AddTool( wxID_UNDO, bmp_undo, "Undo");
    toolbar->AddTool( wxID_REDO, bmp_redo, "Redo");
    toolbar->AddSeparator();
    toolbar->AddSeparator();
    nbr_separators++;
    nbr_separators++;
    toolbar->AddTool( ID_CMD_FLIP,      bmp_flip, "Flip board");
    toolbar->AddTool( ID_CMD_KIBITZ,    bmp_kibitzer,  "Kibitzer start/stop");
    toolbar->AddTool( ID_BUTTON_DOWN,   bmp_down,  "Forward (main line)");
    toolbar->AddTool( ID_BUTTON_UP,     bmp_up,    "Backward (main line)");
    toolbar->AddTool( ID_BUTTON_RIGHT,  bmp_right, "Forward" );
    toolbar->AddTool( ID_BUTTON_LEFT,   bmp_left,  "Backward" );
    toolbar->AddSeparator();
    toolbar->AddSeparator();
    nbr_separators++;
    nbr_separators++;
    toolbar->AddTool( ID_CMD_NEXT_GAME,     bmp_next,      "Next game from file"     );
    toolbar->AddTool( ID_CMD_PREVIOUS_GAME, bmp_previous,  "Previous game from file" );
    toolbar->AddSeparator();
    toolbar->AddSeparator();
    nbr_separators++;
    nbr_separators++;
    toolbar->AddTool( ID_HELP_HELP,  bmp_help, "Help");

    #if 0 // Wake up this code to generate some icon .xpm data
          //  (the link between original .bmps and generated .xpms
          //   has been muddied and perhaps even lost)  
    extern void TempCreateXpm( wxBitmap &bmp, const char *name );
    wxBitmap temp_bmp( "myicons.bmp", wxBITMAP_TYPE_BMP );
    TempCreateXpm( temp_bmp, "myicons_base" );
    wxBitmap temp_bmp4( "extra_icons.bmp", wxBITMAP_TYPE_BMP );
    TempCreateXpm( temp_bmp4, "extra_icons" );
    #endif

    // Position book moves control to right
    // toolbar->SetToolSeparation(100); // note that this approach doesn't work
    wxSize sz0 = GetSize(); // frame size
    wxSize sz1 = toolbar->GetMargins();
    wxSize sz2 = toolbar->GetToolSize();
    int space1 = toolbar->GetToolPacking();
    int nbr_tools = toolbar->GetToolsCount() - nbr_separators;
    const int SEPARATOR_WIDTH = 8;  // Can only be calculated after Realize(), so done
                                    //  offline below
    const int BOOK_MOVES_WIDTH = 100;
    int x1 = nbr_tools*sz2.x + (nbr_tools-1)*space1 + nbr_separators*SEPARATOR_WIDTH;    // where we are now
    int x2 = (sz0.x - BOOK_MOVES_WIDTH - 20);                                            // where we want to be
    nbr_separators = (x2-x1)/SEPARATOR_WIDTH;
    for( int i=0; i<nbr_separators; i++ )
        toolbar->AddSeparator();

    //wxComboBox *combobox = new wxComboBox(toolbar,ID_COMBOBOX);
    //wxPoint pt(100,0);
    wxSize sz(BOOK_MOVES_WIDTH,20);
    gbl_book_moves = new CtrlBoxBookMoves( toolbar,
                          wxID_ANY,
                          wxDefaultPosition,
                          sz );
    toolbar->AddControl(gbl_book_moves);
    toolbar->Realize();
    SetToolBar(toolbar);

    // Some Debug only code to calculate SEPARATOR_WIDTH note that
    //  GetPosition() of controls in toolbar is useless until Realize()
    #ifdef _DEBUG
    int space2 = toolbar->GetToolSeparation();
    nbr_tools = toolbar->GetToolsCount();
    int x,y;
    gbl_book_moves->GetPosition( &x, &y );
    int nbr_buttons = nbr_tools - nbr_separators - 1;   // -1 is gbl_book_moves control
    float width_separator = ( x - sz1.x - nbr_buttons*sz2.x -(nbr_tools-1)*space1
                            ) / ((double)nbr_separators);
    DebugPrintf(( "width of separator = %f (should be integer)\n", width_separator ));
    DebugPrintf(( "margin.x=%d, nbr_tools=%d, nbr_separators=%d, toolsize.x=%d, packing=%d, separation=%d\n",
            sz1.x, nbr_tools, nbr_separators, sz2.x, space1, space2 ));
    int w,h;
    int X, Y;
    wxWindow *pw = gbl_book_moves;
    for(int i=0; ; i++ )
    {
        pw->GetSize( &w, &h );
        pw->GetPosition( &x, &y );
        pw->GetScreenPosition( &X, &Y );
        bool top = pw->IsTopLevel();
        DebugPrintf(( "Book moves(%d,%s) w=%d, h=%d, X=%d, Y=%d, x=%d, y=%d\n", i, top?"top":"child",w, h, X, Y, x, y ));
        if( top )
            break;
        pw = pw->GetParent();
        if( !pw )
            break;
    }
    #endif

    canvas = new Canvas( this, wxID_ANY, wxPoint(0,0), wxSize(10,10) );
    objs.canvas = canvas;
}

void ChessFrame::OnClose( wxCloseEvent& WXUNUSED(event) )
{
    bool okay = objs.gl->OnExit();
    if( okay )
        Destroy();  // only exit if OnExit() worked okay (eg, if it wasn't cancelled)
}

void ChessFrame::OnQuit (wxCommandEvent &)
{
    Close (true);
}

void ChessFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(
        "This is the Tarrasch Chess GUI. Tarrasch is intended to be "
        "an easy to use yet powerful system for playing, analysing and "
        "training with UCI standard chess engines. Tarrasch now has "
        "full support for creating, reading and editing .pgn files."
        "\n\n"
        "Tarrasch includes the simple Tarrasch toy engine as well "
        "as one or more strong engines. "
        "The Tarrasch toy "
        "engine is particularly suited to Tarrasch's training modes. "
        "(These modes handicap the user, and you don't want to "
        "handicap yourself when you are playing against a strong engine!)."
        "\n\n"
        "Visit the publisher's website www.triplehappy.com for Tarrasch "
        "Chess GUI news and updates."
    );
    wxMessageBox(msg, "About the Tarrasch Chess GUI " MASTER_VERSION, wxOK|wxICON_INFORMATION|wxCENTRE, this);
}

/*
void ChessFrame::OnUnimplemented(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(
        "Sorry in this pre-beta version of Tarrasch V2, redo is not "
        "not yet implemented, and undo is simply takeback."
        "\n\n"
        "You can check whether a more complete version of the program "
        "is available at the publisher's main site triplehappy.com or "
        "on his blog at triplehappy.wordpress.com."
    );
    wxMessageBox(msg, "Sorry, unimplemented feature", wxOK|wxICON_INFORMATION|wxCENTRE, this);
}  */


void ChessFrame::OnHelp(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(
        "Tarrasch is firstly an electronic chessboard. You can start "
        "from the standard starting position or set up any (legal) "
        "position easily. You can move the pieces around freely. A "
        "built in chess clock can be turned on, useful if you want to "
        "play an old fashioned normal game of chess with a friend."
        "\n\n"
        "Tarrasch has an opening book, you can easily see the \"book\" "
        "moves early in a game. Tarrasch uses the color green when "
        "listing or suggesting book moves. The book can be turned off "
        "using menu \"Options\". That menu allows control over all "
        "optional features, including blindfold and delayed display "
        "training modes."
        "\n\n"
        "Your moves are recorded in a moves window as a main line "
        "plus variations. It is easy to promote or demote variations. "
        "You can freely type (and edit) comments to go with your moves. "
        "Just click in the moves "
        "window and start typing. Comments can be converted to moves "
        "and vice-versa."
        "\n\n"
        "At any time you can save your work in a new or existing .pgn "
        "file (a standard chess file format). You can load games from "
        "any .pgn file. Open a .pgn file in the \"File\" menu and "
        "select a game, later on the \"Games\" menu allows you to pick "
        "another game from the file."
        "\n\n"
        "You can use a UCI chess engine (UCI is another chess "
        "computing standard) either as an opponent, or to provide "
        "expert commentary."
        "\n\n"
        "The Tarrasch package includes one or more engines and one of "
        "these engines is used by default. Other "
        "engines can be separately purchased or downloaded. Use menu "
        "\"Options\" to select an alternative engine if you have one. "
        "You can start a game with the engine at any time, in any "
        "position, as either white or black. By default you will get "
        "a time handicap. You can change the clock settings by "
        "clicking on the clock, or using menu \"Options\". You can "
        "change player names by clicking on them or by using menu "
        "\"Options\"."
        "\n\n"
        "To turn on commentary (kibitzing in chess parlance), use "
        "the \"Commands\" menu or the Robot button. You can even "
        "get the chess engine to provide commentary when you are "
        "playing against it."
    );
    wxMessageBox(msg, "Tarrasch Chess GUI Help", wxOK|wxICON_INFORMATION|wxCENTRE, this);
}

void ChessFrame::OnCredits(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(
        "Program design and implementation: Bill Forster."
        "\n\n"
        "Foundation supporter: Yves Catineau."
        "\n\n"
        "Foundation donors: Paul Cook, Gary Baydo, Ransith Fernando, " 
        "Lukasz Berezowski, Eric Ziegler, Laurence Dayton, Albrecht Schmidt, "
        "Lloyd Standish and David Beagan."
        "\n\n"
        "Tester: Iliya Kristoff."
        "\n\n"
        "Thanks to David L Brown and the Good Companions for the chess "
        "graphics."
        "\n\n"
        "Thanks to Pawel Koziol for the icon."
        "\n\n"
        "Thanks to Yusuke Kamiyamane for some of the toolbar icons."
        "\n\n"
        "Thanks to Vasik Rajlich, the first engine author to grant "
        "permission to include a strong engine with Tarrasch."
        "\n\n"
        "Thanks to Inno Setup from Jordan Russell (jrsoftware.org), for "
        "the setup program."
        "\n\n"
        "Thanks to Julian Smart and the wxWidgets community for the GUI "
        "library."
        "\n\n"
        "Dedicated to the memory of John Victor Forster 1949-2001. We "
        "miss him every day."
    );
    wxMessageBox(msg, "Credits", wxOK|wxICON_INFORMATION|wxCENTRE, this);
}



#if 0
void CustomLog( const char *txt )
{
    static FILE *log;
    if( log == NULL )
        log = fopen( "C:\\Documents and Settings\\Bill\\My Documents\\Chess\\Engines\\custom-log.txt", "wt" );
    if( log )
    {
        static char buf[1024];
        time_t t = time(NULL);
        struct tm *p = localtime(&t);
        const char *s = asctime(p);
        strcpy( buf, s );
        if( strchr(buf,'\n') )
            *strchr(buf,'\n') = '\0';
        fprintf( log, "[%s] %s", buf, txt );
        fflush(log);
    }
}
#endif

void ChessFrame::OnMove( wxMoveEvent &event )
{
    if( objs.canvas )
        objs.canvas->OnMove();
}

void ChessFrame::OnIdle( wxIdleEvent& event )
{
    //CustomLog( "OnIdle()\n" );
    //if( objs.gl && objs.gl->OnIdleNeeded() )
    //    event.RequestMore();    // more intense idle calls required
    event.Skip();
    if( objs.gl )
    {
        objs.gl->OnIdle();
        int millisecs = 500;
        millisecs = objs.gl->MillisecsToNextSecond();
        if( millisecs < 100 )
            millisecs = 100;
        //char buf[80];
        //sprintf( buf, "Idle: %d milliseconds\n", millisecs );
        //CustomLog( buf );
        m_timer.Start( millisecs, true );
    }
}


void ChessFrame::OnTimeout( wxTimerEvent& WXUNUSED(event) )
{
    //CustomLog( "OnTimeout()\n" );
    if( objs.gl )
    {
        objs.gl->OnIdle();
        int millisecs = 1000;
        millisecs = objs.gl->MillisecsToNextSecond();
        if( millisecs < 100 )
            millisecs = 100;
        //char buf[80];
        //sprintf( buf, "Timeout: %d milliseconds\n", millisecs );
        //CustomLog( buf );
        m_timer.Start( millisecs, true );
    }
}

void ChessFrame::OnFlip (wxCommandEvent &)
{
    objs.gl->CmdFlip();
}

void ChessFrame::OnButtonUp (wxCommandEvent &)
{
    Atomic begin(false);
    objs.canvas->lb->NavigationKey( NK_UP );
}

void ChessFrame::OnButtonDown (wxCommandEvent &)
{
    Atomic begin(false);
    objs.canvas->lb->NavigationKey( NK_DOWN );
}

void ChessFrame::OnButtonLeft (wxCommandEvent &)
{
    Atomic begin(false);
    objs.canvas->lb->NavigationKey( NK_LEFT );
}

void ChessFrame::OnButtonRight (wxCommandEvent &)
{
    Atomic begin(false);
    objs.canvas->lb->NavigationKey( NK_RIGHT );
}

void ChessFrame::OnSetPosition (wxCommandEvent &)
{
    objs.gl->CmdSetPosition();
}

void ChessFrame::OnNewGame (wxCommandEvent &)
{
    objs.gl->CmdNewGame();
}

void ChessFrame::OnEditUndo (wxCommandEvent &)
{
    objs.gl->CmdEditUndo();
}

void ChessFrame::OnEditRedo (wxCommandEvent &)
{
    objs.gl->CmdEditRedo();
}

void ChessFrame::OnTakeback (wxCommandEvent &)
{
    objs.gl->CmdTakeback();
}

void ChessFrame::OnMoveNow (wxCommandEvent &)
{
    objs.gl->CmdMoveNow();
}

void ChessFrame::OnFileNew (wxCommandEvent &)
{
    objs.gl->CmdFileNew();
}

void ChessFrame::OnFileOpen (wxCommandEvent &)
{
    objs.gl->CmdFileOpen();
}

void ChessFrame::OnFileOpenLog (wxCommandEvent &)
{
    objs.gl->CmdFileOpenLog();
}

void ChessFrame::OnFileSave (wxCommandEvent &)
{
    objs.gl->CmdFileSave();
}

void ChessFrame::OnFileSaveAs (wxCommandEvent &)
{
    objs.gl->CmdFileSaveAs();
}

void ChessFrame::OnFileCurrent (wxCommandEvent &)
{
    objs.gl->CmdFileCurrent();
}

void ChessFrame::OnFileSession (wxCommandEvent &)
{
    objs.gl->CmdFileSession();
}

void ChessFrame::OnFileClipboard (wxCommandEvent &)
{
    objs.gl->CmdFileClipboard();
}

void ChessFrame::OnNextGame (wxCommandEvent &)
{
    objs.gl->CmdNextGame();
}

void ChessFrame::OnUpdateNextGame( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateNextGame();
    event.Enable(enabled);
}

void ChessFrame::OnPreviousGame (wxCommandEvent &)
{
    objs.gl->CmdPreviousGame();
}

void ChessFrame::OnUpdatePreviousGame( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdatePreviousGame();
    event.Enable(enabled);
}

void ChessFrame::OnEditCopy (wxCommandEvent &)
{
    objs.canvas->lb->Copy();
}

void ChessFrame::OnEditCut (wxCommandEvent &)
{
    objs.canvas->lb->Cut();
}

void ChessFrame::OnEditPaste (wxCommandEvent &)
{
    objs.canvas->lb->Paste();
}

void ChessFrame::OnEditDelete (wxCommandEvent &)
{
    objs.canvas->lb->Delete();
}

void ChessFrame::OnEditGameDetails (wxCommandEvent &)
{
    objs.gl->CmdEditGameDetails();
}

void ChessFrame::OnEditCopyGamePGNToClipboard(wxCommandEvent &)
{
    objs.gl->CmdEditCopyGamePGNToClipboard();
}

void ChessFrame::OnEditPromote (wxCommandEvent &)
{
    objs.gl->CmdEditPromote();
}

void ChessFrame::OnEditDemote (wxCommandEvent &)
{
    objs.gl->CmdEditDemote();
}

void ChessFrame::OnEditDemoteToComment (wxCommandEvent &)
{
    objs.gl->CmdEditDemoteToComment();
}

void ChessFrame::OnEditPromoteToVariation (wxCommandEvent &)
{
    objs.gl->CmdEditPromoteToVariation();
}

void ChessFrame::OnEditPromoteRestToVariation (wxCommandEvent &)
{
    objs.gl->CmdEditPromoteRestToVariation();
}

void ChessFrame::OnUpdateFileNew( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileOpen( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateFileOpen();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileOpenLog( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateFileOpenLog();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileCurrent( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateFileCurrent();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileSave( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileSaveAs( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileSession( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateFileClipboard( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateFileClipboard();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditCopy( wxUpdateUIEvent &event )
{
    bool enabled = (objs.canvas->lb->GetStringSelection().Length() > 0);
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditCut( wxUpdateUIEvent &event )
{
    bool enabled = (objs.canvas->lb->GetStringSelection().Length() > 0);
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditPaste( wxUpdateUIEvent &event )
{
    bool enabled = (wxTheClipboard->Open() && wxTheClipboard->IsSupported( wxDF_TEXT ));
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditUndo( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateEditUndo();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditRedo( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateEditRedo();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditDelete( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditGameDetails( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditCopyGamePGNToClipboard(wxUpdateUIEvent &event)
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditPromote( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditDemote( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditDemoteToComment( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}


void ChessFrame::OnUpdateEditPromoteToVariation( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnUpdateEditPromoteRestToVariation( wxUpdateUIEvent &event )
{
    bool enabled = true;
    event.Enable(enabled);
}

void ChessFrame::OnDraw(wxCommandEvent &)
{
    objs.gl->CmdDraw();
}

void ChessFrame::OnWhiteResigns(wxCommandEvent &)
{
    objs.gl->CmdWhiteResigns();
}

void ChessFrame::OnUpdateWhiteResigns( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateWhiteResigns();
    event.Enable(enabled);
}

void ChessFrame::OnBlackResigns(wxCommandEvent &)
{
    objs.gl->CmdBlackResigns();
}

void ChessFrame::OnUpdateBlackResigns( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateBlackResigns();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateTakeback( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateTakeback();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateMoveNow( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateMoveNow();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateDraw( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateDraw();
    event.Enable(enabled);
}

void ChessFrame::OnUpdatePlayWhite( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdatePlayWhite();
    event.Enable(enabled);
}

void ChessFrame::OnUpdatePlayBlack( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdatePlayBlack();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateSwapSides( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateSwapSides();
    event.Enable(enabled);
}

void ChessFrame::OnUpdateKibitz( wxUpdateUIEvent &event )
{
    wxString txt;
    bool enabled = objs.gl->CmdUpdateKibitz( txt );
    event.SetText( txt );
    event.Enable(enabled);
}

void ChessFrame::OnUpdateClearKibitz( wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->CmdUpdateClearKibitz();
    event.Enable(enabled);
}

void ChessFrame::OnPlayWhite (wxCommandEvent &)
{
    objs.gl->CmdPlayWhite();
}

void ChessFrame::OnPlayBlack (wxCommandEvent &)
{
    objs.gl->CmdPlayBlack();
}

void ChessFrame::OnSwapSides (wxCommandEvent &)
{
    objs.gl->CmdSwapSides();
}

void ChessFrame::OnKibitz (wxCommandEvent &)
{
    objs.gl->CmdKibitz();
}

void ChessFrame::OnClearKibitz (wxCommandEvent &)
{
    objs.gl->CmdClearKibitz( true );
}

void ChessFrame::OnPlayers(wxCommandEvent &)
{
    objs.gl->CmdPlayers();
}

void ChessFrame::OnUpdatePlayers(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnClocks(wxCommandEvent &)
{
    objs.gl->CmdClocks();
}

void ChessFrame::OnUpdateClocks(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnOptionsReset(wxCommandEvent &)
{
    bool ingame=false;
    bool     old_enabled = objs.repository->book.m_enabled;
    wxString old_file    = objs.repository->book.m_file;
    wxString old_engine_file  = objs.repository->engine.m_file;
    const char *from = LangCheckDiffBegin();
    bool before_large_font = objs.repository->general.m_large_font;
    bool before_no_italics = objs.repository->general.m_no_italics;
    delete objs.repository;
    objs.repository = new Repository(true);
    LangSet( objs.repository->general.m_notation_language );
    if( objs.repository->engine.m_file != old_engine_file )
        ingame = objs.gl->EngineChanged();
    if( objs.repository->book.m_enabled != old_enabled ||
        objs.repository->book.m_file    != old_file )
    {
        if( objs.repository->book.m_enabled )
        {
            wxString error_msg;
            bool error = objs.book->Load( error_msg, objs.repository->book.m_file ); 
            if( error )
                wxMessageBox( error_msg, "Error loading book", wxOK|wxICON_ERROR );
            objs.canvas->BookUpdate( false );
        }
    }
    if( ingame )
    {
        objs.gl->chess_clock.Clocks2Repository();
        objs.gl->chess_clock.GameStart( objs.gl->gd.master_position.WhiteToPlay() );
    }
    else
        objs.gl->chess_clock.Repository2Clocks();
    canvas->RedrawClocks();
    const char *to = LangCheckDiffEnd();
    bool after_large_font = objs.repository->general.m_large_font;
    bool after_no_italics = objs.repository->general.m_no_italics;
    RefreshLanguageFont( from, before_large_font, before_no_italics,
                           to,  after_large_font,  after_no_italics );
    SetFocusOnList();
}

void ChessFrame::OnUpdateOptionsReset(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnBook(wxCommandEvent &)
{
    BookDialog dialog( objs.repository->book, this );
    bool     old_enabled = objs.repository->book.m_enabled;
    wxString old_file    = objs.repository->book.m_file;
    if( wxID_OK == dialog.ShowModal() )
    {
        objs.repository->book = dialog.dat;
        DebugPrintf(( "file=%s, enabled=%s, limit=%d, percent=%d\n",
            dialog.dat.m_file.c_str(),
            dialog.dat.m_enabled?"yes":"no",
            dialog.dat.m_limit_moves,
            dialog.dat.m_post_limit_percent ));
        if( objs.repository->book.m_enabled != old_enabled ||
            objs.repository->book.m_file    != old_file )
        {
            if( objs.repository->book.m_enabled )
            {
                wxString error_msg;
                bool error = objs.book->Load( error_msg, objs.repository->book.m_file ); 
                if( error )
                    wxMessageBox( error_msg, "Error loading book", wxOK|wxICON_ERROR );
                objs.canvas->BookUpdate( false );
            }
            objs.gl->Refresh();
        }
    }
    SetFocusOnList();
}

void ChessFrame::OnUpdateBook(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnLog(wxCommandEvent &)
{
    LogDialog dialog( objs.repository->log, this );
    if( wxID_OK == dialog.ShowModal() )
    {
        objs.repository->log = dialog.dat;
        DebugPrintf(( "file=%s, enabled=%s\n",
            dialog.dat.m_file.c_str(),
            dialog.dat.m_enabled?"yes":"no" ));
    }
    SetFocusOnList();
}

void ChessFrame::OnUpdateLog(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnEngine(wxCommandEvent &)
{
    wxString old_file    = objs.repository->engine.m_file;
    EngineDialog dialog( objs.repository->engine, this );
    if( wxID_OK == dialog.ShowModal() )
    {
        objs.repository->engine = dialog.dat;
        DebugPrintf(( "file=%s\n",
            dialog.dat.m_file.c_str() ));
        if( old_file != objs.repository->engine.m_file )
            objs.gl->EngineChanged();
    }
    SetFocusOnList();
}

void ChessFrame::OnUpdateEngine(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnGeneral(wxCommandEvent &)
{
    const char *from = LangCheckDiffBegin();
    bool before_large_font = objs.repository->general.m_large_font;
    bool before_no_italics = objs.repository->general.m_no_italics;
    GeneralDialog dialog( this, &objs.repository->general );
    if( wxID_OK == dialog.ShowModal() )
    {
        //bool changed = ( 0 != memcmp(&dialog.dat,&objs.repository->training,sizeof(dialog.dat))  );
        objs.repository->general = dialog.dat;
        //if( changed )
        //    objs.gl->SetGroomedPosition();
        DebugPrintf(( "notation language=%s, no italics=%d, straight to game=%d\n",
                                 dialog.dat.m_notation_language.c_str(),
                                 dialog.dat.m_no_italics,
                                 dialog.dat.m_straight_to_game ));
        const char *to = LangCheckDiffEnd();
        bool after_large_font = objs.repository->general.m_large_font;
        bool after_no_italics = objs.repository->general.m_no_italics;
        RefreshLanguageFont( from, before_large_font, before_no_italics,
                             to,   after_large_font,  after_no_italics );
    }
    SetFocusOnList();
}

void ChessFrame::RefreshLanguageFont( const char *from, bool before_large_font, bool before_no_italics,
                                      const char *to,   bool after_large_font,  bool after_no_italics )
{
    if( after_large_font != before_large_font )
        objs.gl->lb->SetLargeFont( after_large_font );
    bool redisplayed = false;
    if( to )
    {

        // If the language has changed, redisplay
        long pos = objs.gl->lb->GetInsertionPoint();
        objs.gl->gd.Rebuild();
        objs.gl->gd.Redisplay(pos);
        redisplayed = true;

        // Change move_txt for all games in the caches
        for( int j=0; j<3; j++ )
        {
            GamesCache *gc=NULL;
            switch( j )
            {
                case 0: gc = &objs.gl->gc;           break;
                case 1: gc = &objs.gl->gc_clipboard; break;
                case 2: gc = &objs.gl->gc_session;   break;
            }
            for( int i=0; gc && i<gc->gds.size(); i++ )
                LangLine( gc->gds[i].moves_txt, from, to );
        }
    }

    // If the italics setting has changed, redisplay
    else if( before_no_italics != after_no_italics )
    {
        long pos = objs.gl->lb->GetInsertionPoint();
        objs.gl->gd.Rebuild();
        objs.gl->gd.Redisplay(pos);
        redisplayed = true;
    }

    // If no redisplay yet, and font change, redisplay
    if( after_large_font!=before_large_font && !redisplayed )
    {
        long pos = objs.gl->lb->GetInsertionPoint();
        objs.gl->gd.Rebuild();
        objs.gl->gd.Redisplay(pos);
    }
    objs.gl->Refresh();
}

void ChessFrame::OnUpdateGeneral(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}

void ChessFrame::OnTraining(wxCommandEvent &)
{
    TrainingDialog dialog( this );
    dialog.dat = objs.repository->training;
    if( wxID_OK == dialog.ShowModal() )
    {
        //bool changed = ( 0 != memcmp(&dialog.dat,&objs.repository->training,sizeof(dialog.dat))  );
        objs.repository->training = dialog.dat;
        //if( changed )
        //    objs.gl->SetGroomedPosition();
        DebugPrintf(( "nbr_half=%d, hide wpawn=%d, hide wpiece=%d,"
                                 " hide bpawn=%d, hide bpiece=%d\n",
                                 dialog.dat.m_nbr_half_moves_behind,
                                 dialog.dat.m_blindfold_hide_white_pawns,
                                 dialog.dat.m_blindfold_hide_white_pieces,
                                 dialog.dat.m_blindfold_hide_black_pawns,
                                 dialog.dat.m_blindfold_hide_black_pieces ));
    }
    objs.gl->Refresh();
    SetFocusOnList();
}

void ChessFrame::OnUpdateTraining(wxUpdateUIEvent &event )
{
    bool enabled = objs.gl->UpdateOptions();
    event.Enable(enabled);
}
