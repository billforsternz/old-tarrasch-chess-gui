/****************************************************************************
 * Undo/Redo
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef UNDO_H
#define UNDO_H
#include <deque>
#include "GameState.h"
#include "GameDocument.h"
#include "GameLifeCycle.h"
#include "ChessPosition.h"

struct RestorePoint
{
    MoveTree    tree;                     // the moves
    long        previous_posn;
    long        posn;
    std::string result;
    GAME_STATE  state;
    thc::Move   ponder_move;
    bool        human_is_white;
    GAME_RESULT game_result;
    bool        normal_orientation;
    bool        takeback;
};

class GameLogic;
class Undo
{
public:

	// Init
	Undo( GameLogic *gl );
    void Clear( GameDocument &gd, GAME_STATE game_state=MANUAL );

public:
    void Save( long undo_previous_posn, GameDocument &gd, GAME_STATE game_state=MANUAL );
    GAME_STATE DoUndo( GameDocument &gd, bool takeback=false );
    GAME_STATE DoRedo( GameDocument &gd );
    bool IsModified();
    bool CanRedo();
private:
    enum { NORMAL, UNDOING } state;
    bool no_front_pops_yet;
    std::deque<RestorePoint> stack;
    std::deque<RestorePoint>::iterator it_saved;
    GameLogic *gl;
};

#endif // UNDO_H
