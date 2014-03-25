/****************************************************************************
 * Keep a record of all games in a session
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef SESSION_H
#define SESSION_H
#include "GameDocument.h"

class Session
{
public:

	// Init
	Session();

public:
    void SaveGame( GameDocument *gd );
    void Gameover() {}
};

#endif // SESSION_H
