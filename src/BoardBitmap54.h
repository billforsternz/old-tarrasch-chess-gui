/****************************************************************************
 * Text version of board bitmap
 *  54 pixel squares version
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef BOARD_BITMAP_54_H
#define BOARD_BITMAP_54_H

#include "BoardBitmap.h"

class BoardBitmap54 : public BoardBitmap
{
public:
    const char **GetXpm();
    const char *GetWhiteKingMask()   ;
    const char *GetWhiteQueenMask()  ;
    const char *GetWhiteRookMask()   ;
    const char *GetWhiteBishopMask() ;
    const char *GetWhiteKnightMask() ;
    const char *GetWhitePawnMask()   ;
    const char *GetBlackKingMask()   ;
    const char *GetBlackQueenMask()  ;
    const char *GetBlackRookMask()   ;
    const char *GetBlackBishopMask() ;
    const char *GetBlackKnightMask() ;
    const char *GetBlackPawnMask()   ;
};

#endif // BOARD_BITMAP_54_H
