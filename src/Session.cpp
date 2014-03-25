/****************************************************************************
 * Keep a record of all games in a session
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "GameLogic.h"
#include "GameDocument.h"
#include "DebugPrintf.h"
#include "Repository.h"
#include "Objects.h"
#include "Session.h"
using namespace std;
using namespace thc;

// Init
Session::Session()
{
}

void Session::SaveGame( GameDocument *gd )
{
    if( !gd->IsEmpty() )
    {
        gd->in_memory = true;
        gd->FleshOutDate();
        gd->FleshOutMoves();
        bool diff=true;
        int sz = objs.gl->gc_session.gds.size();
        if( sz )
        {
            GameDocument temp = objs.gl->gc_session.gds[sz-1];
            diff = gd->IsDiff( temp );
        }
        if( diff )
            objs.gl->gc_session.gds.push_back(*gd);
    }
}
