/****************************************************************************
 * Chess classes - Move
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef MOVE_H
#define MOVE_H
#include "ChessDefs.h"
#include <string>

// TripleHappyChess
namespace thc
{
class ChessRules;

// Our representation of a chess move
//
// Note this is really an old school C struct, designed for speed
// There is no contructor on purpose, we don't want unnecessary
// contruction of an array of Moves in a MOVELIST when we are running
// the fast move generator.
// The default assignment operator (bitwise copy) is ideal.
// We define bitwise == and != operators
// At one time we had 4 (count em, 4) move representations, but this
// was always the primary and main representation. The others were
// FMOVE (16 bit moves), NMOVE (natural string representation, eg
// "Nf3") and TMOVE (terse string representation eg "g1f3"). When I
// realised I could streamline an IMOVE (the old name for Move =
// internal move) to only 32 bits, I realised I could live without
// FMOVEs and that sparked a large simplification exercise.
//
class Move
{
public:
    // Move is a lightweight type, it is accomodated in only 32 bits
    Square  src       : 8;
    Square  dst       : 8;
    SPECIAL special   : 8;
    int     capture   : 8;      // ' ' (empty) if move not a capture   
                                // for some reason Visual C++ 2005 (at least)
                                // blows sizeof(Move) out to 64 bits if
                                // capture is defined as char instead of int

    bool operator ==(const Move &other) const
    {
        return( *((int *)this) == *((int *)(&other)) );
    }

    bool operator !=(const Move &other) const
    {
        return( *((int *)this) != *((int *)(&other)) );
    }

    // Use these sparingly when you need to specifically mark
    //  a move as not yet set up (defined when we got rid of
    //  16 bit FMOVEs, we could always set and test 0 with those)
    void Invalid()  { src=a8; dst=a8; }
    bool Valid()    { return src!=a8 || dst!=a8; }

    // Read natural string move eg "Nf3"
    //  return bool okay
    bool NaturalIn( ChessRules *cr, const char *natural_in );

    // Read terse string move eg "g1f3"
    //  return bool okay
    bool TerseIn( ChessRules *cr, const char *tmove );

    // Convert to natural string
    //  eg "Nf3"
    std::string Move::NaturalOut( ChessRules *cr );

    // Convert to terse string eg "e7e8q"
    std::string Move::TerseOut();
};

// List of moves
struct MOVELIST
{
    int count;	// number of moves
    Move moves[MAXMOVES];
};

} //namespace thc

#endif //MOVE_H
