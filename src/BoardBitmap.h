/******************************************************************************
 * Access text version of board bitmap
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ******************************************************************************/
#ifndef BOARD_BITMAP_H
#define BOARD_BITMAP_H

class BoardBitmap
{
public:
    virtual const char **GetXpm()            = 0;
    virtual const char *GetWhiteKingMask()   = 0;
    virtual const char *GetWhiteQueenMask()  = 0;
    virtual const char *GetWhiteRookMask()   = 0;
    virtual const char *GetWhiteBishopMask() = 0;
    virtual const char *GetWhiteKnightMask() = 0;
    virtual const char *GetWhitePawnMask()   = 0;
    virtual const char *GetBlackKingMask()   = 0;
    virtual const char *GetBlackQueenMask()  = 0;
    virtual const char *GetBlackRookMask()   = 0;
    virtual const char *GetBlackBishopMask() = 0;
    virtual const char *GetBlackKnightMask() = 0;
    virtual const char *GetBlackPawnMask()   = 0;
};

#endif //BOARD_BITMAP_H
