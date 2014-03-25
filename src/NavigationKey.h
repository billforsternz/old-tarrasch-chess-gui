/******************************************************************************
 * Navigation key enumeration (left,right,up,down,home,end etc.)
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ******************************************************************************/
#ifndef NAVIGATION_KEY_H
#define NAVIGATION_KEY_H

enum NAVIGATION_KEY
{
    NK_NULL,
    NK_UP,
    NK_DOWN,
    NK_LEFT,
    NK_RIGHT,
    NK_HOME,
    NK_END,
    NK_PGUP,
    NK_PGDOWN,
    NK_DELETE
};

#endif // NAVIGATION_KEY_H
