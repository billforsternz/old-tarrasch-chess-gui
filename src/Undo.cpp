/****************************************************************************
 * Undo/Redo
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include "Undo.h"
#include "Objects.h"
#include "GameLogic.h"

// Init
Undo::Undo( GameLogic *gl )
{
    this->gl = gl;
    no_front_pops_yet = true;
    state = NORMAL;
}

void Undo::Clear( GameDocument &gd, GAME_STATE game_state )
{
    stack.clear();
    state = NORMAL;
    no_front_pops_yet = true;
    Save( 0, gd, game_state );
    gl->atom.NotUndoAble();
}

bool Undo::IsModified()
{
    bool modified = true;
    if( no_front_pops_yet )
    {
        if( stack.size() <= 1 )
            modified = false;
        else if( state==UNDOING && it_saved==stack.begin() )
            modified = false;
    }
    return modified;
}

void Undo::Save( long undo_previous_posn, GameDocument &gd, GAME_STATE game_state )
{
    RestorePoint rp;
    rp.tree = gd.tree;
    rp.previous_posn = undo_previous_posn;
    rp.posn = gd.GetInsertionPoint();
    rp.result = gd.result;
    rp.state = game_state;
    rp.ponder_move = gl->ponder_move;
    rp.human_is_white = gl->glc.human_is_white;
    rp.game_result = gl->glc.result;
    if( objs.canvas )
        rp.normal_orientation = objs.canvas->GetNormalOrientation();
    else
        rp.normal_orientation = true;
    if( state == UNDOING )
    {
        // remove the tail        }
        while( stack.size() )
        {
            if( it_saved+1 == stack.end() )
                break;
            stack.pop_back();
        }
    }
    stack.push_back(rp);
    state = NORMAL;
}

GAME_STATE Undo::DoUndo( GameDocument &gd, bool takeback )
{
    GAME_STATE ret=MANUAL;
    int len = stack.size();
    if( len )
    {
        std::deque<RestorePoint>::iterator it;
        if( state == NORMAL )
            it = stack.end()-1;
        else if( state==UNDOING )
            it = it_saved;
        if( it > stack.begin() )
        {
            it->takeback = takeback;
            long posn = it->previous_posn;
            it--;
            it_saved = it;
            RestorePoint rp;
            rp = *it;
            ret = rp.state;
            gd.result = rp.result;
            gd.tree = rp.tree;
            gl->ponder_move = rp.ponder_move;
            gl->glc.human_is_white = rp.human_is_white;
            gl->glc.result = rp.game_result;
            objs.canvas->SetNormalOrientation( rp.normal_orientation );
            gd.Rebuild();
            gd.Redisplay( posn );
            state = UNDOING;
        }
    }
    return ret;
}

bool Undo::CanRedo()
{
    bool can_redo = false;
    int len = stack.size();
    if( len && state==UNDOING )
    {
        std::deque<RestorePoint>::iterator it = it_saved;
        if( it+1 != stack.end() )
            can_redo = true;
    }
    return can_redo;
}

GAME_STATE Undo::DoRedo( GameDocument &gd )
{
    GAME_STATE ret=MANUAL;
    int len = stack.size();
    if( len && state==UNDOING )
    {
        std::deque<RestorePoint>::iterator it = it_saved;
        if( it+1 == stack.end() )
            state = NORMAL;
        else
        {
            it++;
            it_saved = it;
            RestorePoint rp;
            rp = *it;
            #if 0   //put this back if you want redo after takeback to restart the game
                    // (not very good because if say engine plays move that will kill
                    //  the redo tail)
            ret = rp.takeback ? rp.state : MANUAL;
            #endif
            gd.tree = rp.tree;
            gd.result = rp.result;
            gl->ponder_move = rp.ponder_move;
            gl->glc.human_is_white = rp.human_is_white;
            gl->glc.result = rp.game_result;
            objs.canvas->SetNormalOrientation( rp.normal_orientation );
            gd.Rebuild();
            gd.Redisplay( rp.posn );
        }
    }
    return ret;
}


