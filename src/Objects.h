/****************************************************************************
 * A collection of important subsystems, i.e. a box of objects
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef OBJECTS_H
#define OBJECTS_H

// The object box has pointers to objects, rather than objects themselves,
//  so we can use forward references rather than including all the objects
class  Canvas;
class  GameLogic;
class  CentralWorkSaver;
class  GraphicBoard;
class  wxListCtrl;
class  PopupControl;
class  Rybka;
class  wxFrame;
class  Repository;
class  Book;
class  Log;
class  Session;

class Objects
{
public:
    GameLogic    *gl;
    Rybka        *rybka;
    wxFrame      *frame;
    Canvas       *canvas;
    Repository   *repository;
    Book         *book;
    Log          *log;
    Session      *session;
    CentralWorkSaver *cws;
};

// Singleton
extern Objects objs;

#endif // OBJECTS_H