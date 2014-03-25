/****************************************************************************
 * Log everything to a .pgn file
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef LOG_H
#define LOG_H
#include "GameDocument.h"

class Log
{
public:

	// Init
	Log();

public:
    void SaveGame( GameDocument *gd, bool editing_log );
    void Gameover() {}
private:
    std::string head;
    std::string body;
};

#endif // LOG_H
